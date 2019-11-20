#include "MainApplication.h"
#include <nxcommon/exception/Exception.h>
#include "System.h"



bool MainApplication::notify(QObject* receiver, QEvent* event)
{
	try {
		return QApplication::notify(receiver, event);
	} catch (Exception& ex) {
		System& sys = System::getInstance();
		sys.unhandledException(ex);
		return false;
	}
}

