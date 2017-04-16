#include "exercisepage.h"

#include <QMessageBox>
#include <QDir>

ExercisePage::ExercisePage(QWidget *parent) : QWidget(parent)
{	
	currentSpeed = 0;
	currentSpeedLevel = 0;

	calibrationSpeedDefault = 80;
	calibrationSpeed = calibrationSpeedDefault;
	calibration = false;

	for(int i = 0; i < 18; i++)
	{
		calibrationSpeedCurve.append(i * 5 + 50);
	}

	createWidgets();

	playing = false;

	for(int i = 0; i < 5; i++)
	{
		userSpeedZones[i] = 0;
	}

	// Benutzereinstellungen laden
	QXmlStreamReader *xmlIn = XMLSerialization::getInstance()->readStartDocument("Andre.xus");

	while(!xmlIn->atEnd() && !xmlIn->hasError())
	{
		xmlIn->readNext();

		if(xmlIn->name() == "UserSettings")
		{
			userSettings = XMLSerialization::getInstance()->readUserSettings(xmlIn);
		}
	}

	XMLSerialization::getInstance()->readEndDocument(xmlIn);

	// Übungen laden
	loadExercises();
}

ExercisePage::~ExercisePage()
{
	QXmlStreamWriter *xmlOut = XMLSerialization::getInstance()->writeStartDocument(userSettings.name + ".xus");
	XMLSerialization::getInstance()->writeUserSettings(userSettings, xmlOut);
	XMLSerialization::getInstance()->writeEndDocument(xmlOut);
}

void ExercisePage::setMidiThread(Midi *pMidiThread)
{
	midiThread = pMidiThread;
	connect(midiThread, SIGNAL(newStep(uint)), tabber, SLOT(highlightStep(uint)));
	connect(midiThread, SIGNAL(stopped()), tabber, SLOT(clearSelection()));
	connect(midiThread, SIGNAL(finished()), this, SLOT(playingFinished()));
}

QList<quint16> ExercisePage::calcSpeedCurve(SpeedPoints pSpeedPoints)
{
	// Geschwindigkeitskurve aus Nutzerwerten generieren

	// Ansteigen auf (Maxpunkt - Startpunkt)

	float incline = float(pSpeedPoints.maxSpeed - pSpeedPoints.startSpeed) / 6;
	float decline = float(pSpeedPoints.maxSpeed - pSpeedPoints.endSpeed) / 5;

	QList<quint16> speedCurve;

	// erst 18 Schritte: 4 Aufwärmen, 6 Steigerung, 4 Abfall, 4 Abklingen

	for(int i = 0; i < 4; i++)
	{
		speedCurve.append(pSpeedPoints.startSpeed);
	}

	// Steigerung
	for(int i = 1; i <= 6; i++)
	{
		speedCurve.append(float(pSpeedPoints.startSpeed) + incline * float(i));
	}

	// Abfall
	for(int i = 1; i <= 4; i++)
	{
		speedCurve.append(float(pSpeedPoints.maxSpeed) - decline * float(i));
	}

	for(int i = 0; i < 4; i++)
	{
		speedCurve.append(pSpeedPoints.endSpeed);
	}

	return speedCurve;
}

void ExercisePage::setCurrentExercise(int collectionIndex, int exerciseIndex)
{
	SpeedPoints currentSpeedPoints;

	currentExercise = collections[collectionIndex].getExercise(exerciseIndex);

	// Falls Nutzereinstellungen zur Übung vorhanden, diese benutzen
	if(userSettings.speedPointsMapping.find(currentExercise.getId()) != userSettings.speedPointsMapping.end())
	{
		currentSpeedPoints = userSettings.speedPointsMapping.value(currentExercise.getId());
	}
	else
	{
		currentSpeedPoints = collections[collectionIndex].getSpeedPoint(exerciseIndex);
	}

	currentSpeedCurve = calcSpeedCurve(currentSpeedPoints);

	nameLabel->setText(currentExercise.getName());
	descriptionTextBrowser->setText(currentExercise.getDescription());

	Track track = currentExercise.getTracks().at(0);

	tabber->setTrack(track);
	speedCurveDisplay->setCurrentSpeedCurve(currentSpeedCurve);

	currentSpeedLevel = 0;
	currentSpeed = currentSpeedCurve.at(currentSpeedLevel);
}

void ExercisePage::loadExercises()
{
	if(Collection::getBaseDirectory().exists())
	{
		QStringList collectionFiles = Collection::getBaseDirectory().entryList(QDir::Files);

		foreach(QString entry, collectionFiles)
		{
			// Sammlungssdatei in exerciseManager einlesen

			Collection collection;

			QXmlStreamReader *xmlIn = XMLSerialization::getInstance()->readStartDocument(Collection::getBaseDirectory().absolutePath() + "/" + entry);

			while(!xmlIn->atEnd() && !xmlIn->hasError())
			{
				xmlIn->readNext();

				if(xmlIn->name() == "Collection")
				{
					collection = XMLSerialization::getInstance()->readCollection(xmlIn);
				}
			}

			XMLSerialization::getInstance()->readCollection(xmlIn);

			collections.append(collection);

			/*if(!exerciseManager->load(collectionsDir.absolutePath() + "/" + entry))
			{
				qDebug() << "Fehler beim Einlesen der Sammlungsdatei " << entry;
			}*/
		}
	}
	else
	{
		qDebug() << "Fehler: Sammlungssordner " << Collection::getBaseDirectory().path() << " existiert nicht im Programmverzeichnis!";
	}

	// Liste der Sammlungen durchgehen
	for(int i = 0; i < collections.size(); i++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(exerciseTree);
		item->setText(0, collections.at(i).getName());

		// Index der Sammlung als Referenz in TreeItem ablegen
		item->setData(0, Qt::UserRole, i);

		//item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		exerciseTree->addTopLevelItem(item);

		// Übungen einhängen
		collections[i].loadExercises();

		QList<Exercise> *exercises = collections[i].getExercises();

		for(int j = 0; j < exercises->size(); j++)
		{
			QTreeWidgetItem *childItem = new QTreeWidgetItem(item);
			childItem->setText(0, exercises->at(j).getName());

			// Index der Übung als Referenz in TreeItem ablegen
			childItem->setData(0, Qt::UserRole, j);
			item->addChild(childItem);
		}
	}

	exerciseTree->expandAll();

	// Übung als Startübung festlegen
	setCurrentExercise(0, 0);

	/*exerciseTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(exerciseTreeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));*/
}

/*void ExercisePage::showContextMenu(const QPoint& pos)
{
	// for most widgets
	QPoint globalPos = exerciseTreeView->mapToGlobal(pos);
	// for QAbstractScrollArea and derived classes you would use:
	// QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

	QMenu contextMenu;
	contextMenu.addAction(tr("Übung hinzufügen"));

	QAction* selectedItem = contextMenu.exec(globalPos);
	if(selectedItem)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(exerciseTreeView);
		item->setText(0, "Test3");
		exerciseTreeView->addTopLevelItem(item);
	}
	else
	{
	}
}*/

void ExercisePage::createWidgets()
{
	nameLabel = new QLabel("Test", this);

	QFont font;
	font.setPointSize(16);
	font.setBold(true);

	nameLabel->setFont(font);
	nameLabel->setAutoFillBackground(true);

	descriptionTextBrowser = new QTextBrowser(this);
	descriptionTextBrowser->setFrameStyle(QFrame::NoFrame);

	exerciseTree = new QTreeWidget();
	exerciseTree->setColumnCount(1);
	exerciseTree->header()->hide();

	speedCurveDisplay = new SpeedCurveDisplay(this);
	speedCurveDisplay->setMinimumSize(500, 300);

	diff1Button = new QPushButton("Sehr einfach");
	diff2Button = new QPushButton("Einfach");
	diff3Button = new QPushButton("Längere Zeit ok");
	diff4Button = new QPushButton("Abschnittsweise ok");
	diff5Button = new QPushButton("Unspielbar");
	calibrateButton = new QPushButton("Messen");

	// Tab-Ansicht vorbereiten

	tabber = new Tabber(this);
	tabber->setEditable(false);

	// In Layout einhängen

	QHBoxLayout *horizontalMain = new QHBoxLayout(this);
	QHBoxLayout *buttons = new QHBoxLayout();
	QGridLayout *grid = new QGridLayout();
	setLayout(horizontalMain);

	horizontalMain->addWidget(exerciseTree, 0);

	grid->addWidget(nameLabel, 0, 0, 1, 2, Qt::AlignTop|Qt::AlignHCenter);
	grid->addWidget(descriptionTextBrowser, 1, 0, 2, 1);
	grid->addWidget(speedCurveDisplay, 1, 1, 1, 1);
	grid->setRowStretch(0, 0);
	grid->setRowStretch(1, 1);
	grid->setRowStretch(2, 1);

	buttons->addWidget(diff1Button);
	buttons->addWidget(diff2Button);
	buttons->addWidget(diff3Button);
	buttons->addWidget(diff4Button);
	buttons->addWidget(diff5Button);
	buttons->addWidget(calibrateButton);

	grid->addLayout(buttons, 2, 1, 1, 1);

	grid->addWidget(tabber->getTableView(), 3, 0, 1, 2);

	grid->addItem(new QSpacerItem(100, 400), 4, 0, 1, 2);

	horizontalMain->addLayout(grid, 1);

	connect(exerciseTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
	connect(diff1Button, SIGNAL(clicked()), this, SLOT(diff1ButtonPressed()));
	connect(diff2Button, SIGNAL(clicked()), this, SLOT(diff2ButtonPressed()));
	connect(diff3Button, SIGNAL(clicked()), this, SLOT(diff3ButtonPressed()));
	connect(diff4Button, SIGNAL(clicked()), this, SLOT(diff4ButtonPressed()));
	connect(diff5Button, SIGNAL(clicked()), this, SLOT(diff5ButtonPressed()));
	connect(calibrateButton, SIGNAL(clicked()), this, SLOT(calibrateButtonPressed()));

	diff1Button->setDisabled(true);
	diff2Button->setDisabled(true);
	diff3Button->setDisabled(true);
	diff4Button->setDisabled(true);
	diff5Button->setDisabled(true);
}

void ExercisePage::treeItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
	if(pCurrent->parent())
	{
		int collectionIndex = pCurrent->parent()->data(0, Qt::UserRole).toInt();
		int exerciseIndex = pCurrent->data(0, Qt::UserRole).toInt();

		setCurrentExercise(collectionIndex, exerciseIndex);
	}
}

void ExercisePage::setVolume(int pNewVolume)
{
	midiThread->setVolume(pNewVolume);
}

void ExercisePage::speedStepChanged(unsigned int pNewSpeedStep, unsigned int pNewSpeedStepBPM)
{
	speedCurveDisplay->setCurrentSpeedStep(pNewSpeedStep);
	emit statusEvent(QString("Neuer Geschwindigkeitsschritt: %1 bpm").arg(pNewSpeedStepBPM));
}

void ExercisePage::diff1ButtonPressed()
{
	// Sehr einfach

	userSpeedZones[0] = currentSpeed;

	diff2Button->setEnabled(true);

	forward();
}

void ExercisePage::diff2ButtonPressed()
{
	// Einfach

	userSpeedZones[1] = currentSpeed;

	diff3Button->setEnabled(true);
	diff1Button->setDisabled(true);

	forward();
}

void ExercisePage::diff3ButtonPressed()
{
	// Längere Zeit ok

	userSpeedZones[2] = currentSpeed;

	diff4Button->setEnabled(true);
	diff2Button->setDisabled(true);

	forward();
}

void ExercisePage::diff4ButtonPressed()
{
	// Abschnittsweise ok

	userSpeedZones[3] = currentSpeed;

	diff5Button->setEnabled(true);
	diff3Button->setDisabled(true);

	forward();
}

void ExercisePage::diff5ButtonPressed()
{
	// Unspielbar

	userSpeedZones[4] = currentSpeed;

	emit calibrateButtonPressed();
}

void ExercisePage::calibrateButtonPressed()
{
	if(calibration)
	{
		diff1Button->setDisabled(true);
		diff2Button->setDisabled(true);
		diff3Button->setDisabled(true);
		diff4Button->setDisabled(true);
		diff5Button->setDisabled(true);

		calibrateButton->setPalette(style()->standardPalette());

		// Aus Messung neue Geschwindkeitskurve aufbauen

		SpeedPoints p;

		// Startgeschwindigkeit: etwas schneller als "einfach"
		p.startSpeed = userSpeedZones[1] + 5;

		// Höchstgeschwindigkeit: etwas langsamer als "unspielbar"
		p.maxSpeed = userSpeedZones[4] - 5;

		// Endgeschwindigkeit: langsamer als "abschnittsweise ok"
		p.endSpeed = userSpeedZones[3] - 10;

		currentSpeedCurve = calcSpeedCurve(p);

		//currentSpeedCurve = exerciseSpeedCurve;
		speedCurveDisplay->setCurrentSpeedCurve(currentSpeedCurve);

		// Benutzer-Werte für diese Übung speichern
		userSettings.speedPointsMapping.insert(currentExercise.getId(), p);

		calibration = false;
	}
	else
	{
		QMessageBox::information(this, "Messung 1: Startgeschwindigkeit", "Spielen Sie die Übung zweimal in der folgenden Geschwindigkeit und klicken Sie danach auf den entsprechenden Knopf.");

		diff1Button->setEnabled(true);

		exerciseSpeedCurve = currentSpeedCurve;
		currentSpeedCurve = calibrationSpeedCurve;
		speedCurveDisplay->setCurrentSpeedCurve(currentSpeedCurve);

		QPalette pal(palette());
		pal.setColor(QPalette::Button, Qt::blue);
		calibrateButton->setPalette(pal);

		calibration = true;
	}
}

void ExercisePage::play()
{	
	playing = true;
	exerciseTree->setDisabled(true);
	currentSpeed = currentSpeedCurve.at(currentSpeedLevel);
	speedStepChanged(currentSpeedLevel, currentSpeed);

	// Abspielen anstoßen (läuft im Hintergrund)
	midiThread->play(currentExercise, currentSpeed);
}

void ExercisePage::pause()
{
	midiThread->pause();
}

void ExercisePage::stop()
{
	playing = false;
	exerciseTree->setEnabled(true);
	midiThread->stop();
	emit finishedPlaying();
}

void ExercisePage::loop()
{
	//midiThread->loop();
}

void ExercisePage::forward()
{
	midiThread->stop();
	//playingFinished();
}

void ExercisePage::playingFinished()
{
	// Abspielen des aktuellen Schrittes abgeschlossen
	if(playing)
	{
		// Stop-Knopf wurde nicht gedrückt
		if(currentSpeedLevel < currentSpeedCurve.size() - 1)
		{
			// Es sind noch Geschwindigkeitstufen zum Abspielen vorhanden
			currentSpeed = currentSpeedCurve.at(++currentSpeedLevel);
			speedStepChanged(currentSpeedLevel, currentSpeed);
			midiThread->play(currentExercise, currentSpeed);
		}
		else
		{
			// letzter Schritt: Übung beendet, aktuelle Geschwindigkeit auf ersten Schritt zurücksetzen
			playing = false;

			currentSpeedLevel = 0;

			currentSpeed = currentSpeedCurve.at(currentSpeedLevel);
			speedStepChanged(currentSpeedLevel, currentSpeed);

			exerciseTree->setEnabled(true);

			emit finishedPlaying();
		}
	}
}
