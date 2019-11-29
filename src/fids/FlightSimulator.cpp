#include "FlightSimulator.h"
#include "System.h"
#include <algorithm>
#include <QTime>
#include <QVariant>


FlightSimulator& FlightSimulator::getInstance()
{
	static FlightSimulator inst;
	return inst;
}



/*void FlightSimulator::testTime(const QString& stime, const QString& slastRoundedTime)
{
	QTime time = QTime::fromString(stime);
	QTime lastRoundedTime = QTime::fromString(slastRoundedTime);
	QTime rounded = roundExpectedDepartureTime(time, lastRoundedTime);
	printf("%s, %s   ->   %s\n",
			lastRoundedTime.toString("HH:mm:ss").toUtf8().constData(),
			time.toString("HH:mm:ss").toUtf8().constData(),
			rounded.toString("HH:mm:ss").toUtf8().constData());
}*/


FlightSimulator::FlightSimulator()
		: mode(ModeInvalid)
{
}


void FlightSimulator::start()
{
	System& sys = System::getInstance();

	beginBoardingTimeDeviation = sys.requireInt64Option("/simulation/beginBoardingTimeDeviation");

	boardingTimeAverage = sys.requireInt64Option("/simulation/boardingTimeAverage");
	boardingTimeDeviation = sys.requireInt64Option("/simulation/boardingTimeDeviation");

	gateCloseTime = sys.requireInt64Option("/simulation/gateCloseTime");

	expectedDepartureTimeModulo = sys.requireInt64Option("/simulation/expectedDepartureTimeModulo");
	expectedDepartureTimeHysteresis = sys.requireInt64Option("/simulation/expectedDepartureTimeHysteresis");

	departLingerTime = sys.requireInt64Option("/simulation/departLingerTime");

	delayMax = sys.requireInt64Option("/simulation/delayMax");

	const rapidjson::Value* jevents = sys.getArrayOption("/simulation/events");

	for (size_t i = 0 ; i < jevents->Size() ; i++) {
		const rapidjson::Value& jevt = (*jevents)[i];

		Event* evt = new Event;
		connect(&evt->timer, SIGNAL(timeout()), this, SLOT(eventOccurred()));
		evt->jevt = &jevt;
		events << evt;
	}

	for (Flight* flight : plan) {
		initFlight(flight);
	}

	connect(&plan, SIGNAL(flightAdded(Flight*)), this, SLOT(flightAdded(Flight*)));

	connect(&sys, SIGNAL(simulatedDateTimeChanged(const QDateTime&)), this, SLOT(simulatedDateTimeChanged(const QDateTime&)));

	statusUpdateTimer.setSingleShot(false);
	statusUpdateTimer.setInterval(sys.getIntOption("/simulation/updateInterval", 1) * 1000);
	connect(&statusUpdateTimer, SIGNAL(timeout()), this, SLOT(statusUpdateTick()));

	cancelUpdateTimer.setSingleShot(false);
	cancelUpdateTimer.setInterval(sys.getIntOption("/simulation/cancelInterval", 1000));
	connect(&cancelUpdateTimer, SIGNAL(timeout()), this, SLOT(cancelUpdateTick()));

	connect(&delayedModeChangeTimer, SIGNAL(timeout()), this, SLOT(delayedModeChangeTriggered()));

	enterMode(ModeNormal);
}


void FlightSimulator::enterMode(Mode mode)
{
	if (this->mode == mode) {
		return;
	}

	System& sys = System::getInstance();

	Mode oldMode = this->mode;
	this->mode = mode;

	if (mode == ModeNormal) {
		reloadPlan();

		cancelUpdateTimer.stop();
		statusUpdateTimer.start();

		for (Event* evt : events) {
			evt->timer.stop();
		}
		for (Event* evt : events) {
			scheduleEvent(evt, true);
		}
	} else if (mode == ModeCancelled) {
		for (Event* evt : events) {
			evt->timer.stop();
		}

		int cancelNumMin = sys.requireIntOption("/simulation/cancelNumMin");
		int cancelNumMax = sys.requireIntOption("/simulation/cancelNumMax");
		cancelNumLeft = cancelNumMin + rand() % (cancelNumMax-cancelNumMin+1);

		statusUpdateTimer.stop();

		cancelUpdateTick();
		cancelUpdateTimer.start();
	} else if (mode == ModeInvalid) {
		cancelUpdateTimer.stop();
		statusUpdateTimer.stop();

		for (Event* evt : events) {
			evt->timer.stop();
		}

		clearPlan();
	}
}


void FlightSimulator::enterModeDelayed(Mode mode, int delay)
{
	delayedMode = mode;

	delayedModeChangeTimer.stop();

	delayedModeChangeTimer.setSingleShot(true);
	delayedModeChangeTimer.setInterval(delay);
	delayedModeChangeTimer.start();
}


void FlightSimulator::delayedModeChangeTriggered()
{
	enterMode(delayedMode);
}


void FlightSimulator::clearPlan()
{
	plan.clear();
}


void FlightSimulator::reloadPlan()
{
	System& sys = System::getInstance();

	int64_t loadCutoffTime = sys.getInt64Option("/simulation/loadCutoffTime", 300);

	clearPlan();

	plan.loadFlights(sys.requireArrayOption("/flights"));

	QTime now = sys.getSimulatedTime();
	QTime cutoffTime = now.addSecs(loadCutoffTime);

	while (plan.size() != 0  &&  plan[0]->getScheduledDepartureTime() <= cutoffTime) {
		plan.removeFlight(plan[0]);
	}
}


void FlightSimulator::initFlight(Flight* flight)
{
	//flight->setBeginBoardingTime(flight->getScheduledDepartureTime().addSecs(-getBoardingTimeAverage()));

	// Initial boarding begin time (without event-based delays) is based on scheduled departure time and
	// the expected boarding time.
	int beginBoardingToSchedDepOffset = getBoardingTimeAverage() + (RandomFloat(0.0f, 1.0f) * beginBoardingTimeDeviation);
	flight->setBeginBoardingTime(flight->getScheduledDepartureTime().addSecs(-beginBoardingToSchedDepOffset));
}


void FlightSimulator::scheduleEvent(Event* evt, bool freshStart)
{
	System& sys = System::getInstance();
	double timeScale = sys.getSimulatedTimeScale();

	if (freshStart) {
		int freshTriggersMin = (*evt->jevt)["freshTriggersMin"].GetInt();
		int freshTriggersMax = (*evt->jevt)["freshTriggersMax"].GetInt();
		int freshTriggers = freshTriggersMin + (rand() % (freshTriggersMax-freshTriggersMin+1));
		printf("Fresh triggering %d\n", freshTriggers);

		for (int i = 0 ; i < freshTriggers ; i++) {
			triggerEvent(evt);
		}
	}

	int intervalMin = (*evt->jevt)["intervalMin"].GetInt();
	int intervalMax = (*evt->jevt)["intervalMax"].GetInt();
	int interval = intervalMin + (rand() % (intervalMax-intervalMin+1));
	evt->timer.setProperty("fidsEvtIdx", QVariant(events.indexOf(evt)));
	evt->timer.setSingleShot(true);
	evt->timer.setInterval(std::max((int) (interval*1000.0 / timeScale), 100));
	evt->timer.start();
}


void FlightSimulator::triggerEvent(Event* evt)
{
	System& sys = System::getInstance();

	QTime now = sys.getSimulatedTime();

	QList<Flight*> targetFlights;

	for (Flight* flight : plan) {
		QString sstatus;
		switch (flight->getStatus()) {
		case Flight::StatusScheduled:
			sstatus = "Scheduled";
			break;
		case Flight::StatusBoarding:
			sstatus = "Boarding";
			break;
		case Flight::StatusGateClosing:
			sstatus = "GateClosing";
			break;
		case Flight::StatusDeparted:
			sstatus = "Departed";
			break;
		case Flight::StatusCancelled:
			sstatus = "Cancelled";
			break;
		}

		sstatus = sstatus.toLower();

		bool valid = false;

		const rapidjson::Value& jstatus = (*evt->jevt)["status"];
		for (size_t i = 0 ; i < jstatus.Size() ; i++) {
			QString isstatus(jstatus[i].GetString());
			isstatus = isstatus.toLower();

			if (isstatus == sstatus) {
				valid = true;
				break;
			}
		}

		if (evt->jevt->HasMember("farAffectedTime")) {
			int farAffectedTime = (*evt->jevt)["farAffectedTime"].GetInt();

			int secsToExpDep = now.secsTo(flight->getExactExpectedDepartureTime());

			if (secsToExpDep > farAffectedTime) {
				valid = false;
			}
		}

		if (valid) {
			targetFlights << flight;
		}
	}

	if (targetFlights.empty()) {
		printf("No valid target flight for event\n");
		return;
	}


	int flightIdx = rand() % targetFlights.size();

	Flight* flight = targetFlights[flightIdx];


	QString type((*evt->jevt)["type"].GetString());

	if (type == "delay") {
		// Apply the event!

		if (type == "delay") {
			int delayMin = (*evt->jevt)["delayMin"].GetInt();
			int delayMax = (*evt->jevt)["delayMax"].GetInt();
			int delay = delayMin + (rand() % (delayMax-delayMin+1));

			printf("Delaying flight %s for %ds\n", flight->toString().toUtf8().constData(), delay);

			flight->delay(delay);
			plan.notifyFlightUpdated(flight);
		}
	}
}


void FlightSimulator::flightAdded(Flight* flight)
{
	initFlight(flight);
}


void FlightSimulator::simulatedDateTimeChanged(const QDateTime& now)
{
	//reloadPlan();
	Mode mode = this->mode;
	enterMode(ModeInvalid);
	enterMode(mode);
}


void FlightSimulator::eventOccurred()
{
	QTimer* t = (QTimer*) sender();
	Event* evt = events[t->property("fidsEvtIdx").toInt()];

	triggerEvent(evt);

	scheduleEvent(evt, false);
}


void FlightSimulator::statusUpdateTick()
{
	if (mode != ModeNormal) {
		return;
	}

	System& sys = System::getInstance();

	QTime now = sys.getSimulatedTime();

	//printf("*** Tick ***\n");

	for (size_t i = 0 ; i < plan.size() ; i++) {
		Flight* flight = plan[i];

		//printf("Delay: %lld\n", flight->getDelay());

		Flight::Status status = flight->getStatus();

		bool flightUpdated = false;

		if (status == Flight::StatusScheduled) {
			if (now >= flight->getBeginBoardingTime()) {
				// Begin boarding!

				flight->setStatus(Flight::StatusBoarding);

				/*// See how long boarding will take
				int64_t boardingTimespan = getBoardingTimeAverage()
						+ (int64_t) (RandomFloat(-1.0f, 1.0f) * getBoardingTimeDeviation());
				flight->setDepartureTime(flight->getBeginBoardingTime().addSecs(boardingTimespan));*/

				// Determine an initial time for when the gate will close
				int64_t gateCloseTimespan = getBoardingTimeAverage()
						+ (int64_t) (RandomFloat(0.0f, 1.0f) * getBoardingTimeDeviation())
						- getGateCloseTime();
				flight->setGateCloseTime(flight->getBeginBoardingTime().addSecs(gateCloseTimespan));

				flightUpdated = true;
			}
		}

		if (status == Flight::StatusBoarding) {
			if (now >= flight->getGateCloseTime()) {
				// Close the gates!

				// Determine an initial time for departure
				int64_t departTimespan = getGateCloseTime();
				flight->setDepartureTime(flight->getGateCloseTime().addSecs(departTimespan));

				flight->setStatus(Flight::StatusGateClosing);
				flightUpdated = true;
			}
		}

		if (status == Flight::StatusGateClosing) {
			if (now >= flight->getDepartureTime()) {
				// Depart!

				flight->setStatus(Flight::StatusDeparted);
				flightUpdated = true;
			}
		}

		if (status == Flight::StatusDeparted) {
			QTime removeTime = flight->getDepartureTime().addSecs(departLingerTime);

			if (now >= removeTime) {
				plan.removeFlight(flight);
			}
		}


		if (flightUpdated) {
			plan.notifyFlightUpdated(flight);
		}
	}
}


void FlightSimulator::cancelUpdateTick()
{
	if (mode != ModeCancelled) {
		return;
	}

	System& sys = System::getInstance();

	QList<Flight*> uncancelledFlights;

	int targetNum = sys.requireIntOption("/simulation/cancelTargetNum");

	size_t i = 0;
	for (Flight* flight : plan) {
		if (i >= targetNum) {
			break;
		}
		if (flight->getStatus() != Flight::StatusCancelled) {
			uncancelledFlights << flight;
		}
		i++;
	}

	if (uncancelledFlights.empty()  ||  cancelNumLeft <= 0) {
		cancelUpdateTimer.stop();
		cancelNumLeft = 0;
		return;
	}

	cancelNumLeft--;

	int idx = rand() % uncancelledFlights.size();

	Flight* flight = uncancelledFlights[idx];

	flight->setStatus(Flight::StatusCancelled);
	plan.notifyFlightUpdated(flight);
}


QTime FlightSimulator::roundExpectedDepartureTime(const QTime& time, const QTime& lastRoundedTime) const
{
	int64_t totalSecs = time.second() + time.minute()*60 + time.hour()*60*60;

	if (lastRoundedTime.isValid()) {
		int64_t lastTotalSecs = lastRoundedTime.second() + lastRoundedTime.minute()*60 + lastRoundedTime.hour()*60*60;

		if (abs(lastTotalSecs-totalSecs) <= expectedDepartureTimeHysteresis) {
			return lastRoundedTime;
		}
	}

	totalSecs = (totalSecs/expectedDepartureTimeModulo)*expectedDepartureTimeModulo;

	int secs = totalSecs%60;
	int mins = (totalSecs/60) % 60;
	int hours = (totalSecs/(60*60)) % 60;

	return QTime(hours, mins, secs);
}


void FlightSimulator::cue(const QString& cue)
{
	System& sys = System::getInstance();

	if (cue == "Test1"  ||  cue == "Test2") {
		int delay = 5;

		if (cue == "Test2") {
			delay = -delay;
		}

		for (size_t i = 0 ; i < plan.size() ; i++) {
			Flight* flight = plan[i];

			int64_t actualDelay = flight->delay(delay);
			plan.notifyFlightUpdated(flight);
		}
	} else if (cue == "EnterModePart1") {
		enterMode(ModeInvalid);
		enterModeDelayed(ModeNormal, 2000);
		if (sys.hasOption("/simulationTimeStart")) {
			sys.setSimulatedTime(QTime::fromString(sys.requireStringOption("/simulationTimeStart")));
		} else {
			sys.setSimulatedTime(QTime::currentTime());
		}
	} else if (cue == "EnterModeCancelled") {
		enterMode(ModeCancelled);
	} else if (cue == "EnterModePart2") {
		enterMode(ModeInvalid);
		enterModeDelayed(ModeNormal, 2000);
		if (sys.hasOption("/simulationTimePart2")) {
			sys.setSimulatedTime(QTime::fromString(sys.requireStringOption("/simulationTimePart2")));
		} else {
			sys.setSimulatedTime(QTime::currentTime());
		}
	}
}


