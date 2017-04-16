#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDial>
#include <QToolButton>
#include <QLabel>
#include <QtWidgets>
#include <QStackedWidget>

#include "midi.h"
#include "RtMidi.h"
#include "about.h"
#include "exercise.h"
#include "exercisepage.h"
#include "designerpage.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void finishedPlaying();
	void newSpeedStep(unsigned int pNewSpeedStep);
	void exercisePlay();
	void exercisePause();
	void exerciseStop();
	void exerciseLoop();
	void exerciseForward();
	void volumeDialMoved(int pNewVolume);

	void designerLoad();
	void designerSave();

	void about();

	void exercisePageClicked();
	void designerPageClicked();

private:
	QStringList midiOutPortNameList;

	bool looped;

	QLabel *volumeLabel;

	QDial *volumeDial;

	QAction *playAction;
	QAction *pauseAction;
	QAction *stopAction;
	QAction *loopAction;
	QAction *forwardAction;

	QAction *settingsAction;
	QAction *aboutAction;

	QAction *exercisePageAction;
	QAction *designerPageAction;

	QAction *loadDesignerAction;
	QAction *saveDesignerAction;

	QMenu *fileMenu;
	QMenu *helpMenu;

	QToolBar *playToolBar;
	QToolBar *pagesToolBar;
	QToolBar *designerToolBar;

	ExercisePage *exercisePage;
	DesignerPage *designerPage;

	QStackedWidget *stackedWidget;

	Midi *midiThread;

	void createMenus();
	void createActions();
	void createToolbars();
	void createStatusBar();
	void createWidgets();

	void readSettings();
	void writeSettings();
};

#endif // MAINWINDOW_H
