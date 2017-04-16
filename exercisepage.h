#ifndef EXERCISEPAGE_H
#define EXERCISEPAGE_H

#include <QtWidgets>

#include "exercise.h"
#include "midi.h"
#include "tabber.h"
#include "speedcurvedisplay.h"
#include "collection.h"

class ExercisePage : public QWidget
{
	Q_OBJECT
public:
	explicit ExercisePage(QWidget *parent = 0);
	~ExercisePage();

	void setMidiThread(Midi *pMidiThread);

	QList<quint16> calcSpeedCurve(SpeedPoints pSpeedPoints);

	void setCurrentExercise(int collectionIndex, int exerciseIndex);

signals:
	void statusEvent(const QString &pText);
	void finishedPlaying();

public slots:
	void play();
	void pause();
	void stop();
	void loop();
	void forward();

	void playingFinished();

private slots:
	//void showContextMenu(const QPoint& pos);
	void treeItemChanged(QTreeWidgetItem *pCurrent,QTreeWidgetItem *pPrevious);
	void setVolume(int pNewVolume);

	void diff1ButtonPressed();
	void diff2ButtonPressed();
	void diff3ButtonPressed();
	void diff4ButtonPressed();
	void diff5ButtonPressed();
	void calibrateButtonPressed();

private:
	void loadExercises();
	void createWidgets();
	void speedStepChanged(unsigned int pNewSpeedStep, unsigned int pNewSpeedStepBPM);

	QLabel *nameLabel;
	QTreeWidget *exerciseTree;
	QTextBrowser *descriptionTextBrowser;
	QPushButton *diff1Button;
	QPushButton *diff2Button;
	QPushButton *diff3Button;
	QPushButton *diff4Button;
	QPushButton *diff5Button;
	QPushButton *calibrateButton;

	Tabber *tabber;
	Midi *midiThread;
	SpeedCurveDisplay *speedCurveDisplay;

	Exercise currentExercise;

	quint16 currentSpeed;
	int currentSpeedLevel;
	QList<quint16> currentSpeedCurve;
	QList<quint16> calibrationSpeedCurve;
	QList<quint16> exerciseSpeedCurve;

	QList<Collection> collections;

	bool playing;
	bool calibration;

	quint16 calibrationSpeedDefault;
	quint16 calibrationSpeed;

	quint16 userSpeedZones[5];

	UserSettings userSettings;
};

#endif // EXERCISEPAGE_H
