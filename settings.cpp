#include "settings.h"

void Settings::accept()
{

}

void Settings::reject()
{

}

Settings::Settings(QStringList ports, QWidget *parent) :
    QDialog(parent)
{
    lbChannel = new QLabel(tr("Kanal"));
    cbChannel = new QComboBox();

    for(int i = 0; i < ports.size(); i++) {
        cbChannel->addItem(QString::number(i) + ": " + ports[i]);
    }

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(cbChannel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Einstellungen"));
}

Settings::~Settings()
{

}
