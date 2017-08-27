#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include "localstorage.h"
#include "settingsset.h"
#include <QDialog>
#include <QMessageBox>

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
    bool chkWeb();

private slots:
    void on_buttonBox_accepted();
    
    void on_SettingsDialog_finished(int /*result*/);

    void on_chkAutoClose_pressed();

private:
	Ui::SettingsDialog *ui;
	//QSettings *config;
};

#endif // SETTINGSDIALOG_H
