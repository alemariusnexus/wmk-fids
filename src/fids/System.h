#ifndef FIDS_SYSTEM_H_
#define FIDS_SYSTEM_H_

#include <nxcommon/file/File.h>
#include <nxcommon/config/ConfigFile.h>
#include <nxcommon/config/ConfigFileDelegate.h>
#include <nxcommon/exception.h>


class System : public ConfigFileDelegate
{
public:
	static System& getInstance();

public:
	void setDataDirectory(const File& dataDir);
	File getDataDirectory() const;

	void loadConfig();
	ConfigFile& getConfigFile();

	void unhandledException(Exception& ex);

protected:
	virtual const ConfigFile* getConfigFileDelegate() const;

private:
	System();

private:
	File dataDir;
	ConfigFile cfg;
};

#endif /* FIDS_SYSTEM_H_ */
