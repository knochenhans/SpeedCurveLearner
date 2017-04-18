#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLayout>

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QStringList ports, QWidget *parent = 0);
    ~Settings();

private:
    QLabel *lbChannel;
    QComboBox *cbChannel;
    QDialogButtonBox *buttonBox;
};

#endif // SETTINGS_H
