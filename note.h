#ifndef NOTE_H
#define NOTE_H

#include <QtGlobal>
#include <QDataStream>
#include <QDebug>

#include "midiDefines.h"

class Note
{
public:
	Note() : pitch(0), velocity(0) {}
	Note(const quint8 pPitch, const quint8 pVelocity) : pitch(pPitch), velocity(pVelocity) {}
    Note(const Note &other)
    {
        pitch = other.getPitch();
        velocity = other.getVelocity();
    }
	~Note() {}

	quint8 getVelocity() const { return velocity; }
	void setVelocity(quint8 pVelocity) { velocity = pVelocity; }

	quint8 getPitch() const { return pitch; }
	void setPitch(quint8 pPitch) { pitch = pPitch; }

	bool isEmpty();

protected:
	quint8 pitch;       //MIDI-Pitch, wird von Instrument automatisch in Fret-Position umgewandelt
	quint8 velocity;

	friend QDataStream &operator <<(QDataStream &out, const Note &note);
	friend QDataStream &operator >>(QDataStream &in, Note &note);
};

#endif // NOTE_H
