#ifndef EXERCISEMANAGER_H
#define EXERCISEMANAGER_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>

#include "exercise.h"
#include "midi.h"
#include "xmlserialization.h"
#include "collection.h"

struct SpeedPoints
{
	quint16 startSpeed;
	quint16 maxSpeed;
	quint16 endSpeed;
};

class ExerciseManager
{
public:
	ExerciseManager();
	void addExercise(Exercise pExercise) { exercises.append(pExercise); }
	void deleteExercise(QString pId);

	QList<Exercise> *getExercises() { return &exercises; }

	QList<SpeedPoints> *getSpeedPoints() { return &speedPoints; }

	bool loadExercise(QString pFilename);
	bool loadCollection(QString pFilename);

private:
	QList<Exercise> exercises;
	QList<Collection> collections;

	// Start-, Maximal- und Endgeschwindigkeit einer Übung
	QList<SpeedPoints> speedPoints;
};

#endif // EXERCISEMANAGER_H
