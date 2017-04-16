#include "note.h"

bool Note::isEmpty()
{
	if(pitch == 0 && velocity == 0)
		return true;
	return false;
}

QDataStream &operator <<(QDataStream &out, const Note &note)
{
    out << note.getPitch() << note.getVelocity();

	return out;
}

QDataStream &operator >>(QDataStream &in, Note &note)
{
    quint8 pitch, velocity;

    in >> pitch >> velocity;

    note = Note(pitch, velocity);

	return in;
}
