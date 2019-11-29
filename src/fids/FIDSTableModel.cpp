#include "FIDSTableModel.h"
#include "System.h"
#include "FlightSimulator.h"
#include <nxcommon/util.h>
#include <QColor>
#include <QBrush>



FIDSTableModel::FIDSTableModel(FlightPlan* plan)
		: plan(plan), timeFormat("HH:mm"), maxDisplayedFlights(-1), blinkState(true)
{
	System& sys = System::getInstance();

	connect(plan, SIGNAL(flightUpdated(Flight*)), this, SLOT(flightUpdated(Flight*)));
	connect(plan, SIGNAL(flightAboutToBeAdded(Flight*, size_t)), this, SLOT(flightAboutToBeAdded(Flight*, size_t)));
	connect(plan, SIGNAL(flightAdded(Flight*)), this, SLOT(flightAdded(Flight*)));
	connect(plan, SIGNAL(flightAboutToBeRemoved(Flight*)), this, SLOT(flightAboutToBeRemoved(Flight*)));
	connect(plan, SIGNAL(flightRemoved(Flight*)), this, SLOT(flightRemoved(Flight*)));

	int blinkInterval = sys.getIntOption("/blinkInterval", 1000);

	if (blinkInterval > 0) {
		connect(&blinkTimer, SIGNAL(timeout()), this, SLOT(blinkTick()));
		blinkTimer.setSingleShot(false);
		blinkTimer.setInterval(blinkInterval);
		blinkTimer.start();
	}
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
				return flight->getExpectedDepartureTime().toString(timeFormat);
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
			auto it = blinkAnims.find(flight);
			bool blinkStatus = true;

			if (it != blinkAnims.end()) {
				blinkStatus = this->blinkState;
			}

			if (blinkStatus) {
				return flight->getRemark().toUpper();
			} else {
				return QString("");
			}
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
	updateBlinkStatus(flight);
	emit dataChanged(createIndex(row, 0), createIndex(row, columnCount()));
}


void FIDSTableModel::flightAboutToBeAdded(Flight* flight, size_t atIdx)
{
	updateBlinkStatus(flight);
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


void FIDSTableModel::updateBlinkStatus(Flight* flight)
{
	int blinkTime = flight->getRemarkBlinkTime();

	bool updated = false;

	if (blinkTime > 0) {
		if (!blinkAnims.contains(flight)) {
			// Start blink animation

			BlinkAnim anim;
			anim.startTickcount = GetTickcount();
			blinkAnims.insert(flight, anim);

			updated = true;
		}
	} else {
		if (blinkAnims.contains(flight)) {
			// Stop blink animation

			blinkAnims.remove(flight);

			updated = true;
		}
	}
}


void FIDSTableModel::blinkTick()
{
	if (FlightSimulator::getInstance().isFrozen()) {
		return;
	}

	uint64_t nowTc = GetTickcount();

	QList<Flight*> endedBlinks;

	blinkState = !blinkState;

	for (auto it = blinkAnims.begin() ; it != blinkAnims.end() ; it++) {
		Flight* flight = it.key();
		BlinkAnim& anim = it.value();

		uint64_t t = nowTc - anim.startTickcount;

		if (t > flight->getRemarkBlinkTime()) {
			endedBlinks << flight;
			continue;
		}

		int row = plan->indexOf(flight);

		if (row >= 0) {
			emit dataChanged(createIndex(row, ColRemarks), createIndex(row, ColRemarks));
		}

		/*bool newState = (t / flight->getRemarkBlinkInterval()) % 2 == 0;

		if (anim.curState != newState) {
			anim.curState = newState;

			int row = plan->indexOf(flight);

			if (row >= 0) {
				emit dataChanged(createIndex(row, ColRemarks), createIndex(row, ColRemarks));
			}
		}*/
	}

	for (Flight* flight : endedBlinks) {
		blinkAnims.remove(flight);

		int row = plan->indexOf(flight);
		emit dataChanged(createIndex(row, ColRemarks), createIndex(row, ColRemarks));
	}
}




