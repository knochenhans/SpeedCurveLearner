#include "tabbertablemodel.h"

TabberTableModel::TabberTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int TabberTableModel::rowCount(const QModelIndex &) const
{
	// Voices
	return pitchMap.size();
}

int TabberTableModel::columnCount(const QModelIndex &) const
{
	// Rows
	return pitchMap[0].size();
}

void TabberTableModel::appendSteps(int pStepCount)
{
	setSize(pitchMap.size(), pitchMap[0].size() + pStepCount);
}

void TabberTableModel::cropSteps(int pStepCount)
{
	setSize(pitchMap.size(), pitchMap[0].size() - pStepCount);
}

QVariant TabberTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if(!pitchMap.isEmpty())
		{
			if(!pitchMap[0].isEmpty())
			{
				return pitchMap[index.row()][index.column()];
			}
		}
	}
	return QVariant::Invalid;
}

bool TabberTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole)
	{
		if(!pitchMap.isEmpty())
		{
			if(!pitchMap[0].isEmpty())
			{
				pitchMap[index.row()][index.column()] = value.toString();
			}
		}
	}
	return true;
}

Qt::ItemFlags TabberTableModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void TabberTableModel::clear()
{
	for(int v = 0; v < pitchMap.size(); v++)
	{
		// Steps
		pitchMap[v].clear();
	}
	pitchMap.clear();
}

void TabberTableModel::setSize(int pVoices, int pSteps)
{
	beginResetModel();

	// Voices / Rows

	pitchMap.resize(pVoices);

	// Steps / Columns

	for(int v = 0; v < pitchMap.size(); v++)
	{
		pitchMap[v].resize(pSteps);
	}

	endResetModel();
}

void TabberTableModel::fromTrack(const Track &track)
{
	clear();

	// Größe der Tabelle an Track anpassen
	setSize(track.getVoiceCount(), track.getStepCount());

	// Einträge übertragen

	// Voices
	for(int v = 0; v < rowCount(); v++)
	{
		// Steps
		for(int s = 0; s < columnCount(); s++)
		{
			// Nur Notes mit Velocity > 0 eintragen
			if(track.getNote(v, s).getVelocity() > 0)
			{
				pitchMap[v][s] = QString("%1").arg(track.getNote(v, s).getPitch());
			}
		}
	}
}


Track TabberTableModel::toTrack() const
{
	Track track(rowCount(), columnCount());

	// Voices
	for(int v = 0; v < rowCount(); v++)
	{
		// Steps
		for(int s = 0; s < columnCount(); s++)
		{
			if(!pitchMap[v][s].isEmpty())
			{
				Note note(pitchMap[v][s].toInt(), 127);
				track.setNote(note, v, s);
			}
			else
			{
				// Leere Note, wenn nichts eingetragen
				Note note(0, 0);
				track.setNote(note, v, s);
			}
		}
	}

	return track;
}
