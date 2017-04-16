#ifndef COLLECTION_H
#define COLLECTION_H

#include <QString>
#include <QStringList>
#include <QDir>

#include "exercise.h"

struct SpeedPoints
{
	quint16 startSpeed;
	quint16 maxSpeed;
	quint16 endSpeed;
};

class Collection
{
public:
	Collection() {}

	void setName(QString pName) { name = pName; }
	QString getName() const { return name; }

	void setDescription(QString pDescription) { description = pDescription; }
	QString getDescription() const { return description; }

	void setAuthor(QString pAuthor) { author = pAuthor; }
	QString getAuthor() const { return author; }

	void setFiles(QStringList pFiles) { files = pFiles; }
	QStringList getFiles() const { return files; }

	Exercise getExercise(int pIndex) { return exercises.at(pIndex); }
	QStringList getExerciseNames() const;

	QList<Exercise> *getExercises() { return &exercises; }

	SpeedPoints getSpeedPoint(int pIndex) { return speedPoints.at(pIndex); }

	void loadExercises();

	static QDir getBaseDirectory() { return baseDirectory; }

private:
	QString name;
	QString description;
	QString author;
	QStringList files;

	// Werden nicht mit serialisiert, sondern zur Laufzeit geladen
	QList<Exercise> exercises;

	// Start-, Maximal- und Endgeschwindigkeit einer Übung
	QList<SpeedPoints> speedPoints;

	bool loadExercise(QString pFilename);

	static QDir baseDirectory;
};

#endif // COLLECTION_H
