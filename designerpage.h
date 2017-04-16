#ifndef DESIGNERPAGE_H
#define DESIGNERPAGE_H

#include <QtWidgets>

#include "tabber.h"
#include "exercise.h"
#include "midi.h"

class DesignerPage : public QWidget
{
	Q_OBJECT
public:
	explicit DesignerPage(QWidget *parent = 0);

	void setMidiThread(Midi *pMidiThread);
	
public slots:
	void load();
	void save();

	void play();
	void pause();
	void stop();
	void loop();

private slots:
	//void speedCurveAddItemSlot();
	//void speedCurveDeleteItemSlot();
	void instrumentBoxChanged(int pIndex);

private:
	//QLabel *idLabel;
	//QLabel *nameLabel;
	//QLabel *authorLabel;
	//QLabel *descriptionLabel;

	QLineEdit *idLineEdit;
	QLineEdit *nameLineEdit;
	QLineEdit *authorLineEdit;
	QTextEdit *descriptionTextEdit;

	//QPushButton *speedCurveAddItemPushButton;
	//QPushButton *speedCurveDeleteItemPushButton;

	//QTableView *speedCurveTableView;
	//QStandardItemModel *speedCurveModel;

	QComboBox *instrumentBox;
	QPushButton *saveButton;
	QPushButton *loadButton;
	QPushButton *plusButton;
	QPushButton *minusButton;

	QList<Instrument> instruments;

    Tabber *tabber;

	Midi *midiThread;

	void clear();
};

#endif // DESIGNERPAGE_H
