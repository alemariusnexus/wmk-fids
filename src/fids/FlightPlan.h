#ifndef FIDS_FLIGHTPLAN_H_
#define FIDS_FLIGHTPLAN_H_

#include <QObject>
#include <QList>
#include <nxcommon/json.h>
#include "Flight.h"


class FlightPlan : public QObject
{
	Q_OBJECT

public:
	typedef QList<Flight*> FlightList;
	typedef FlightList::iterator FlightIterator;
	typedef FlightList::const_iterator ConstFlightIterator;

public:
	FlightPlan();
	virtual ~FlightPlan();

	//void addFlight(Flight* flight);
	Flight* addFlight(const QTime& schedDepTime, const QString& destination, const QString& flightNo, const QString& gate);
	void removeFlight(Flight* flight);
	void loadFlights(const rapidjson::Value& jsonData);
	void clear();

	void notifyFlightUpdated(Flight* flight);

	FlightIterator begin() { return flights.begin(); }
	ConstFlightIterator begin() const { return flights.cbegin(); }
	FlightIterator end() { return flights.end(); }
	ConstFlightIterator end() const { return flights.cend(); }
	size_t size() const { return flights.size(); }
	Flight* operator[](size_t idx) { return flights[idx]; }
	const Flight* operator[](size_t idx) const { return flights[idx]; }
	int indexOf(Flight* flight) const { return flights.indexOf(flight); }

signals:
	void flightUpdated(Flight* flight);
	void flightAboutToBeAdded(Flight* flight, size_t atIdx);
	void flightAdded(Flight* flight);
	void flightAboutToBeRemoved(Flight* flight);
	void flightRemoved(Flight* flight);

private:
	void addFlight(Flight* flight);

private:
	FlightList flights;
};

#endif /* FIDS_FLIGHTPLAN_H_ */
