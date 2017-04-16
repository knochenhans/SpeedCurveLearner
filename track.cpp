#include "track.h"

Track::Track(Instrument pInstrument) : instrument(pInstrument)
{
	int voiceCount = instrument.getStringCount();

	// String-Anzahl festlegen
	noteMap.resize(voiceCount);
}

Track::Track(Instrument pInstrument, const qint32 pVoiceCount, const qint32 pStepCount)
{
	//Track(pVoiceCount, pStepCount);
	setSize(pVoiceCount, pStepCount);
	instrument = pInstrument;
}

Track::Track(const qint32 pVoiceCount, const qint32 pStepCount)
{
	setSize(pVoiceCount, pStepCount);
}

Track::Track(const Track &other)
{
	noteMap = other.noteMap;
	instrument = other.instrument;
}

void Track::appendNote(const Note pNote, const int pVoice)
{
	//setNote(pNote, pVoice, noteMap[0].size());
	// An Vector der entsprechenden Voice anhängen, alle anderen Voices mit Leerstellen auffüllen
	for(int v = 0; v < noteMap.size(); v++)
	{
		if(v == pVoice)
		{
			noteMap[pVoice].append(pNote);
		}
		else
		{
			noteMap[pVoice].append(Note(0, 0));
		}
	}
}

void Track::setSize(const qint32 pVoiceCount, const qint32 pStepCount)
{
	noteMap.resize(pVoiceCount);

	for(int v = 0; v < pVoiceCount; v++)
	{
		noteMap[v].resize(pStepCount);
	}
}

void Track::setNote(Note pNote, int pVoice, int pStep)
{
	if(checkNote(pNote, pVoice))
	{
		if(pVoice < noteMap.size())
		{
			if(pStep < noteMap[0].size())
			{
				// Note hinzufügen
				noteMap[pVoice][pStep] = pNote;
			}
			else
				qDebug() << "Note (" << pVoice << "," << pStep << ") würde außerhalb des Tracks liegen!";
		}
		else
			qDebug() << "Note (" << pVoice << "," << pStep << ") würde außerhalb des Tracks liegen!";
	}
}

bool Track::checkNote(Note pNote, int pVoice)
{
	if(pVoice > 15)
	{
		qDebug() << "Track::checkNote(): Es sind maximal 16 MIDI-Kanäle nutzbar!";
		return false;
	}
	return true;
}

QDataStream &operator<<(QDataStream &out, const Track &track)
{
	// Voices
	out << track.noteMap.size();

	// Steps
	out << track.noteMap[0].size();

	for(int v = 0; v < track.noteMap.size(); v++)
	{
		for(int s = 0; s < track.noteMap[0].size(); s++)
		{
			out << track.getNote(v, s);
		}
	}

	out << track.getInstrument();

	//qDebug() << "Track: " << track.getInstrument().getName();

	return out;
}

QDataStream &operator>>(QDataStream &in, Track &track)
{	
	int voices, steps;

	in >> voices;

	in >> steps;

	// Track mithilfe der ermittelten Größen festlegen
	track.setSize(voices, steps);

	if(voices > 0)
	{
		if(steps > 0)
		{
			qDebug() << "Track lesen:";
			for(int v = 0; v < voices; v++)
			{
				for(int s = 0; s < steps; s++)
				{
					Note note;

					in >> note;

					track.setNote(note, v, s);
				}
			}
		}
		else
		{
			qDebug() << "Datei fehlerhaft: Keine Steps vorhanden!";
		}
	}
	else
	{
		qDebug() << "Datei fehlerhaft: Keine Voices vorhanden!";
	}

	Instrument instrument;

	in >> instrument;

	track.setInstrument(instrument);

	return in;
}
