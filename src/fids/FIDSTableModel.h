#ifndef FIDS_FIDSTABLEMODEL_H_
#define FIDS_FIDSTABLEMODEL_H_

#include <QAbstractTableModel>
#include <QList>
#include <QColor>
#include <QFont>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include "FlightPlan.h"
#include "Flight.h"


class FIDSTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	struct RowScheme
	{
		QColor bgColor;
		QColor textColor;
		QFont font;
	};

private:
	enum Column
	{
		ColMarginLeft = 0,
		ColScheduledDepartureTime = 1,
		ColExpectedDepartureTime = 2,
		ColDestination = 3,
		ColFlightNo = 4,
		ColGate = 5,
		ColRemarks = 6,
		ColMarginRight = 7,

		ColMAX = ColMarginRight
	};

	struct BlinkAnim
	{
		uint64_t startTickcount;
	};

public:
	FIDSTableModel(FlightPlan* plan);

	void setRowSchemes(const QList<RowScheme>& schemes);
	void setHeaderScheme(const RowScheme& scheme);
	void setHeaderTitles(const QList<QString> titles);
	void setTimeFormat(const QString& timeFormat);
	void setMaximumDisplayedFlights(int num);

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	void updateBlinkStatus(Flight* flight);

private slots:
	void flightUpdated(Flight* flight);
	void flightAboutToBeAdded(Flight* flight, size_t atIdx);
	void flightAdded(Flight* flight);
	void flightAboutToBeRemoved(Flight* flight);
	void flightRemoved(Flight* flight);

	void blinkTick();

private:
	FlightPlan* plan;
	QList<QString> headerTitles;
	QList<RowScheme> rowSchemes;
	RowScheme headerScheme;
	QString timeFormat;
	int maxDisplayedFlights;
	QTimer blinkTimer;
	QMap<Flight*, BlinkAnim> blinkAnims;
	bool blinkState;
};

#endif /* FIDS_FIDSTABLEMODEL_H_ */
