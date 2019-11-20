#include <cstdlib>
#include <QWidget>
#include <QMessageBox>
#include <nxcommon/exception.h>
#include "MainApplication.h"
#include "System.h"
#include "FIDSWidget.h"
#include "FIDSControlWindow.h"


int main(int argc, char** argv)
{
	System& sys = System::getInstance();

	srand(time(nullptr));

	try {
		MainApplication app(argc, argv);

		app.setApplicationName("Terminal 2 FIDS - Wiesbadener Musik- & Kunstschule");

		FIDSControlWindow* fidsCtrlWindow = new FIDSControlWindow;
		fidsCtrlWindow->show();

		return app.exec();
	} catch (Exception& ex) {
		sys.unhandledException(ex);
		return 1;
	}
}


