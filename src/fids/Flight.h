#ifndef FIDS_FLIGHT_H_
#define FIDS_FLIGHT_H_

#include <cstdio>
#include <QObject>
#include <QString>
#include <QTime>
#include <QColor>
#include <nxcommon/json.h>

class FlightPlan;


class Flight : public QObject
{
	Q_OBJECT

	friend class FlightPlan;

public:
	enum Status
	{
		StatusScheduled,
		StatusBoarding,
		StatusGateClosing,
		StatusDeparted,

		StatusCancelled
	};

public:
	QTime getScheduledDepartureTime() const { return schedDepTime; }
	QTime getBeginBoardingTime() const { return beginBoardingTime; }
	QTime getExpectedDepartureTime() const;
	QTime getExactExpectedDepartureTime() const;
	int64_t getDelay() const;
	QString getDestination() const { return destination; }
	QString getFlightNumber() const { return flightNo; }
	QString getGate() const { return gate; }
	QString getRemark() const;
	QColor getRemarkColor() const;

	void setBeginBoardingTime(const QTime& time);

	void setGateCloseTime(const QTime& time);
	QTime getGateCloseTime() const;

	QTime getDepartureTime() const;
	void setDepartureTime(const QTime& time);

	int64_t delay(int64_t secs);

	//void setExpectedDepartureTime(const QTime& time);
	//void setRemark(const QString& remark);
	//void setRemarkColor(const QColor& color);

	void setStatus(Status status);
	Status getStatus() const { return status; }

	QString toString() const;

private:
	Flight(FlightPlan* plan, const QTime& schedDepTime, const QString& destination,
			const QString& flightNo, const QString& gate);
	Flight(FlightPlan* plan, const rapidjson::Value& jsonData);

	void updateExpectedDepartureTime();

private:
	FlightPlan* plan;

	QTime schedDepTime;
	//QTime expDepTime;
	QString destination;
	QString flightNo;
	QString gate;

	Status status;
	QTime beginBoardingTime;
	QTime gateCloseTime;
	QTime depTime;
	QTime expDepTime;

	//QString remark;
	//QColor remarkColor;
};

#endif /* FIDS_FLIGHT_H_ */
