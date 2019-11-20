#include "FIDSTableModel.h"
#include "System.h"
#include <QColor>
#include <QBrush>



FIDSTableModel::FIDSTableModel(FlightPlan* plan)
		: plan(plan), timeFormat("HH:mm"), maxDisplayedFlights(-1)
{
	connect(plan, SIGNAL(flightUpdated(Flight*)), this, SLOT(flightUpdated(Flight*)));
	connect(plan, SIGNAL(flightAboutToBeAdded(Flight*, size_t)), this, SLOT(flightAboutToBeAdded(Flight*, size_t)));
	connect(plan, SIGNAL(flightAdded(Flight*)), this, SLOT(flightAdded(Flight*)));
	connect(plan, SIGNAL(flightAboutToBeRemoved(Flight*)), this, SLOT(flightAboutToBeRemoved(Flight*)));
	connect(plan, SIGNAL(flightRemoved(Flight*)), this, SLOT(flightRemoved(Flight*)));
}


void FIDSTableModel::setRowSchemes(const QList<RowScheme>& schemes)
{
	rowSchemes = schemes;
}


void FIDSTableModel::setHeaderScheme(const RowScheme& scheme)
{
	headerScheme = scheme;
}


void FIDSTableModel::setHeaderTitles(const QList<QString> titles)
{
	headerTitles = titles;
}


void FIDSTableModel::setTimeFormat(const QString& timeFormat)
{
	this->timeFormat = timeFormat;
}


void FIDSTableModel::setMaximumDisplayedFlights(int num)
{
	maxDisplayedFlights = num;
}


int FIDSTableModel::rowCount(const QModelIndex& parent) const
{
	if (maxDisplayedFlights >= 0) {
		if (plan->size() > maxDisplayedFlights) {
			return maxDisplayedFlights;
		}
	}
	return plan->size();
}


int FIDSTableModel::columnCount(const QModelIndex& parent) const
{
	return ColMAX+1;
}


QVariant FIDSTableModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	System& sys = System::getInstance();

	int row = index.row();

	if (row >= plan->size()) {
		return QVariant();
	}

	Flight* flight = (*plan)[row];

	RowScheme scheme = rowSchemes[row % rowSchemes.size()];

	int col = index.column();

	/*if (col == ColMarginLeft  ||  col == ColMarginRight) {
		return QVariant();
	}*/

	switch (col) {
	case ColScheduledDepartureTime:
		if (role == Qt::DisplayRole) {
			return flight->getScheduledDepartureTime().toString(timeFormat);
		}
		break;
	case ColExpectedDepartureTime:
		if (role == Qt::DisplayRole) {
			QTime et = flight->getExactExpectedDepartureTime();
			if (!et.isNull()) {
				int threshold = sys.getIntOption("/expectedDepartureDisplayThreshold", 300);
				if (abs(et.secsTo(flight->getScheduledDepartureTime())) < threshold) {
					return QString("");
				}
				return et.toString(timeFormat);
			} else {
				return QString("");
			}
		}
		break;
	case ColDestination:
		if (role == Qt::DisplayRole) {
			return flight->getDestination().toUpper();
		}
		break;
	case ColFlightNo:
		if (role == Qt::DisplayRole) {
			return flight->getFlightNumber();
		}
		break;
	case ColGate:
		if (role == Qt::DisplayRole) {
			return flight->getGate();
		}
		break;
	case ColRemarks:
		if (role == Qt::DisplayRole) {
			return flight->getRemark().toUpper();
		} else if (role == Qt::ForegroundRole) {
			if (flight->getRemarkColor().isValid()) {
				return QBrush(flight->getRemarkColor());
			}
		}
		break;
	}

	if (role == Qt::BackgroundRole) {
		return QBrush(scheme.bgColor);
	} else if (role == Qt::ForegroundRole) {
		return QBrush(scheme.textColor);
	} else if (role == Qt::FontRole) {
		return scheme.font;
	}

	return QVariant();
}


QVariant FIDSTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical) {
		return QVariant();
	}

	if (section == ColMarginLeft  ||  section == ColMarginRight) {
		return QVariant();
	}

	// Skip ColMarginLeft
	section--;

	if (role == Qt::TextAlignmentRole) {
		return Qt::AlignLeft;
	}

	if (role == Qt::DisplayRole) {
		if (section >= headerTitles.size()) {
			return QVariant();
		}

		return headerTitles[section];
	}

	return QVariant();
}


void FIDSTableModel::flightUpdated(Flight* flight)
{
	int row = plan->indexOf(flight);
	emit dataChanged(createIndex(row, 0), createIndex(row, columnCount()));
}


void FIDSTableModel::flightAboutToBeAdded(Flight* flight, size_t atIdx)
{
	beginInsertRows(QModelIndex(), atIdx, atIdx);
}


void FIDSTableModel::flightAdded(Flight* flight)
{
	endInsertRows();
}


void FIDSTableModel::flightAboutToBeRemoved(Flight* flight)
{
	int idx = plan->indexOf(flight);
	beginRemoveRows(QModelIndex(), idx, idx);
}


void FIDSTableModel::flightRemoved(Flight* flight)
{
	endRemoveRows();
}




