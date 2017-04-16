#include "tabbertabnote.h"

void TabberTabNote::keyPressEvent(QKeyEvent *event)
{
    QString newString = event->text();

    switch(event->key())
    {
    case Qt::Key_Space:
        setPlainText(" ");
        break;

    case Qt::Key_R:
        setPlainText("R");
        break;

    default:
        if(!newString.isEmpty())
        {
            //QRegExp rx("(\\d+)");

            // Wenn Zeit zwischen zwei Eingaben gering ist, diese als zusammengesetzte Zahl-Eingabe werten
            if(keyPressTimeLast.msecsTo(QTime::currentTime()) < 500)
            {
                newString = toPlainText() + event->text();
            }
            else
            {
                newString = event->text();
            }

            if(checkTabNote(newString))
                setPlainText(newString);

            keyPressTimeLast = QTime::currentTime();
        }
        else
            setPlainText(newString);
    }
}

bool TabberTabNote::checkTabNote(const QString pTabNote)
{
    bool check;

    // Ist die Eingabe überhaupt eine Zahl?
    pTabNote.toInt(&check);

    if(check)
        return instrument->checkNote(pTabNote.toInt());
    else
        return false;
}
