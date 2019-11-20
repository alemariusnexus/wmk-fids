#include "FIDSWidget.h"
#include "System.h"
#include "FlightSimulator.h"
#include <QKeyEvent>
#include <QTimer>
#include <QSettings>



FIDSWidget::FIDSWidget(QWidget* parent)
		: QWidget(parent)
{
	ui.setupUi(this);

	QTimer::singleShot(0, this, SLOT(init()));
}


FIDSWidget::~FIDSWidget()
{
	delete fidsModel;
}


void FIDSWidget::init()
{
	System& sys = System::getInstance();
	FlightSimulator& sim = FlightSimulator::getInstance();


	setWindowTitle("Display - " + qApp->applicationName());

	curTimeFormat = sys.getStringOption("/curTimeFormat", "HH:mm");


	setStyleSheet(sys.getStringOption("/globalCSS", ""));

	ui.fidsTable->setWordWrap(false);

	ui.fidsTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.fidsTable->horizontalHeader()->setFixedHeight(sys.requireIntOption("/headerHeight"));

	ui.fidsTable->setFocusPolicy(Qt::NoFocus);
	ui.fidsTable->horizontalHeader()->setStyleSheet(sys.getStringOption("/tableHeaderCSS", ""));
	ui.fidsTable->setStyleSheet(sys.getStringOption("/tableCSS", ""));

	// TODO: Here

	fidsModel = new FIDSTableModel(sim.getFlightPlan());
	fidsModel->setMaximumDisplayedFlights(sys.getIntOption("/maxDisplayedFlights", -1));
	fidsModel->setTimeFormat(sys.getStringOption("/fidsTimeFormat", "HH:mm"));

	QList<QString> headerTitles;
	const rapidjson::Value& jsonHeaderTitles = sys.requireArrayOption("/headerTitles");
	for (size_t i = 0 ; i < jsonHeaderTitles.Size() ; i++) {
		headerTitles << QString(jsonHeaderTitles[i].GetString());
	}
	fidsModel->setHeaderTitles(headerTitles);

	QList<FIDSTableModel::RowScheme> rowSchemes;
	const rapidjson::Value& jsonRowSchemes = sys.requireArrayOption("/rowSchemes");
	for (size_t i = 0 ; i < jsonRowSchemes.Size() ; i++) {
		const rapidjson::Value& jscheme = jsonRowSchemes[i];
		rowSchemes << loadRowScheme(jscheme);
	}
	fidsModel->setRowSchemes(rowSchemes);

	//fidsModel->setHeaderScheme(loadRowScheme(sys.requireObjectOption("/headerScheme")));

	const rapidjson::Value& jsonColumnSizes = sys.requireArrayOption("/columnSizes");
	float totalSize = 0.0f;
	for (size_t i = 0 ; i < jsonColumnSizes.Size() ; i++) {
		float relSize = jsonColumnSizes[i].GetFloat();
		columnSizes << relSize;
		totalSize += relSize;
	}

	for (size_t i = 0 ; i < columnSizes.size() ; i++) {
		columnSizes[i] /= totalSize;
	}

	ui.fidsTable->setModel(fidsModel);

	QPixmap titleIcon(File(sys.getDataDirectory(), "icons/departure.png").toString());
	if (sys.hasOption("/titleIconScale")) {
		int scale = sys.requireIntOption("/titleIconScale");
		titleIcon = titleIcon.scaled(scale, scale, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	ui.titleWidget->setStyleSheet(sys.getStringOption("/titleCSS", ""));

	ui.titleLabel->setText(sys.requireStringOption("/titleText"));
	ui.titleLabel->setStyleSheet(sys.getStringOption("/titleLabelCSS", ""));

	ui.titleIcon->setPixmap(titleIcon);
	ui.titleIcon->setStyleSheet(sys.getStringOption("/titleIconCSS", ""));
	//ui.titleIcon->setStyleSheet(sys.getStringOption("/titleCSS", ""));

	ui.timeLabel->setStyleSheet(sys.getStringOption("/titleTimeCSS", ""));
	//ui.timeLabel->setStyleSheet(sys.getStringOption("/titleCSS", ""));

	autoResizeTable();

	QSettings settings;

	if (settings.contains("state/fidsWidgetGeometry")) {
		restoreGeometry(settings.value("state/fidsWidgetGeometry").toByteArray());
	}


	tickTimer.setSingleShot(false);
	tickTimer.setInterval(10);
	connect(&tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
	tickTimer.start();
}


void FIDSWidget::cue(const QString& cue)
{
	if (cue == "Test1") {
		/*for (size_t i = 0 ; i < plan->size() ; i++) {
			if (i%2 == 1) {
				Flight* flight = (*plan)[i];
				flight->setRemark("CANCELLED");
				flight->setRemarkColor(QColor("#d92f49"));
				plan->notifyFlightUpdated(flight);
			}
		}*/
		printf("Rows: %d\n", fidsModel->rowCount(QModelIndex()));
	}
}


void FIDSWidget::autoResizeTable()
{
	System& sys = System::getInstance();

	int w = ui.fidsTable->width();

	int padLeft = sys.getIntOption("/tablePaddingLeft", 0);
	int padRight = sys.getIntOption("/tablePaddingRight", 0);

	w -= padLeft+padRight;

	ui.fidsTable->setColumnWidth(0, padLeft);

	if (!columnSizes.empty()) {
		for (size_t i = 0 ; i < columnSizes.size() ; i++) {
			ui.fidsTable->setColumnWidth(i + 1, (int) (columnSizes[i] * w));
		}
	}

	ui.fidsTable->setColumnWidth(columnSizes.size()+1, padRight);
}


void FIDSWidget::tick()
{
	QTime time = QTime::currentTime();

	ui.timeLabel->setText(time.toString(curTimeFormat));
}


FIDSTableModel::RowScheme FIDSWidget::loadRowScheme(const rapidjson::Value& jscheme) const
{
	FIDSTableModel::RowScheme scheme;

	scheme.bgColor = QColor(QString::fromUtf8(jscheme["backgroundColor"].GetString()));
	scheme.textColor = QColor(QString::fromUtf8(jscheme["textColor"].GetString()));

	QFont font;

	if (jscheme.HasMember("font")) {
		const rapidjson::Value& jfont = jscheme["font"];
		if (jfont.HasMember("family")) {
			font.setFamily(QString(jfont["family"].GetString()));
		}
		if (jfont.HasMember("size")) {
			font.setPointSize(jfont["size"].GetInt());
		}
	}
	scheme.font = font;

	return scheme;
}


void FIDSWidget::keyReleaseEvent(QKeyEvent* evt)
{
	if (evt->key() == Qt::Key_Escape) {
		evt->accept();
		close();
		return;
	} else if (evt->key() == Qt::Key_Enter  ||  evt->key() == Qt::Key_Return) {
		if (isFullScreen()) {
			showNormal();
		} else {
			showFullScreen();
		}
	}

	QWidget::keyReleaseEvent(evt);
}


void FIDSWidget::resizeEvent(QResizeEvent* evt)
{
	autoResizeTable();

	QWidget::resizeEvent(evt);
}


void FIDSWidget::closeEvent(QCloseEvent* evt)
{
	QSettings settings;

	settings.setValue("state/fidsWidgetGeometry", saveGeometry());

	QWidget::closeEvent(evt);

	emit closed();
}


