#include "collection.h"

#include "xmlserialization.h"

QStringList Collection::getExerciseNames() const
{
	QStringList list;

	for(int i = 0; i < exercises.size(); i++)
	{
		list << exercises.at(i).getName();
	}
	return list;
}

void Collection::loadExercises()
{
	foreach(QString file, files)
	{
		loadExercise(baseDirectory.absolutePath() + "/" + getName() + "/" + file);
	}
}

bool Collection::loadExercise(QString pFilename)
{
	if(!pFilename.isEmpty())
	{
		Exercise exercise;

		QXmlStreamReader *xmlIn = XMLSerialization::getInstance()->readStartDocument(pFilename);

		while(!xmlIn->atEnd() && !xmlIn->hasError())
		{
			xmlIn->readNext();

			if(xmlIn->name() == "Exercise")
			{
				exercise = XMLSerialization::getInstance()->readExercise(xmlIn);
			}
		}

		XMLSerialization::getInstance()->readEndDocument(xmlIn);

		// Clicktrack für Exercise erzeugen → überlegen, wie und wo das zu konfigurieren sein soll
		exercise.generateClickTrack(16, 2, 0);

		exercises.append(exercise);

		// SpeedPoints für aktuelle Übung erstellen → erstmal Standardwerte festlegen (vll. später genauer ausbalanciere)

		SpeedPoints sp = {50, 120, 100};

		speedPoints.append(sp);
	}
	else
		return false;

	return true;
}

QDir Collection::baseDirectory("collections");
