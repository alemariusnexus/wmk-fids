#ifndef FIDS_FIDSWIDGET_H_
#define FIDS_FIDSWIDGET_H_

#include <QWidget>
#include <QList>
#include <QTimer>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <ui_FIDSWidget.h>
#include "FlightPlan.h"
#include "FIDSTableModel.h"


class FIDSWidget : public QWidget
{
	Q_OBJECT

private:
	enum FadeType
	{
		FadeTypeInvalid,
		FadeTypeInOut
	};

public:
	FIDSWidget(QWidget* parent = nullptr);
	virtual ~FIDSWidget();

	void setWeather(const QString& weather);

	void cueFade(uint64_t fadeTime, FadeType type, uint64_t fadeMidTime = 0);

protected:
	virtual void keyReleaseEvent(QKeyEvent* evt);
	virtual void resizeEvent(QResizeEvent* evt);
	virtual void closeEvent(QCloseEvent* evt);

signals:
	void closed();

private slots:
	void init();
	void tick();
	void fadeTick();
	void cueTriggered(const QString& cue);

private:
	void autoResizeTable();
	FIDSTableModel::RowScheme loadRowScheme(const rapidjson::Value& jscheme) const;

private:
	Ui_FIDSWidget ui;
	QTimer tickTimer;
	FIDSTableModel* fidsModel;
	QList<float> columnSizes;
	QString curTimeFormat;
	QString curWeather;
	QMovie* curWeatherMovie;

	QTimer fadeTimer;
	uint64_t fadeStartTime;
	uint64_t fadeTimeTotal;
	uint64_t fadeMidTime;
	FadeType fadeType;
	QGraphicsOpacityEffect* fadeEffect;
};

#endif /* FIDS_FIDSWIDGET_H_ */
