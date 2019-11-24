#ifndef FIDS_SYSTEM_H_
#define FIDS_SYSTEM_H_

#include <QObject>
#include <QDateTime>
#include <QTime>
#include <nxcommon/file/File.h>
#include <nxcommon/config/ConfigFile.h>
#include <nxcommon/config/ConfigFileDelegate.h>
#include <nxcommon/exception.h>


class System : public QObject, public ConfigFileDelegate
{
	Q_OBJECT

public:
	static System& getInstance();

public:
	void setDataDirectory(const File& dataDir);
	File getDataDirectory() const;

	void loadConfig();
	ConfigFile& getConfigFile();

	void unhandledException(Exception& ex);

	QDateTime getSimulatedDateTime() const;
	QTime getSimulatedTime() const { return getSimulatedDateTime().time(); }

	void setSimulatedDateTime(const QDateTime& simNow);
	void setSimulatedTime(const QTime& simNow);
	void offsetSimulatedTime(int numSecs);

	void setSimulatedTimeScale(double scale);
	double getSimulatedTimeScale() const { return simTimeScale; }

signals:
	void simulatedDateTimeChanged(const QDateTime& now);

protected:
	virtual const ConfigFile* getConfigFileDelegate() const;

private:
	System();

private:
	File dataDir;
	ConfigFile cfg;

	QDateTime simTimeSyncPointReal;
	QDateTime simTimeSyncPointSim;
	double simTimeScale;
};

#endif /* FIDS_SYSTEM_H_ */
