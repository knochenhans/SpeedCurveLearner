#ifndef TRACK_H
#define TRACK_H

#include <QtGlobal>
#include <QVector>
#include <QDataStream>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QFile>

#include "note.h"
#include "instrument.h"

class Track
{
public:
	Track() {}
	Track(Instrument pInstrument);
	Track(Instrument pInstrument, const qint32 pVoiceCount, const qint32 pStepCount);
	Track(const qint32 pVoiceCount, const qint32 pStepCount);
	Track(const Track &other);

	void appendNote(const Note pNote, const int pVoice);

	int getVoiceCount() const { return noteMap.size(); }
	int getStepCount() const { return noteMap[0].size(); }

	void setSize(const qint32 pVoiceCount, const qint32 pStepCount);

	Note getNote(int pVoice, int pStep) const { return noteMap[pVoice][pStep]; }
	void setNote(Note pNote, int pVoice, int pStep);

	void setNoteMap(QVector< QVector <Note> > pNoteMap) { noteMap = pNoteMap; }

	Instrument getInstrument() const { return instrument; }
	void setInstrument(Instrument pInstrument) { instrument = pInstrument; }

	bool checkNote(Note pNote, int pVoice);

private:
	// Voice * Step
	QVector< QVector <Note> > noteMap;
	Instrument instrument;

	friend QDataStream &operator <<(QDataStream &out, const Track &track);
	friend QDataStream &operator >>(QDataStream &in, Track &track);
};

#endif // TRACK_H
