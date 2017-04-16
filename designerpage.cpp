#include "designerpage.h"

DesignerPage::DesignerPage(QWidget *parent) : QWidget(parent)
{
	idLineEdit = new QLineEdit(this);
	idLineEdit->setFixedHeight(20);
	idLineEdit->setStatusTip(tr("ID"));
	nameLineEdit = new QLineEdit(this);
	nameLineEdit->setFixedHeight(20);
	nameLineEdit->setStatusTip(tr("Name"));
	authorLineEdit = new QLineEdit(this);
	authorLineEdit->setFixedHeight(20);
	authorLineEdit->setStatusTip(tr("Autor"));
	descriptionTextEdit = new QTextEdit(this);
	descriptionTextEdit->setStatusTip(tr("Beschreibung"));
	descriptionTextEdit->setMaximumHeight(60);

	//speedCurveTableView = new QTableView(this);
	//speedCurveTableView->setStatusTip(tr("Geschwindigkeitsschritte"));

	//speedCurveAddItemPushButton = new QPushButton("Erstellen", this);
	//speedCurveDeleteItemPushButton = new QPushButton("Löschen", this);

	//grid->addWidget(speedCurveTableView, 0, 6, 4, 2);
	//grid->addWidget(speedCurveAddItemPushButton, 4, 6, 1, 1);
	//grid->addWidget(speedCurveDeleteItemPushButton, 4, 7, 1, 1);

	//connect(speedCurveAddItemPushButton, SIGNAL(clicked()), this, SLOT(speedCurveAddItemSlot()));
	//connect(speedCurveDeleteItemPushButton, SIGNAL(clicked()), this, SLOT(speedCurveDeleteItemSlot()));

	//speedCurveModel = new QStandardItemModel(0, 0, this);
	//speedCurveModel->setHorizontalHeaderItem(0, new QStandardItem(QString(tr("BPM"))));
	//speedCurveTableView->setModel(speedCurveModel);

	//setLayout(grid);

	Instrument guitar("Gitarre");
	Instrument bass("Bass");

	QList<quint8> stringTunings;
	stringTunings.append(64);
	stringTunings.append(59);
	stringTunings.append(55);
	stringTunings.append(50);
	stringTunings.append(45);
	stringTunings.append(40);

	guitar.setStringCount(6);
	guitar.setFretCount(21);
	guitar.setStringTunings(stringTunings);
	guitar.setMidiProgram(30);

	instruments.append(guitar);

	stringTunings.clear();

	stringTunings.append(43);
	stringTunings.append(38);
	stringTunings.append(33);
	stringTunings.append(28);

	bass.setFretCount(21);
	bass.setStringCount(4);
	bass.setStringTunings(stringTunings);
	bass.setMidiProgram(34);

	instruments.append(bass);

	tabber = new Tabber(this);
	tabber->setCurrentInstrument(guitar);
	tabber->show();

	// erstmal Gitarre als Standardinstrument erstellen

	instrumentBox = new QComboBox;

	for(int x = 0; x < instruments.size(); x++)
	{
		instrumentBox->insertItem(x, instruments.at(x).getName());
	}

	saveButton = new QPushButton("Spur speichern", this);
	loadButton = new QPushButton("Spur laden", this);
	plusButton = new QPushButton("1/4 +", this);
	minusButton = new QPushButton("1/4 -", this);

	QGridLayout *gridLayout = new QGridLayout(this);
	QFormLayout *formLayout = new QFormLayout;
	//grid->setSpacing(10);
	setLayout(gridLayout);

	formLayout->addRow(tr("&ID:"), idLineEdit);
	formLayout->addRow(tr("&Name:"), nameLineEdit);
	formLayout->addRow(tr("&Autor:"), authorLineEdit);
	formLayout->addRow(tr("&Beschreibung:"), descriptionTextEdit);

	gridLayout->addLayout(formLayout, 0, 0, 1, 2);

	gridLayout->addWidget(tabber->getTableView(), 1, 0, 5, 1);

	gridLayout->addWidget(instrumentBox, 1, 1, 1, 1);

	gridLayout->addWidget(loadButton, 2, 1, 1, 1);
	gridLayout->addWidget(saveButton, 3, 1, 1, 1);
	gridLayout->addWidget(plusButton, 4, 1, 1, 1);
	gridLayout->addWidget(minusButton, 5, 1, 1, 1);

	connect(saveButton, SIGNAL(clicked()), tabber, SLOT(saveTrack()));
	connect(loadButton, SIGNAL(clicked()), tabber, SLOT(loadTrack()));
	connect(plusButton, SIGNAL(clicked()), tabber, SLOT(plusBeat()));
	connect(minusButton, SIGNAL(clicked()), tabber, SLOT(minusBeat()));

	connect(instrumentBox, SIGNAL(currentIndexChanged(int)), this, SLOT(instrumentBoxChanged(int)));
}

void DesignerPage::setMidiThread(Midi *pMidiThread)
{
	midiThread = pMidiThread;
	//connect(midiThread, SIGNAL(newStep(uint)), tabber, SLOT(highlightStep(uint)));
}

void DesignerPage::load()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Übung öffnen"), "", tr("Übungsdateien (*.xex)"));

	if(!fileName.isEmpty())
	{
		Exercise exercise;

		QXmlStreamReader *xmlIn = XMLSerialization::getInstance()->readStartDocument(fileName);
		exercise = XMLSerialization::getInstance()->readExercise(xmlIn);
		XMLSerialization::getInstance()->readEndDocument(xmlIn);

		idLineEdit->setText(exercise.getId());
		nameLineEdit->setText(exercise.getName());
		authorLineEdit->setText(exercise.getAuthor());
		descriptionTextEdit->setText(exercise.getDescription());

		// TODO: muss noch für mehrere Tracks angepasst werden
		Track track = exercise.getTracks().at(0);

		tabber->setTrack(track);
	}
}

void DesignerPage::save()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Übung speichern"), "", tr("Übungsdateien (*.xex)"));

	if(!fileName.isEmpty())
	{
		// Track aus Tabber abholen
		QList<Track> tracks;
		tracks.append(tabber->getTrack());

		// ID berechnen
		QString string = nameLineEdit->text() + authorLineEdit->text() + descriptionTextEdit->toPlainText();
		QString id = QString(QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5).toHex());

		Exercise exercise(id, nameLineEdit->text(), authorLineEdit->text(), descriptionTextEdit->toPlainText(), tracks);

		QXmlStreamWriter *xmlOut = XMLSerialization::getInstance()->writeStartDocument(fileName);
		XMLSerialization::getInstance()->writeExercise(exercise, xmlOut);
		XMLSerialization::getInstance()->writeEndDocument(xmlOut);
	}
}

void DesignerPage::play()
{
	/*QList<quint16> speedCurve;

	for(int i = 0; i < speedCurveModel->rowCount(); i++)
	{
		speedCurve.append(speedCurveModel->item(i, 0)->text().toUInt());
	}*/

	// Track aus Tabber abholen
	QList<Track> tracks;
	tracks.append(tabber->getTrack());

	Exercise exercise(idLineEdit->text(), nameLineEdit->text(), authorLineEdit->text(), descriptionTextEdit->toPlainText(), tracks);

	// Mit aktueller SpeedCurve abspielen
	//midiThread->play(exercise, speedCurve);
}

void DesignerPage::pause()
{
	midiThread->pause();
}

void DesignerPage::stop()
{
	midiThread->stop();
}

void DesignerPage::loop()
{
	//midiThread->loop();
}

/*void DesignerPage::speedCurveAddItemSlot()
{
	speedCurveModel->appendRow(new QStandardItem(""));
}

void DesignerPage::speedCurveDeleteItemSlot()
{
	QItemSelection selection(speedCurveTableView->selectionModel()->selection());

	QList<int> rows;
	foreach(const QModelIndex &index, selection.indexes())
	{
		rows.append(index.row());
	}

	qSort(rows);

	int prev = -1;
	for(int i = rows.count() - 1; i >= 0; i -= 1)
	{
		int current = rows[i];
		if(current != prev)
		{
			speedCurveModel->removeRows(current, 1);
			prev = current;
		}
	}
}*/

void DesignerPage::clear()
{
	idLineEdit->clear();
	nameLineEdit->clear();
	authorLineEdit->clear();
	descriptionTextEdit->clear();
	//speedCurveModel->clear();
}

void DesignerPage::instrumentBoxChanged(int pIndex)
{
	// TableModel an Eigenschaften des neuen Instruments anpassen
	//tableModel->setSize(instruments[pIndex].getStringCount(), tableModel->columnCount());
}
