#ifndef FIDS_FLIGHTSIMULATOR_H_
#define FIDS_FLIGHTSIMULATOR_H_

#include <QObject>
#include <QTimer>
#include <QList>
#include "Flight.h"
#include "FlightPlan.h"


class FlightSimulator : public QObject
{
	Q_OBJECT

private:
	struct Event
	{
		const rapidjson::Value* jevt;
		QTimer timer;
	};

	enum Mode
	{
		ModeInvalid,

		ModeNormal,
		ModeCancelled
	};

public:
	static FlightSimulator& getInstance();

public:
	void start();

	FlightPlan* getFlightPlan() { return &plan; }

	int64_t getBoardingTimeAverage() const { return boardingTimeAverage; }
	int64_t getBoardingTimeDeviation() const { return boardingTimeDeviation; }

	int64_t getGateCloseTime() const { return gateCloseTime; }

	int64_t getMaximumDelay() const { return delayMax; }

	QTime roundExpectedDepartureTime(const QTime& time, const QTime& lastRoundedTime) const;

	void enterMode(Mode mode);

	void cue(const QString& cue);

private:
	FlightSimulator();

	void initFlight(Flight* flight);

	void scheduleEvent(Event* evt);

	void reloadPlan();

	//void testTime(const QString& stime, const QString& slastRoundedTime);

private slots:
	void flightAdded(Flight* flight);

	void eventOccurred();

	void statusUpdateTick();

	void cancelUpdateTick();

private:
	FlightPlan plan;

	Mode mode;

	QTimer statusUpdateTimer;
	QTimer cancelUpdateTimer;

	int64_t boardingTimeAverage;
	int64_t boardingTimeDeviation;

	int64_t gateCloseTime;

	int64_t expectedDepartureTimeModulo;
	int64_t expectedDepartureTimeHysteresis;

	int64_t departLingerTime;

	int64_t delayMax;

	QList<Event*> events;
};

#endif /* FIDS_FLIGHTSIMULATOR_H_ */
