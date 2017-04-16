#ifndef TABBERTABNOTE_H
#define TABBERTABNOTE_H

#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QList>
#include <QTime>

#include "instrument.h"

//! Bildet eine Tabnote im Tab ab.
/*!
Reräsentation einer Tabnote im Tab. Definiert durch Saiten-/Bund-Position auf Instrument. Wenn Saite und Bund = 0, dann ist es eine leere Note.
*/

class TabberTabNote : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit TabberTabNote(const QString &text, Instrument *pInstrument, QGraphicsItem *parent = 0)
        : QGraphicsTextItem(text, parent), fret(0), string(0), instrument(pInstrument), keyPressTimeLast(QTime::currentTime()) {}

signals:
    
public slots:
    
private:
    void keyPressEvent(QKeyEvent *event);
    bool checkTabNote(const QString pTabNote);

    int fret;
    int string;
    Instrument *instrument;

    QTime keyPressTimeLast;
};

#endif // TABBERTABNOTE_H
