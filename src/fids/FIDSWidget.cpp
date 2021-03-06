#include "FIDSWidget.h"
#include "System.h"
#include "FlightSimulator.h"
#include <QGraphicsOpacityEffect>
#include <QKeyEvent>
#include <QTimer>
#include <QSettings>
#include <QMovie>



FIDSWidget::FIDSWidget(QWidget* parent)
		: QWidget(parent), curWeatherMovie(nullptr), fadeType(FadeTypeInvalid)
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

	//QPixmap weatherIcon(File(sys.getDataDirectory(), "icons/weather.gif").toString());

	ui.titleWidget->setStyleSheet(sys.getStringOption("/titleCSS", ""));

	ui.titleLabel->setText(sys.requireStringOption("/titleText"));
	ui.titleLabel->setStyleSheet(sys.getStringOption("/titleLabelCSS", ""));

	ui.titleIcon->setPixmap(titleIcon);
	ui.titleIcon->setStyleSheet(sys.getStringOption("/titleIconCSS", ""));
	//ui.titleIcon->setStyleSheet(sys.getStringOption("/titleCSS", ""));

	ui.weatherLabel->setText("");
	ui.weatherLabel->setStyleSheet(sys.getStringOption("/weatherIconCSS", ""));

	setWeather(sys.requireStringOption("/weatherDefault"));

	ui.timeLabel->setStyleSheet(sys.getStringOption("/titleTimeCSS", ""));
	//ui.timeLabel->setStyleSheet(sys.getStringOption("/titleCSS", ""));

	ui.pauseLabel->setText(sys.requireStringOption("/pauseLabelText"));
	ui.pauseLabel->setStyleSheet(sys.getStringOption("/pauseLabelCSS", ""));

	ui.finishLabel->setText(sys.requireStringOption("/finishLabelText"));
	ui.finishLabel->setStyleSheet(sys.getStringOption("/finishLabelCSS", ""));

	autoResizeTable();

	QSettings settings;

	if (settings.contains("state/fidsWidgetGeometry")) {
		restoreGeometry(settings.value("state/fidsWidgetGeometry").toByteArray());
	}


	connect(&sys, SIGNAL(cueTriggered(const QString&)), this, SLOT(cueTriggered(const QString&)));

	fadeEffect = new QGraphicsOpacityEffect(ui.contentWidget);
	fadeEffect->setOpacity(1.0f);
	ui.contentStackedWidget->setGraphicsEffect(fadeEffect);
	ui.contentStackedWidget->setAutoFillBackground(true);


	sys.installGlobalShortcuts(this);


	tickTimer.setSingleShot(false);
	tickTimer.setInterval(10);
	connect(&tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
	tickTimer.start();

	fadeTimer.setSingleShot(false);
	fadeTimer.setInterval(50);
	connect(&fadeTimer, SIGNAL(timeout()), this, SLOT(fadeTick()));
	fadeTimer.start();
}


void FIDSWidget::cueTriggered(const QString& cue)
{
	System& sys = System::getInstance();

	if (cue.startsWith("FadeEnterMode")) {
		QString mode = cue.right(cue.length() - strlen("FadeEnterMode"));

		/*QGraphicsOpacityEffect* op = new QGraphicsOpacityEffect(ui.contentWidget);
		op->setOpacity(0.5f);
		ui.contentWidget->setGraphicsEffect(op);
		ui.contentWidget->setAutoFillBackground(true);*/

		const rapidjson::Value& jmode = sys.requireObjectOption(CString("/modes/").append(mode));

		if (jmode.HasMember("fadeTime")) {
			int fadeTime = jmode["fadeTime"].GetInt();
			int fadeMidTime = jmode.HasMember("fadeMidTime") ? jmode["fadeMidTime"].GetInt() : 0;
			cueFade(fadeTime, FadeTypeInOut, fadeMidTime);
		}
	} else if (cue.startsWith("EnterMode")) {
		QString mode = cue.right(cue.length() - strlen("EnterMode"));

		const rapidjson::Value& jmode = sys.requireObjectOption(CString("/modes/").append(mode));

		if (jmode.HasMember("weather")) {
			CString weather(jmode["weather"].GetString());

			setWeather(weather);
		}

		if (mode == "Finish") {
			ui.contentStackedWidget->setCurrentIndex(2);
		} else {
			if (ui.contentStackedWidget->currentIndex() == 2) {
				ui.contentStackedWidget->setCurrentIndex(0);
			}
		}
	} else if (cue == "FadeTogglePause") {
		int fadeTime = sys.getIntOption("/pauseFadeTime", 3000);
		int midTime = sys.getInt64Option("/pauseFadeMidTime", 1000);

		cueFade(fadeTime, FadeTypeInOut, midTime);

		sys.cueDelayed("TogglePause", (fadeTime-midTime)/2 + midTime/2);
	} else if (cue == "TogglePause") {
		if (ui.contentStackedWidget->currentIndex() == 0) {
			ui.contentStackedWidget->setCurrentIndex(1);
		} else {
			ui.contentStackedWidget->setCurrentIndex(0);
		}
	}

	/*if (cue == "EnterModePart1") {
		setWeather(sys.requireStringOption("/weatherDefault"));
	} if (cue == "EnterModePart2") {
		setWeather(sys.requireStringOption("/weatherPart2"));
	}*/
}


void FIDSWidget::setWeather(const QString& weather)
{
	System& sys = System::getInstance();

	QMovie* oldMovie = nullptr;
	if (curWeatherMovie) {
		oldMovie = curWeatherMovie;
	}

	const rapidjson::Value& jweather = sys.requireArrayOption("/weather");
	for (size_t i = 0 ; i < jweather.Size() ; i++) {
		const rapidjson::Value& jw = jweather[i];
		QString wid(jw["id"].GetString());

		if (wid == weather) {
			QMovie* movie = new QMovie(File(sys.getDataDirectory(), QString(jw["icon"].GetString())).toString());

			if (jw.HasMember("iconScale")) {
				QSize scale;
				if (jw["iconScale"].IsArray()) {
					scale = QSize(jw["iconScale"][0].GetInt(), jw["iconScale"][1].GetInt());
				} else {
					scale = QSize(jw["iconScale"].GetInt(), jw["iconScale"].GetInt());
				}
				movie->setScaledSize(scale);
			}

			ui.weatherLabel->setMovie(movie);
			curWeatherMovie = movie;
			movie->start();
		}
	}

	delete oldMovie;
}


void FIDSWidget::cueFade(uint64_t fadeTime, FadeType type, uint64_t fadeMidTime)
{
	fadeStartTime = GetTickcount();
	fadeTimeTotal = fadeTime;
	this->fadeMidTime = fadeMidTime;
	fadeType = type;
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
	QTime time = System::getInstance().getSimulatedTime();

	ui.timeLabel->setText(time.toString(curTimeFormat));
}


void FIDSWidget::fadeTick()
{
	float opacity = 1.0f;

	uint64_t now = GetTickcount();

	if (fadeType == FadeTypeInOut) {
		uint64_t fadeIntervalTime = (fadeTimeTotal-fadeMidTime) / 2;

		if (now <= fadeStartTime+fadeIntervalTime) {
			// Fade out
			opacity = 1.0f - (now - fadeStartTime) / (float) fadeIntervalTime;
		} else if (now <= fadeStartTime+fadeIntervalTime+fadeMidTime) {
			// Remain faded out
			opacity = 0.0f;
		} else if (now <= fadeStartTime+2*fadeIntervalTime+fadeMidTime) {
			// Fade in
			opacity = (now - (fadeStartTime+fadeTimeTotal/2)) / (float) (fadeTimeTotal/2);
		} else {
			// Fade done
			opacity = 1.0f;
			fadeType = FadeTypeInvalid;
		}
	}

	opacity = std::min(std::max(opacity, 0.0f), 1.0f);

	fadeEffect->setOpacity(opacity);
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
	System& sys = System::getInstance();

	/*if (sys.handleGlobalHotkeys(evt)) {
		evt->accept();
	} else {*/
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
	//}

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


