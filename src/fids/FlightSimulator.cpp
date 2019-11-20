#include "FlightSimulator.h"
#include "System.h"
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

	statusUpdateTimer.setSingleShot(false);
	statusUpdateTimer.setInterval(sys.getIntOption("/simulation/updateInterval", 1) * 1000);
	connect(&statusUpdateTimer, SIGNAL(timeout()), this, SLOT(statusUpdateTick()));

	cancelUpdateTimer.setSingleShot(false);
	cancelUpdateTimer.setInterval(sys.getIntOption("/simulation/cancelInterval", 1000));
	connect(&cancelUpdateTimer, SIGNAL(timeout()), this, SLOT(cancelUpdateTick()));

	enterMode(ModeNormal);
}


void FlightSimulator::enterMode(Mode mode)
{
	if (this->mode == mode) {
		return;
	}

	Mode oldMode = this->mode;
	this->mode = mode;

	if (mode == ModeNormal) {
		reloadPlan();

		cancelUpdateTimer.stop();
		statusUpdateTimer.start();

		for (Event* evt : events) {
			scheduleEvent(evt);
		}
	} else if (mode == ModeCancelled) {
		for (Event* evt : events) {
			evt->timer.stop();
		}

		statusUpdateTimer.stop();
		cancelUpdateTimer.start();
	}
}


void FlightSimulator::reloadPlan()
{
	System& sys = System::getInstance();

	int64_t loadCutoffTime = sys.getInt64Option("/simulation/loadCutoffTime", 300);

	plan.clear();

	plan.loadFlights(sys.requireArrayOption("/flights"));

	QTime now = QTime::currentTime();
	QTime cutoffTime = now.addSecs(loadCutoffTime);

	while (plan.size() != 0  &&  plan[0]->getScheduledDepartureTime() <= cutoffTime) {
		plan.removeFlight(plan[0]);
	}
}


void FlightSimulator::initFlight(Flight* flight)
{
	flight->setBeginBoardingTime(flight->getScheduledDepartureTime().addSecs(-getBoardingTimeAverage()));
}


void FlightSimulator::scheduleEvent(Event* evt)
{
	int intervalMin = (*evt->jevt)["intervalMin"].GetInt();
	int intervalMax = (*evt->jevt)["intervalMax"].GetInt();
	int interval = intervalMin + (rand() % (intervalMax-intervalMin+1));
	evt->timer.setProperty("fidsEvtIdx", QVariant(events.indexOf(evt)));
	evt->timer.setSingleShot(true);
	evt->timer.setInterval(interval*1000);
	evt->timer.start();
}


void FlightSimulator::flightAdded(Flight* flight)
{
	initFlight(flight);
}


void FlightSimulator::eventOccurred()
{
	QTimer* t = (QTimer*) sender();
	Event* evt = events[t->property("fidsEvtIdx").toInt()];


	QString type((*evt->jevt)["type"].GetString());

	if (type == "delay") {
		int flightIdx = rand() % plan.size();

		Flight* flight = plan[flightIdx];

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

		if (valid) {
			// Apply the event!

			if (type == "delay") {
				int delayMin = (*evt->jevt)["delayMin"].GetInt();
				int delayMax = (*evt->jevt)["delayMax"].GetInt();
				int delay = delayMin + (rand() % (delayMax-delayMin+1));

				//printf("Delaying flight %s for %ds\n", flight->toString().toUtf8().constData(), delay);

				flight->delay(delay);
				plan.notifyFlightUpdated(flight);
			}
		}
	}


	scheduleEvent(evt);
}


void FlightSimulator::statusUpdateTick()
{
	if (mode != ModeNormal) {
		return;
	}

	QTime now = QTime::currentTime();

	//printf("*** Tick ***\n");

	for (size_t i = 0 ; i < plan.size() ; i++) {
		Flight* flight = plan[i];

		//printf("Delay: %lld\n", flight->getDelay());

		Flight::Status status = flight->getStatus();

		bool flightUpdated = false;

		if (status == Flight::StatusScheduled) {
			if (now >= flight->getBeginBoardingTime()) {
				// Begin boarding!

				printf("Begin boarding\n");

				flight->setStatus(Flight::StatusBoarding);

				/*// See how long boarding will take
				int64_t boardingTimespan = getBoardingTimeAverage()
						+ (int64_t) (RandomFloat(-1.0f, 1.0f) * getBoardingTimeDeviation());
				flight->setDepartureTime(flight->getBeginBoardingTime().addSecs(boardingTimespan));*/

				// Determine an initial time for when the gate will close
				int64_t gateCloseTimespan = getBoardingTimeAverage()
						+ (int64_t) (RandomFloat(-1.0f, 1.0f) * getBoardingTimeDeviation())
						- getGateCloseTime();
				flight->setGateCloseTime(flight->getBeginBoardingTime().addSecs(gateCloseTimespan));

				flightUpdated = true;
			}
		}

		if (status == Flight::StatusBoarding) {
			if (now >= flight->getGateCloseTime()) {
				// Close the gates!

				printf("Close gate\n");

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

				printf("Depart\n");

				flight->setStatus(Flight::StatusDeparted);
				flightUpdated = true;
			}
		}

		if (status == Flight::StatusDeparted) {
			QTime removeTime = flight->getDepartureTime().addSecs(departLingerTime);

			if (now >= removeTime) {
				plan.removeFlight(flight);
				printf("Remove\n");
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

	QList<Flight*> uncancelledFlights;

	for (Flight* flight : plan) {
		if (flight->getStatus() != Flight::StatusCancelled) {
			uncancelledFlights << flight;
		}
	}

	if (uncancelledFlights.empty()) {
		cancelUpdateTimer.stop();
		return;
	}

	//int idx = rand() % uncancelledFlights.size();
	int idx = 0;

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
	} else if (cue == "EnterModeCancelled") {
		enterMode(ModeCancelled);
	} else if (cue == "EnterModeNormal") {
		enterMode(ModeNormal);
	}
}


