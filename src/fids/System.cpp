#include "System.h"
#include <QMessageBox>



System& System::getInstance()
{
	static System sys;
	return sys;
}



System::System()
{
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
