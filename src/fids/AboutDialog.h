#ifndef FIDS_ABOUTDIALOG_H_
#define FIDS_ABOUTDIALOG_H_

#include <QDialog>
#include <ui_AboutDialog.h>



class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	AboutDialog(QWidget* parent = nullptr);

private:
	Ui_AboutDialog ui;
};

#endif /* FIDS_ABOUTDIALOG_H_ */
