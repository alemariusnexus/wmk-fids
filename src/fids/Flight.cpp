#include "Flight.h"
#include "FlightSimulator.h"
#include "System.h"
#include <algorithm>



Flight::Flight (
		FlightPlan* plan,
		const QTime& schedDepTime, const QString& destination,
		const QString& flightNo, const QString& gate
)	: plan(plan), schedDepTime(schedDepTime), destination(destination), flightNo(flightNo), gate(gate),
	  status(StatusScheduled)
{
	init();
}


Flight::Flight(FlightPlan* plan, const rapidjson::Value& jsonData)
		: plan(plan), status(StatusScheduled)
{
	schedDepTime = QTime::fromString(QString::fromUtf8(jsonData[0].GetString(), jsonData[0].GetStringLength()));

	/*if (!jsonData[1].IsNull()) {
		expDepTime = QTime::fromString(QString::fromUtf8(jsonData[1].GetString(), jsonData[1].GetStringLength()));
	}*/

	destination = QString::fromUtf8(jsonData[1].GetString(), jsonData[1].GetStringLength());
	flightNo = QString::fromUtf8(jsonData[2].GetString(), jsonData[2].GetStringLength());
	gate = QString::fromUtf8(jsonData[3].GetString(), jsonData[3].GetStringLength());

	init();
}


void Flight::init()
{
	System& sys = System::getInstance();

	cancelledBlinkTime = sys.getIntOption("/blinkTimeCancelled", 0);
	boardingBlinkTime = sys.getIntOption("/blinkTimeBoarding", 0);
	gateClosingBlinkTime = sys.getIntOption("/blinkTimeGateClosing", 0);
	departedBlinkTime = sys.getIntOption("/blinkTimeDeparted", 0);
}


QTime Flight::getGateCloseTime() const
{
	return gateCloseTime;
	/*if (!depTime.isValid()) {
		return QTime();
	}

	FlightSimulator& sim = FlightSimulator::getInstance();

	return depTime.addSecs(-sim.getGateCloseTime());*/
}


void Flight::setGateCloseTime(const QTime& time)
{
	gateCloseTime = time;
	updateExpectedDepartureTime();
}


QTime Flight::getExpectedDepartureTime() const
{
	return expDepTime;
}


QTime Flight::getExactExpectedDepartureTime() const
{
	FlightSimulator& sim = FlightSimulator::getInstance();

	// Calculate exact expected departure time assuming average timing and no further
	// delays (but DO consider delays that have already been applied)

	QTime expDepTime;

	if (status == StatusScheduled) {
		expDepTime = getBeginBoardingTime().addSecs(sim.getBoardingTimeAverage());
	} else if (status == StatusBoarding) {
		expDepTime = getGateCloseTime().addSecs(sim.getGateCloseTime());
	} else if (status == StatusGateClosing  ||  status == StatusDeparted) {
		expDepTime = getDepartureTime();
	}

	return expDepTime;
}


int64_t Flight::getDelay() const
{
	QTime expDepTime = getExactExpectedDepartureTime();
	return schedDepTime.secsTo(expDepTime);
}


void Flight::updateExpectedDepartureTime()
{
	FlightSimulator& sim = FlightSimulator::getInstance();

	if (status == StatusDeparted) {
		expDepTime = QTime();
	}

	// Calculate exact expected departure time assuming average timing and no further
	// delays (but DO consider delays that have already been applied)

	QTime expDepTime;

	if (status == StatusScheduled) {
		expDepTime = getBeginBoardingTime().addSecs(sim.getBoardingTimeAverage());
	} else if (status == StatusBoarding) {
		expDepTime = getGateCloseTime().addSecs(sim.getGateCloseTime());
	} else if (status == StatusGateClosing) {
		expDepTime = getDepartureTime();
	}

	this->expDepTime = sim.roundExpectedDepartureTime(expDepTime, this->expDepTime);
}


void Flight::setBeginBoardingTime(const QTime& time)
{
	beginBoardingTime = time;
	updateExpectedDepartureTime();
}


QTime Flight::getDepartureTime() const
{
	return depTime;
}


void Flight::setDepartureTime(const QTime& time)
{
	depTime = time;
	updateExpectedDepartureTime();
}


int64_t Flight::delay(int64_t secs)
{
	FlightSimulator& sim = FlightSimulator::getInstance();

	int64_t curDelay = getDelay();

	secs = std::min(secs, std::max(sim.getMaximumDelay()-curDelay, (int64_t) 0));

	if (status == StatusScheduled) {
		// Begin boarding later
		//printf("Delaying %s   -   delaying boarding begin\n", toString().toUtf8().constData());
		setBeginBoardingTime(getBeginBoardingTime().addSecs(secs));
		return secs;
	} else if (status == StatusBoarding) {
		// Begin closing gate later
		//printf("Delaying %s   -   delaying gate close\n", toString().toUtf8().constData());
		setGateCloseTime(getGateCloseTime().addSecs(secs));
		return secs;
	} else if (status == StatusGateClosing) {
		// Depart later
		//printf("Delaying %s   -   delaying departure\n", toString().toUtf8().constData());
		setDepartureTime(getDepartureTime().addSecs(secs));
		return secs;
	}

	return 0;
}


QString Flight::getRemark() const
{
	if (status == StatusBoarding) {
		return "Boarding";
	} else if (status == StatusGateClosing) {
		return "Gate Closing";
	} else if (status == StatusCancelled) {
		return "Cancelled";
	} else if (status == StatusDeparted) {
		return "Departed";
	}

	return QString();
}


int Flight::getRemarkBlinkTime() const
{
	if (status == StatusCancelled) {
		return cancelledBlinkTime;
	} else if (status == StatusBoarding) {
		return boardingBlinkTime;
	} else if (status == StatusGateClosing) {
		return gateClosingBlinkTime;
	} else if (status == StatusDeparted) {
		return departedBlinkTime;
	}

	return 0;
}


QColor Flight::getRemarkColor() const
{
	System& sys = System::getInstance();

	if (status == StatusBoarding) {
		return QColor(sys.getStringOption("/remarkBoardingColor", "#00ff00"));
	} if (status == StatusGateClosing) {
		return QColor(sys.getStringOption("/remarkGateClosingColor", "#ff6000"));
	} if (status == StatusDeparted) {
		return QColor(sys.getStringOption("/remarkDepartedColor", "#ffffff"));
	} if (status == StatusCancelled) {
		return QColor(sys.getStringOption("/remarkCancelledColor", "#d92f49"));
	}

	return QColor();
}


/*void Flight::setExpectedDepartureTime(const QTime& time)
{
	expDepTime = time;
}


void Flight::setRemark(const QString& remark)
{
	this->remark = remark;
}


void Flight::setRemarkColor(const QColor& color)
{
	remarkColor = color;
}*/


void Flight::setStatus(Status status)
{
	this->status = status;
	updateExpectedDepartureTime();
}


QString Flight::toString() const
{
	return QString("%1 (%2)").arg(flightNo).arg(schedDepTime.toString("HH:mm"));
}


