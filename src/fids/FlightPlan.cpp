#include "FlightPlan.h"
#include "System.h"



FlightPlan::FlightPlan()
{
}


FlightPlan::~FlightPlan()
{
	for (Flight* f : flights) {
		delete f;
	}
}


Flight* FlightPlan::addFlight(const QTime& schedDepTime, const QString& destination, const QString& flightNo, const QString& gate)
{
	Flight* flight = new Flight(this, schedDepTime, destination, flightNo, gate);
	addFlight(flight);
	return flight;
}


void FlightPlan::addFlight(Flight* flight)
{
	FlightIterator it;
	for (it = flights.begin() ; it != flights.end()  &&  (*it)->getScheduledDepartureTime() <= flight->getScheduledDepartureTime() ; it++);
	size_t atIdx = it - flights.begin();
	emit flightAboutToBeAdded(flight, atIdx);
	flights.insert(it, flight);

	emit flightAdded(flight);
}


void FlightPlan::removeFlight(Flight* flight)
{
	emit flightAboutToBeRemoved(flight);
	flights.removeOne(flight);
	emit flightRemoved(flight);
}


void FlightPlan::clear()
{
	while (!flights.empty()) {
		removeFlight(flights[0]);
	}
}


void FlightPlan::loadFlights(const rapidjson::Value& jsonData)
{
	for (size_t i = 0 ; i < jsonData.Size() ; i++) {
		Flight* f = new Flight(this, jsonData[i]);
		addFlight(f);
	}
}


void FlightPlan::notifyFlightUpdated(Flight* flight)
{
	emit flightUpdated(flight);
}
