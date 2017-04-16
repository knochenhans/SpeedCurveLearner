#include "tabberscene.h"

void TabberScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left)
    {
        focusItem()->clearFocus();

        QGraphicsItem *item = track->value(1);
        item->setFocus();
    }

    // den Rest von den Items selbst bearbeiten lassen
    QGraphicsScene::keyPressEvent(event);
}

void TabberScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(QColor(200, 200, 200)));

    for(qreal x = 0; x < instrument->getStringCount(); x++)
    {
        painter->drawLine(0, x * 30, 1000, x * 30);
    }
}
