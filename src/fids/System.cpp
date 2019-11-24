#include "System.h"
#include <QMessageBox>



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
