#ifndef TABBER_H
#define TABBER_H

#include <QtWidgets>
#include <QHeaderView>
#include <QTableView>
#include <QList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "instrument.h"
#include "tabbertablemodel.h"
#include "xmlserialization.h"

//! TabberWidget, zur Eingabe von Tabs
/*!
Als Widget einbindbar, dient zur Verwaltung, Darstellung und Eingabe von Tabs.
*/

class Tabber : public QWidget
{
    Q_OBJECT
public:
	explicit Tabber(QWidget *parent = 0);

	Track getTrack() const;
	void setTrack(Track &track);

	Instrument getCurrentInstrument() const { return currentInstrument; }
	void setCurrentInstrument(Instrument &pCurrentInstrument) { currentInstrument = pCurrentInstrument; }

	void setEditable(bool pEditable);

	QTableView *getTableView() { return tableView; }

public slots:
	void saveTrack();
	void loadTrack();
	void plusBeat();
	void minusBeat();

	void resetView();
	void highlightStep(unsigned int pStep);
	void clearSelection();

private:
	QTableView *tableView;
	TabberTableModel *tableModel;

	Instrument currentInstrument;
};

#endif // TABBER_H
