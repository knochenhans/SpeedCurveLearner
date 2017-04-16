#include "exercisemanager.h"

ExerciseManager::ExerciseManager()
{
	/*Collection coll1;
	coll1.setName("Übungen1");
	coll1.setAuthor("Andre Jonas");
	coll1.setDescription("Ein paar Übungen");

	Collection coll2;
	coll2.setName("Übungen2");
	coll2.setAuthor("Andre Jonas");
	coll2.setDescription("Noch ein paar Übungen");

	QStringList files;
	files << "16tel_4er_1234.xex";

	coll1.setFiles(files);
	files.clear();

	files << "16tel_4er_chromatisch.xex";

	coll2.setFiles(files);

	collections.append(coll1);
	collections.append(coll2);*/

	//QXmlStreamWriter *xmlOut = XMLSerialization::getInstance()->writeStartSettings();
	//xmlOut->writeStartElement("Collections");

	/*for(int i = 0; i < collections.size(); i++)
	{
		Collection collection = collections.at(i);
		QXmlStreamWriter *xmlOut = XMLSerialization::getInstance()->writeStartDocument(collection.getName() + ".xcl");
		XMLSerialization::getInstance()->writeCollection(collection, xmlOut);
		XMLSerialization::getInstance()->writeEndDocument(xmlOut);
	}*/

	//xmlOut->writeEndElement();
	//XMLSerialization::getInstance()->writeEndSettings(xmlOut);

	// Sammlungsdatei laden
	/*QXmlStreamReader *xmlIn = XMLSerialization::getInstance()->readStartDocument("collections.xcs");
	exercise = XMLSerialization::getInstance()->readExercise(xmlIn);
	XMLSerialization::getInstance()->readEndDocument(xmlIn);*/

	QDir collectionsDir("collections");

	if(collectionsDir.exists())
	{
		QStringList collections = collectionsDir.entryList(QDir::Files);

		foreach(QString entry, collections)
		{
			// Sammlungssdatei in exerciseManager einlesen
			if(!exerciseManager->load(collectionsDir.absolutePath() + "/" + entry))
			{
				qDebug() << "Fehler beim Einlesen der Sammlungsdatei " << entry;
			}
		}
	}
	else
	{
		qDebug() << "Fehler: Sammlungssordner " << collectionsDir.path() << " existiert nicht im Programmverzeichnis!";
	}
}

/*void ExerciseManager::deleteExercise(QString pId)
{
	for(QList<Exercise>::iterator i=exercises.begin(); i!=exercises.end(); ++i)
	{
		if((*i).getId() == pId)
		{
			exercises.erase(i);
			break;
		}
	}
}

bool ExerciseManager::loadExercise(QString pFilename)
{
	if(!pFilename.isEmpty())
	{
		Exercise exercise;

		QXmlStreamReader *xmlIn = XMLSerialization::getInstance()->readStartDocument(pFilename);
		exercise = XMLSerialization::getInstance()->readExercise(xmlIn);
		XMLSerialization::getInstance()->readEndDocument(xmlIn);

		exercises.append(exercise);

		// SpeedPoints für aktuelle Übung erstellen → erstmal Standardwerte festlegen (vll. später genauer ausbalanciere)

		SpeedPoints sp = {50, 120, 100};

		speedPoints.append(sp);
	}

	return true;
}*/


bool ExerciseManager::loadCollection(QString pFilename)
{
	return true;
}
