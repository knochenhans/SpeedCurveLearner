#include "speedcurvedisplay.h"

SpeedCurveDisplay::SpeedCurveDisplay(QWidget *pParent) : QWidget(pParent)
{
	//setFixedSize(500, 230);
	currentSpeedStep = 0;
}

void SpeedCurveDisplay::setCurrentSpeedCurve(QList<quint16> pCurrentSpeedCurve)
{
	currentSpeedCurve = pCurrentSpeedCurve;
	update();
}

void SpeedCurveDisplay::setCurrentSpeedStep(quint16 pCurrentSpeedStep)
{
	currentSpeedStep = pCurrentSpeedStep;
	update();
}

void SpeedCurveDisplay::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(event->rect(), Qt::white);

	// x - left und width - right?
	QRectF drawArea(10, 10, width() - 10, height() - 30);

	drawSpeedCurveTable(drawArea, &painter);

	painter.end();
}

void SpeedCurveDisplay::drawSpeedCurveTable(QRectF pDrawArea, QPainter *pPainter)
{
	// Maximalwerte bestimmen
	QList<quint16>::iterator itMax = std::max_element(currentSpeedCurve.begin(), currentSpeedCurve.end());
	QList<quint16>::iterator itMin = std::min_element(currentSpeedCurve.begin(), currentSpeedCurve.end());

	quint16 maxBPM = *itMax;
	quint16 minBPM = *itMin;

	// Rausfinden, welche BPM-Zahlen dargestellt werden müssen

	// Minimum auf Zehner nach unten begrenzen / Maximum auf nächst höchsten Zehner setzen
	minBPM = ((minBPM - 10)/ 10) * 10;
	maxBPM = ((maxBPM + 10) / 10) * 10;

	// links etwas Platz für die Legende lassen
	QRectF gridDrawArea = pDrawArea;
	gridDrawArea.setLeft(gridDrawArea.left() + 30.0);
	gridDrawArea.setWidth(gridDrawArea.width() + 30.0);

	// Text etwas nach unten verschieben, sodass er zu den Linien passt
	QRectF measureDrawArea = pDrawArea;
	measureDrawArea.setTop(pDrawArea.top() + 3);
	measureDrawArea.setHeight(pDrawArea.height() + 3);
	measureDrawArea.setWidth(50);

	drawGrid(gridDrawArea, (maxBPM - minBPM) / 10, currentSpeedCurve.size(), pPainter);
	drawMeasure(measureDrawArea, minBPM, maxBPM, 10, "bpm", pPainter);

	// SpeedCurve einzeichnen

	QBrush column(QColor(180, 200, 200));
	QBrush highlightedColumn(QColor(230, 250, 250));
	//QBrush defaultBrush = pPainter->brush();

	// Schrittweiten errechnen
	qreal distX = (gridDrawArea.width() - gridDrawArea.x()) / qreal(currentSpeedCurve.size());
	qreal distY = (gridDrawArea.height() - gridDrawArea.y()) / qreal(maxBPM - minBPM);

	qreal height = 0;
	QRectF rect;

	for(int i = 0; i < currentSpeedCurve.size(); i++)
	{
		if(i == currentSpeedStep)
			pPainter->setBrush(highlightedColumn);
		else
			pPainter->setBrush(column);

		rect.setLeft(qRound(gridDrawArea.x() + qreal(i) * distX));
		rect.setWidth(qRound(distX));
		height = qRound(distY * (currentSpeedCurve.at(i) - minBPM));

		// Vom Ende des Grid-Diplays abziehen, um reale Koordinaten zu erhalten
		rect.setTop(gridDrawArea.height() - height);
		rect.setBottom(gridDrawArea.height());

		pPainter->drawRect(rect);
	}
}

void SpeedCurveDisplay::drawGrid(QRectF pDrawArea, int pRows, int pColumns, QPainter *pPainter)
{
	// Schrittweiten errechnen
	qreal distX = (pDrawArea.width() - pDrawArea.x()) / qreal(pColumns);
	qreal distY = (pDrawArea.height() - pDrawArea.y()) / qreal(pRows);

	// Vertikale Linien
	for(int i = 0; i < pColumns + 1; i++)
	{
		pPainter->drawLine(qRound(pDrawArea.x() + qreal(i) * distX), pDrawArea.y(), qRound(pDrawArea.x() + qreal(i) * distX), pDrawArea.height());
	}

	// Horizontale Linien
	for(int i = 0; i < pRows + 1; i++)
	{
		pPainter->drawLine(pDrawArea.x(), qRound(pDrawArea.y() + qreal(i) * distY), pDrawArea.width(), qRound(pDrawArea.y() + qreal(i) * distY));
	}
}

void SpeedCurveDisplay::drawMeasure(QRectF pDrawArea, int pMinValue, int pMaxValue, int pStep, QString pUnit, QPainter *pPainter)
{
	// erstmal nur Vertikal

	// Anzahl der zu zeichnenden Schritte
	int count = (pMaxValue - pMinValue) / pStep;

	// Schrittweiten errechnen
	qreal distY = (pDrawArea.height() - pDrawArea.y()) / qreal(count);

	for(int i = 0; i < count + 1; i++)
	{
		pPainter->drawText(pDrawArea.x(), (pDrawArea.y() + qreal(i) * distY), QString("%1").arg(pMaxValue - i * pStep));
	}

	// Einheit zeichnen
	pPainter->drawText(pDrawArea.x(), (pDrawArea.y() + qreal(count) * distY) + 20, pUnit);
}
