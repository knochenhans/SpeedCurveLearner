#ifndef SPEEDCURVEDISPLAY_H
#define SPEEDCURVEDISPLAY_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <algorithm>
#include <QRectF>

class SpeedCurveDisplay : public QWidget
{
	Q_OBJECT
public:
	explicit SpeedCurveDisplay(QWidget *pParent = 0);

	void setCurrentSpeedCurve(QList<quint16> pCurrentSpeedCurve);
	void setCurrentSpeedStep(quint16 pCurrentSpeedStep);

	void drawGrid(QRectF pDrawArea, int pRows, int pColumns, QPainter *pPainter);
	void drawSpeedCurveTable(QRectF pDrawArea, QPainter *pPainter);
	void drawMeasure(QRectF pDrawArea, int pMinValue, int pMaxValue, int pStep, QString pUnit, QPainter *pPainter);
	
protected:
	void paintEvent(QPaintEvent *event);

private:
	QList<quint16> currentSpeedCurve;
	quint16 currentSpeedStep;
};

#endif // SPEEDCURVEDISPLAY_H
