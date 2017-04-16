#ifndef TABBERSCENE_H
#define TABBERSCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QMap>

#include "tabbertabnote.h"
#include "instrument.h"

class TabberScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TabberScene(Instrument *pInstrument, QMap<int, TabberTabNote *> *pTrack, QObject *parent = 0)
        : QGraphicsScene(parent), instrument(pInstrument), track(pTrack) {}
    void drawBackground(QPainter *painter, const QRectF &rect);
    void keyPressEvent(QKeyEvent *event);
    
signals:
    
public slots:
    
private:
    Instrument *instrument;

    QMap<int, TabberTabNote *> *track;
};

#endif // TABBERSCENE_H
