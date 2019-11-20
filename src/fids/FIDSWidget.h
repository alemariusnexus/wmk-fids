#ifndef FIDS_FIDSWIDGET_H_
#define FIDS_FIDSWIDGET_H_

#include <QWidget>
#include <QList>
#include <QTimer>
#include <ui_FIDSWidget.h>
#include "FlightPlan.h"
#include "FIDSTableModel.h"


class FIDSWidget : public QWidget
{
	Q_OBJECT

public:
	FIDSWidget(QWidget* parent = nullptr);
	virtual ~FIDSWidget();

	void cue(const QString& cue);

protected:
	virtual void keyReleaseEvent(QKeyEvent* evt);
	virtual void resizeEvent(QResizeEvent* evt);
	virtual void closeEvent(QCloseEvent* evt);

signals:
	void closed();

private slots:
	void init();
	void tick();

private:
	void autoResizeTable();
	FIDSTableModel::RowScheme loadRowScheme(const rapidjson::Value& jscheme) const;

private:
	Ui_FIDSWidget ui;
	QTimer tickTimer;
	FIDSTableModel* fidsModel;
	QList<float> columnSizes;
	QString curTimeFormat;
};

#endif /* FIDS_FIDSWIDGET_H_ */
