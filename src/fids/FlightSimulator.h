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
		ModeCancelled,
		ModeFreeze
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
	void enterModeDelayed(Mode mode, int delay);

	bool isFrozen() { return mode == ModeFreeze; }

private:
	FlightSimulator();

	void initFlight(Flight* flight);

	void scheduleEvent(Event* evt, bool freshStart = false);
	void triggerEvent(Event* evt);

	void clearPlan();
	void reloadPlan();

	//void testTime(const QString& stime, const QString& slastRoundedTime);

private slots:
	void flightAdded(Flight* flight);

	void cueTriggered(const QString& cue);

	void simulatedDateTimeChanged(const QDateTime& now);

	void delayedModeChangeTriggered();

	void eventOccurred();

	void statusUpdateTick();

	void cancelUpdateTick();

private:
	FlightPlan plan;

	Mode mode;
	Mode delayedMode;

	int cancelNumLeft;

	QTimer statusUpdateTimer;
	QTimer cancelUpdateTimer;
	QTimer delayedModeChangeTimer;

	int64_t beginBoardingTimeDeviation;

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
