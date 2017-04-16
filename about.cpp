#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::About)
{
	ui->setupUi(this);

	ui->label->setText(QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion());
}

About::~About()
{
	delete ui;
}
