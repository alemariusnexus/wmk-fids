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



	setWindowTitle("Control - " + qApp->applicationName());

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


