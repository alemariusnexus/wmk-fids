#ifndef FIDS_FIDSCONTROLWINDOW_H_
#define FIDS_FIDSCONTROLWINDOW_H_

#include <QMainWindow>
#include <ui_FIDSControlWindow.h>
#include "FIDSWidget.h"


class FIDSControlWindow : public QMainWindow
{
	Q_OBJECT

public:
	FIDSControlWindow(QWidget* parent = nullptr);
	virtual ~FIDSControlWindow();

protected:
	virtual void closeEvent(QCloseEvent* evt);
	virtual void keyReleaseEvent(QKeyEvent* evt);

private slots:
	void init();
	void fidsWidgetClosed();
	void cueButtonClicked();
	void onAbout();
	void cueTriggered(const QString& cue);

private:
	Ui_FIDSControlWindow ui;
	FIDSWidget* fidsWidget;
};

#endif /* FIDS_FIDSCONTROLWINDOW_H_ */
