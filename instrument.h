#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QList>
#include <QString>
#include <QDataStream>

#include "note.h"

//! Instrument, um Spezialitäten eines Instruments beim Tabben und Abspielen abzubilden
/*!
    Hier werden Anzahl der Saiten, Anzahl der Bünde und Stimmung der Saiten erfasst. Außerdem
    lassen sich MIDI-Notenzahlen generieren und korrekte Tab-Eingaben überprüfen.
*/
class Instrument
{
public:
	Instrument(QString pName, quint8 pFretCount, quint8 pStringCount, QList<quint8> pStringTunings)
		: name(pName), stringCount(pStringCount), fretCount(pFretCount), stringTunings(pStringTunings) {}

	Instrument(QString pName);
	Instrument();

	QString getName() const { return name; }
	void setName(QString pName) { name = pName; }

	quint8 getStringCount() const { return stringCount; }
	void setStringCount(quint8 pStringCount) { stringCount = pStringCount; }

	quint8 getFretCount() const { return fretCount; }
	void setFretCount(quint8 pFretCount) { fretCount = pFretCount; }

	QList<quint8> getStringTunings() const { return stringTunings; }
	void setStringTunings(QList<quint8> pStringTunings) { stringTunings = pStringTunings; }

	bool isPercussion() const { return percussion; }
	void setPercussion(bool pPercussion) { percussion = pPercussion; }

	quint8 getMidiProgram() const { return midiProgram; }
	void setMidiProgram(quint8 pMidiProgram) { midiProgram = pMidiProgram; }

	quint8 getMidiPitchFromNote(quint8 pString, quint8 pFret) const;
	bool checkNote(quint8 pFret) const;

private:
    QString name;
	quint8 stringCount;
	quint8 fretCount;
	QList<quint8> stringTunings;
	quint8 midiProgram;
	bool percussion;

	friend QDataStream &operator <<(QDataStream &out, const Instrument &instrument);
	friend QDataStream &operator >>(QDataStream &in, Instrument &instrument);
};

#endif // INSTRUMENT_H
