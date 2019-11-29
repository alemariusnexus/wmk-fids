#include "System.h"
#include <QMessageBox>
#include <QShortcut>



System& System::getInstance()
{
	static System sys;
	return sys;
}



System::System()
{
	simTimeSyncPointReal = QDateTime::currentDateTime();
	simTimeSyncPointSim = simTimeSyncPointReal;
	simTimeScale = 1.0;
}


void System::setDataDirectory(const File& dataDir)
{
	this->dataDir = dataDir;
}


File System::getDataDirectory() const
{
	return dataDir;
}


void System::loadConfig()
{
	cfg.load(File(dataDir, "config.js"));
}


ConfigFile& System::getConfigFile()
{
	return cfg;
}


const ConfigFile* System::getConfigFileDelegate() const
{
	return &cfg;
}


void System::unhandledException(Exception& ex)
{
	fprintf(stderr, "ERROR: Exception caught at top level: %s\n", ex.what());

	QMessageBox::critical (
			nullptr,
			"Exception at top level",
			QString("ERROR: Exception caught at top level: %1").arg(QString(ex.what()))
			);
}


/*bool System::handleGlobalHotkeys(QKeyEvent* evt)
{

}*/


void System::installGlobalShortcuts(QWidget* parent)
{
	// Mode shortcuts

	const rapidjson::Value* jmodes = getObjectOption("/modes");
	if (jmodes) {
		for (auto it = jmodes->MemberBegin() ; it != jmodes->MemberEnd() ; it++) {
			CString mode = it->name.GetString();
			const rapidjson::Value& jmode = it->value;

			if (jmode.HasMember("shortcut")) {
				QString shortcutStr(jmode["shortcut"].GetString());

				QShortcut* shortcut = new QShortcut(QKeySequence(shortcutStr), parent);
				connect(shortcut, SIGNAL(activated()), this, SLOT(cueShortcutActivated()));
				shortcut->setObjectName(QString("EnterMode%1").arg(mode));
			}
		}
	}
}


void System::cueShortcutActivated()
{
	QShortcut* shortcut = (QShortcut*) sender();
	cue(shortcut->objectName());
}


void System::cue(const QString& cue)
{
	emit cueTriggered(cue);
}


QDateTime System::getSimulatedDateTime() const
{
	QDateTime nowReal = QDateTime::currentDateTime();
	int64_t offsetMsecs = simTimeSyncPointReal.msecsTo(nowReal);
	offsetMsecs = (int64_t) (simTimeScale * offsetMsecs);
	QDateTime nowSim = simTimeSyncPointSim.addMSecs(offsetMsecs);
	return nowSim;
}


void System::setSimulatedDateTime(const QDateTime& simNow)
{
	simTimeSyncPointReal = QDateTime::currentDateTime();
	simTimeSyncPointSim = simNow;
	emit simulatedDateTimeChanged(simNow);
}


void System::setSimulatedTime(const QTime& simNow)
{
	QDateTime simDNow = QDateTime::currentDateTime();
	simDNow.setTime(simNow);
	setSimulatedDateTime(simDNow);
}


void System::offsetSimulatedTime(int numSecs)
{
	setSimulatedDateTime(getSimulatedDateTime().addSecs(numSecs));
}


void System::setSimulatedTimeScale(double scale)
{
	simTimeScale = scale;
}
