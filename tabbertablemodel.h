#ifndef TABBERTABLEMODEL_H
#define TABBERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include <QStringList>
#include <QDebug>

#include "track.h"

//! TabberTableModel: stark vereinfachtes Modell zur Speicherung eingegebener Track-Daten als Tabelle
/*!
Ich arbeite hier aus Gründen der Einfachheit erstmal nur mit Pitches. In Zukunft müsste ein Weg gefunden werden, Noten ordentlich einzugeben.
*/
class TabberTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
	explicit TabberTableModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void appendSteps(int pStepCount);
	void cropSteps(int pStepCount);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	Qt::ItemFlags flags(const QModelIndex & index) const;

	void clear();
	void setSize(int pVoices, int pSteps);

	void fromTrack(const Track &track);
	Track toTrack() const;

	QVector< QVector <QString> > * getPitchMap() { return &pitchMap; }

private:
	//Track *track;
	QVector< QVector <QString> > pitchMap;
};

#endif // TABBERTABLEMODEL_H
