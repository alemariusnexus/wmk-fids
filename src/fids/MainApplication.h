#ifndef FIDS_MAINAPPLICATION_H_
#define FIDS_MAINAPPLICATION_H_

#include <QApplication>


class MainApplication : public QApplication
{
	Q_OBJECT

public:
	MainApplication(int& argc, char** argv) : QApplication(argc, argv) {}
	virtual bool notify(QObject* receiver, QEvent* event);
};

#endif /* FIDS_MAINAPPLICATION_H_ */
