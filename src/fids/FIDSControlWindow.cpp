#include <fids/AboutDialog.h>
#include "FIDSControlWindow.h"
#include "FlightSimulator.h"
#include "System.h"
#include <QTimer>
#include <QSettings>
#include <QRegularExpression>
#include <nxcommon/file/File.h>



FIDSControlWindow::FIDSControlWindow(QWidget* parent)
		: QMainWindow(parent), fidsWidget(nullptr)
{
	ui.setupUi(this);

	connect(ui.aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));

	QTimer::singleShot(0, this, SLOT(init()));
}


FIDSControlWindow::~FIDSControlWindow()
{
	disconnect(fidsWidget, SIGNAL(closed()), this, SLOT(fidsWidgetClosed()));
	delete fidsWidget;
}


void FIDSControlWindow::init()
{
	System& sys = System::getInstance();

	File dataRoot;

	// Try to find data directory under current working directory
	if (dataRoot.isNull()) {
		if (File("data/config.js").exists()) {
			dataRoot = File("data").getAbsoluteFile();
		}
	}

	// Try to find data directory under executable directory
	if (dataRoot.isNull()) {
		File exeFile = File::getExecutableFile();
		File exeDir = exeFile.getParent();

		if (File(exeDir, "data/config.js").exists()) {
			dataRoot = File(exeDir, "data").getAbsoluteFile();
		} else if (File(exeDir.getParent(), "data/config.js").exists()) {
			dataRoot = File(exeDir.getParent(), "data").getAbsoluteFile();
		}
	}

	if (!dataRoot.isDirectory()  ||  !File(dataRoot, "config.js").exists()) {
		fprintf(stderr, "ERROR: Data directory not found or invalid!\n");
		exit(1);
	}

	sys.setDataDirectory(dataRoot);

	sys.loadConfig();


	if (sys.hasOption("/simulationTimeStart")) {
		QTime simNow = QTime::fromString(sys.requireStringOption("/simulationTimeStart"));
		sys.setSimulatedTime(simNow);
	}
	if (sys.hasOption("/simulationTimeScale")) {
		double scale = sys.requireDoubleOption("/simulationTimeScale");
		sys.setSimulatedTimeScale(scale);
	}


	setWindowTitle("Control - " + qApp->applicationName());


	const rapidjson::Value* jsimTimePresets = sys.getArrayOption("/simulationTimePresets");
	if (jsimTimePresets  &&  jsimTimePresets->Size() > 0) {
		for (size_t i = 0 ; i < jsimTimePresets->Size() ; i++) {
			const rapidjson::Value& jpreset = (*jsimTimePresets)[i];
			QString name(jpreset["name"].GetString());
			QTime time = QTime::fromString(jpreset["time"].GetString());

			if (i != 0) {
				ui.timePresetsBox->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
			}

			QPushButton* cueButton = new QPushButton(ui.timePresetsBox);
			cueButton->setText(QString("Time \"%1 (%2)\"").arg(name).arg(time.toString("HH:mm")));
			cueButton->setObjectName(QString("cueButtonSetTimePreset%1").arg(i));
			ui.timePresetsBox->layout()->addWidget(cueButton);
		}
	} else {
		ui.timePresetsBox->hide();
	}


	QSettings settings;

	if (settings.contains("state/fidsControlWindowGeometry")) {
		restoreGeometry(settings.value("state/fidsControlWindowGeometry").toByteArray());
	}

	fidsWidget = new FIDSWidget;
	connect(fidsWidget, SIGNAL(closed()), this, SLOT(fidsWidgetClosed()));
	//fidsWidget->showFullScreen();
	fidsWidget->show();

	QRegularExpression cueButtonRegex("^cueButton.*$");
	QList<QPushButton*> cueButtons = findChildren<QPushButton*>(cueButtonRegex);

	for (QPushButton* btn : cueButtons) {
		connect(btn, SIGNAL(clicked()), this, SLOT(cueButtonClicked()));
	}


	FlightSimulator& sim = FlightSimulator::getInstance();
	sim.start();
}


void FIDSControlWindow::cueTriggered(const QString& cue)
{
	System& sys = System::getInstance();

	if (cue.startsWith("SetTimePreset")) {
		int presetIdx = cue.right(cue.length() - strlen("SetTimePreset")).toInt();

		const rapidjson::Value* jsimTimePresets = sys.getArrayOption("/simulationTimePresets");
		const rapidjson::Value& jpreset = (*jsimTimePresets)[presetIdx];

		QString name(jpreset["name"].GetString());
		QTime time = QTime::fromString(jpreset["time"].GetString());

		printf("Setting simulated time to %s\n", time.toString("HH:mm").toUtf8().constData());

		sys.setSimulatedTime(time);
	}

	FlightSimulator::getInstance().cue(cue);
	if (fidsWidget) {
		fidsWidget->cue(cue);
	}
}


void FIDSControlWindow::cueButtonClicked()
{
	QString cueName = sender()->objectName().right(sender()->objectName().length() - strlen("cueButton"));

	cueTriggered(cueName);
}


void FIDSControlWindow::closeEvent(QCloseEvent* evt)
{
	fidsWidget->close();

	QSettings settings;

	settings.setValue("state/fidsControlWindowGeometry", saveGeometry());

	QMainWindow::closeEvent(evt);
}


void FIDSControlWindow::fidsWidgetClosed()
{
	close();
}


void FIDSControlWindow::onAbout()
{
	AboutDialog* aw = new AboutDialog;
	aw->setAttribute(Qt::WA_DeleteOnClose);
	aw->exec();
}


