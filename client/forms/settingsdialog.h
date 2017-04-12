#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include "localstorage.h"
#include "settingsset.h"
#include <QDialog>

class QSettings;
namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent=0);
    ~SettingsDialog();
    bool getFormData(SettingsSet *settings);

private slots:
    void on_buttonBox_accepted();
    
    void on_SettingsDialog_finished(int /*result*/);
    
private:
	Ui::SettingsDialog *ui;
	//QSettings *config;
};

#endif // SETTINGSDIALOG_H
