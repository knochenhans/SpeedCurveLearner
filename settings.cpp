#include "settings.h"

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

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    hBoxLayout->addWidget(lbChannel);
    hBoxLayout->addWidget(cbChannel);
    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(buttonBox);
    setLayout(vBoxLayout);
    setWindowTitle(tr("Einstellungen"));
}

Settings::~Settings()
{

}
