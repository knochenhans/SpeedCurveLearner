#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QApplication::setStyle(QStyleFactory::create("fusion"));

	QCoreApplication::setOrganizationDomain("Andre Jonas");
	QCoreApplication::setOrganizationName("Andre Jonas");
	QCoreApplication::setApplicationName("SpeedCurveLearner");
	QCoreApplication::setApplicationVersion("1.0.0");

	midiThread = new Midi();

	if(!midiThread->initialize())
	{
		QMessageBox msgBox;
		msgBox.setText("Fehler bei der MIDI-Initialisierung.");
		msgBox.exec();
	}

	createWidgets();
	createActions();
	createMenus();
	createToolbars();
	createStatusBar();

    readSettings();

	looped = false;
}

void MainWindow::createActions()
{
	// Play

    playAction = new QAction(QIcon("../SpeedCurveLearner/Icons/start.png"), tr("&Abspielen"), this);
	//playAction->setShortcuts(QKeySequence::New);
	playAction->setStatusTip(tr("Spielt die aktuelle Übung ab."));
	connect(playAction, SIGNAL(triggered()), this, SLOT(exercisePlay()));

    pauseAction = new QAction(QIcon("../SpeedCurveLearner/Icons/pause.png"), tr("&Pause"), this);
	//pauseAction->setShortcuts(QKeySequence::New);
	pauseAction->setStatusTip(tr("Pausiert die aktuelle Übung."));
	connect(pauseAction, SIGNAL(triggered()), this, SLOT(exercisePause()));

    stopAction = new QAction(QIcon("../SpeedCurveLearner/Icons/stop.png"), tr("&Stop"), this);
	//pauseAction->setShortcuts(QKeySequence::New);
	stopAction->setStatusTip(tr("Stoppt die aktuelle Übung."));
	connect(stopAction, SIGNAL(triggered()), this, SLOT(exerciseStop()));

	// relative vs. absolute Pfade überarbeiten
    forwardAction = new QAction(QIcon("../SpeedCurveLearner/Icons/forward.png"), tr("&Stufe überspringen"), this);
	//pauseAction->setShortcuts(QKeySequence::New);
	forwardAction->setStatusTip(tr("Überspringt die aktuelle Geschwindigkeitsstufe."));
	connect(forwardAction, SIGNAL(triggered()), this, SLOT(exerciseForward()));

    loopAction = new QAction(QIcon("../SpeedCurveLearner/Icons/loop_deactivated.png"), tr("&Wiederholen"), this);
	//pauseAction->setShortcuts(QKeySequence::New);
	loopAction->setStatusTip(tr("Wiederholt die aktuelle Übung kontinuierlich."));
	connect(loopAction, SIGNAL(triggered()), this, SLOT(exerciseLoop()));

	pauseAction->setDisabled(true);
	stopAction->setDisabled(true);
	forwardAction->setDisabled(true);

    settingsAction = new QAction(QIcon("../SpeedCurveLearner/Icons/preferences.png"), tr("&Einstellungen"), this);
	settingsAction->setStatusTip(tr("Einstellungen des Programms."));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(settings()));

	aboutAction = new QAction(tr("&Über %1 …").arg(QCoreApplication::applicationName()), this);
	aboutAction->setStatusTip(tr("Informationen über %1.").arg(QCoreApplication::applicationName()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	// Windows

	QShortcut *exercisePageShortcut = new QShortcut(QKeySequence("Ctrl+1"), this);
	QShortcut *designerPageShortcut = new QShortcut(QKeySequence("Ctrl+2"), this);
	QShortcut *stopPlaying = new QShortcut(Qt::Key_Space, this);

	connect(stopPlaying, SIGNAL(activated()), this, SLOT(exerciseStop()));

    exercisePageAction = new QAction(QIcon("../SpeedCurveLearner/Icons/exercise.png"), tr("&Übungen"), this);
	exercisePageAction->setStatusTip(tr("Übungen"));
	connect(exercisePageAction, SIGNAL(triggered()), this, SLOT(exercisePageClicked()));
	connect(exercisePageShortcut, SIGNAL(activated()), this, SLOT(exercisePageClicked()));

    designerPageAction = new QAction(QIcon("../SpeedCurveLearner/Icons/designer.png"), tr("&Designer"), this);
	designerPageAction->setStatusTip(tr("Designer"));
	connect(designerPageAction, SIGNAL(triggered()), this, SLOT(designerPageClicked()));
	connect(designerPageShortcut, SIGNAL(activated()), this, SLOT(designerPageClicked()));

	// Designer

    loadDesignerAction = new QAction(QIcon("../SpeedCurveLearner/Icons/open.png"), tr("&Übung laden"), this);
	loadDesignerAction->setStatusTip(tr("&Übung laden"));

    saveDesignerAction = new QAction(QIcon("../SpeedCurveLearner/Icons/save.png"), tr("&Übung speichern"), this);
	saveDesignerAction->setStatusTip(tr("&Übung speichern"));
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&Datei"));
	fileMenu->addAction(settingsAction);

	helpMenu = menuBar()->addMenu(tr("&Hilfe"));
	helpMenu->addAction(aboutAction);
}

void MainWindow::createToolbars()
{
	pagesToolBar = addToolBar(tr("Fenster"));
	pagesToolBar->addAction(exercisePageAction);
	pagesToolBar->addAction(designerPageAction);

	playToolBar = addToolBar(tr("Abspielen"));
	playToolBar->addAction(playAction);
	playToolBar->addAction(pauseAction);
	playToolBar->addAction(stopAction);
	playToolBar->addAction(forwardAction);
	playToolBar->addAction(loopAction);

	volumeDial = new QDial(stackedWidget);
	volumeDial->setFixedSize(32, 32);
	volumeDial->setRange(0, 127);

	volumeLabel = new QLabel(stackedWidget);
	volumeLabel->setText("Vol");

	playToolBar->addSeparator();

	playToolBar->addWidget(volumeLabel);
	playToolBar->addWidget(volumeDial);

	designerToolBar = addToolBar(tr("Designer"));
	designerToolBar->addAction(loadDesignerAction);
	designerToolBar->addAction(saveDesignerAction);

	connect(loadDesignerAction, SIGNAL(triggered()), this, SLOT(designerLoad()));
	connect(saveDesignerAction, SIGNAL(triggered()), this, SLOT(designerSave()));

	connect(volumeDial, SIGNAL(sliderMoved(int)), midiThread, SLOT(setVolume(int)));
	connect(volumeDial, SIGNAL(sliderMoved(int)), this, SLOT(volumeDialMoved(int)));

	designerToolBar->hide();
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Bereit"));
}

void MainWindow::createWidgets()
{
	exercisePage = new ExercisePage;
	designerPage = new DesignerPage;

	stackedWidget = new QStackedWidget;
	stackedWidget->addWidget(exercisePage);
	stackedWidget->addWidget(designerPage);
	setCentralWidget(stackedWidget);

	connect(exercisePage, SIGNAL(statusEvent(QString)), statusBar(), SLOT(showMessage(QString)));
	connect(exercisePage, SIGNAL(finishedPlaying()), this, SLOT(finishedPlaying()));

	exercisePage->setMidiThread(midiThread);
	designerPage->setMidiThread(midiThread);
}

void MainWindow::writeSettings()
{
	QSettings settings;

	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();

	settings.beginGroup("MIDI");
	settings.setValue("Volume", volumeDial->value());
	settings.endGroup();
}

void MainWindow::readSettings()
{
	QSettings settings;

	settings.beginGroup("MainWindow");
    /*resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());*/
	settings.endGroup();

	int volume = 0;

	settings.beginGroup("MIDI");
	volume = settings.value("Volume", 100).toInt();
	settings.endGroup();

	volumeDial->setValue(volume);
	midiThread->setVolume(volume);
}

MainWindow::~MainWindow()
{
	this->writeSettings();

	midiThread->quit();

	delete midiThread;
}

void MainWindow::about()
{
	About aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::settings()
{
    Settings settingsDialog(midiThread->getPorts(), this);
    settingsDialog.exec();
}

void MainWindow::finishedPlaying()
{
	playAction->setEnabled(true);
	pauseAction->setDisabled(true);
	stopAction->setDisabled(true);
	forwardAction->setDisabled(true);
}

void MainWindow::newSpeedStep(unsigned int pNewSpeedStep)
{
	statusBar()->showMessage(tr("Neuer Geschwindigkeitsschritt: %1 BPM").arg(pNewSpeedStep));
}

void MainWindow::exercisePlay()
{
	playAction->setDisabled(true);
	pauseAction->setEnabled(true);
	stopAction->setEnabled(true);
	forwardAction->setEnabled(true);

	if(stackedWidget->currentIndex() == 0)
		exercisePage->play();
	else if(stackedWidget->currentIndex() == 1)
		designerPage->play();
}

void MainWindow::exercisePause()
{
	playAction->setEnabled(true);

	if(stackedWidget->currentIndex() == 0)
		exercisePage->pause();
	else if(stackedWidget->currentIndex() == 1)
		designerPage->pause();
}

void MainWindow::exerciseStop()
{
	playAction->setEnabled(true);
	pauseAction->setDisabled(true);
	stopAction->setDisabled(true);
	forwardAction->setDisabled(true);

	if(stackedWidget->currentIndex() == 0)
		exercisePage->stop();
	else if(stackedWidget->currentIndex() == 1)
		designerPage->stop();
}

void MainWindow::exerciseLoop()
{
	if(stackedWidget->currentIndex() == 0)
		exercisePage->loop();
	else if(stackedWidget->currentIndex() == 1)
		designerPage->loop();

	if(looped)
	{
        loopAction->setIcon(QIcon("../SpeedCurveLearner/Icons/loop_deactivated.png"));
		looped = false;
	}
	else
	{
        loopAction->setIcon(QIcon("../SpeedCurveLearner/Icons/loop.png"));
		looped = true;
	}
}

void MainWindow::exerciseForward()
{
	if(stackedWidget->currentIndex() == 0)
		exercisePage->forward();
}

void MainWindow::volumeDialMoved(int pNewVolume)
{
	statusBar()->showMessage(tr("Lautstärke: %1 %").arg(roundf(100.0 / 127.0 * float(pNewVolume))));
}

void MainWindow::exercisePageClicked()
{
	stackedWidget->setCurrentIndex(0);
	designerToolBar->hide();
}

void MainWindow::designerPageClicked()
{
	stackedWidget->setCurrentIndex(1);
	designerToolBar->show();
}

void MainWindow::designerLoad()
{
	designerPage->load();
}

void MainWindow::designerSave()
{
	designerPage->save();
}
