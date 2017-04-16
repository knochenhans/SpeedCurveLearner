#include "tabber.h"

Tabber::Tabber(QWidget *parent) : QWidget(parent)
{
	//QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);

	tableModel = new TabberTableModel(this);
	tableModel->setSize(6, 32);

	tableView = new QTableView(this);
	tableView->horizontalHeader()->hide();
	tableView->verticalHeader()->hide();
	tableView->setTabKeyNavigation(false);
	tableView->setModel(tableModel);
	//tableView->setSelectionMode(QTableView::NoSelection);
	//tableView->selectColumn(2);
	//tableView->setFixedSize(500, 200);
	//tableView->adjustSize();
	tableView->setEditTriggers(QTableView::AnyKeyPressed);
	//tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//tableView->setFixedWidth(1000);
	//tableView->setFixedHeight(tableModel->rowCount() * 20 + 2);
	tableView->setFixedHeight(tableModel->rowCount() * 20 + 18);

	//layout->addWidget(tableView);

	//setLayout(layout);

	connect(tableModel, SIGNAL(modelReset()), this, SLOT(resetView()));

	resetView();
}

// Liefert den Track an das aufrufende Objekt zurück
Track Tabber::getTrack() const
{
	Track track(tableModel->toTrack());

	// Aktuelles Instrument für Track festschreiben
	track.setInstrument(currentInstrument);

	return track;
}

void Tabber::setTrack(Track &track)
{
	tableModel->fromTrack(track);
}

void Tabber::setEditable(bool pEditable)
{
	if(pEditable)
		tableView->setEditTriggers(QTableView::AnyKeyPressed);
	else
		tableView->setEditTriggers(QTableView::NoEditTriggers);
}

void Tabber::saveTrack()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Spur speichern"), "", tr("Spurdateien (*.xtr)"));

	if(!fileName.isEmpty())
	{
		QXmlStreamWriter *xml = XMLSerialization::getInstance()->writeStartDocument(fileName);

		Track track(getTrack());
		XMLSerialization::getInstance()->writeTrack(track, xml);
		XMLSerialization::getInstance()->writeEndDocument(xml);
	}
}

void Tabber::loadTrack()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Spur öffnen"), "", tr("Spurdateien (*.xtr)"));

	if(!fileName.isEmpty())
	{
		QXmlStreamReader *xml = XMLSerialization::getInstance()->readStartDocument(fileName);
		XMLSerialization::getInstance()->readTrack(xml);
		XMLSerialization::getInstance()->readEndDocument(xml);
	}
}

void Tabber::plusBeat()
{
	tableModel->appendSteps(4);
}

void Tabber::minusBeat()
{
	tableModel->cropSteps(4);
}

void Tabber::resetView()
{
	for(int x = 0; x < tableModel->columnCount(); x++)
	{
		for(int y = 0; y < tableModel->rowCount(); y++)
		{
			tableView->setColumnWidth(x, 20);
			tableView->setRowHeight(y, 20);
		}
	}

	// Größe an Inhalt anpassen
	//tableView->setMinimumWidth(tableModel->columnCount() * 20 + 2);
	//tableView->setMaximumWidth(tableModel->columnCount() * 20 + 2 + 300);
	//tableView->setMinimumHeight(tableModel->rowCount() * 20 + 2);
}

void Tabber::highlightStep(unsigned int pStep)
{
	tableView->selectColumn(pStep);
}

void Tabber::clearSelection()
{
	tableView->clearSelection();
}
