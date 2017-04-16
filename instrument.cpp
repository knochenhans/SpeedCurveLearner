#include "instrument.h"

Instrument::Instrument(QString pName) : name(pName), stringCount(0), fretCount(0)
{
    // Standard-Instrument: Gitarre mit Standard-E-Stimmung

	/*stringTunings.append(64);
	stringTunings.append(59);
	stringTunings.append(55);
	stringTunings.append(50);
	stringTunings.append(45);
	stringTunings.append(40);*/

	midiProgram = 0;
	percussion = false;
}

Instrument::Instrument() : name(""), stringCount(0), fretCount(0)
{
	midiProgram = 0;
	percussion = false;
}

quint8 Instrument::getMidiPitchFromNote(quint8 pString, quint8 pFret) const
{
	// MIDI-Pitch aus Voice und (relativem) Pitch der Note errechnen (Gitarre: String und Fret)
	return stringTunings[pString] + pFret;
}

bool Instrument::checkNote(quint8 pFret) const
{
   if(pFret > fretCount)
   {
       return false;
   }
   return true;
}

QDataStream &operator <<(QDataStream &out, const Instrument &instrument)
{
	out << instrument.getName()
		<< instrument.getStringCount()
		<< instrument.getFretCount()
		<< instrument.getStringTunings()
		<< instrument.getMidiProgram()
		<< instrument.isPercussion();

	return out;
}

QDataStream &operator >>(QDataStream &in, Instrument &instrument)
{
	QString name;
	quint8 stringCount, fretCount, midiProgram;
	QList<quint8> stringTunings;
	bool percussion;

	in >> name
	   >> stringCount
	   >> fretCount
	   >> stringTunings
	   >> midiProgram
	   >> percussion;

	instrument.setName(name);
	instrument.setStringCount(stringCount);
	instrument.setFretCount(fretCount);
	instrument.setStringTunings(stringTunings);
	instrument.setMidiProgram(midiProgram);
	instrument.setPercussion(percussion);

	return in;
}
