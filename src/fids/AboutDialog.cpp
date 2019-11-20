#include "AboutDialog.h"
#include <QApplication>


AboutDialog::AboutDialog(QWidget* parent)
		: QDialog(parent)
{
	ui.setupUi(this);

	setWindowTitle("About - " + qApp->applicationName());
	ui.appNameLabel->setText(qApp->applicationName());

	adjustSize();
}
