#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include "localstorage.h"
#include "settingsset.h"
#include <QDialog>
#include <QMessageBox>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winreg.h>
#include <winbase.h>
#endif

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

#ifdef Q_OS_WIN
    enum class dialogKinds{stander, question, warning, error};

    bool isRunAsAdmin();
    bool delReg(HKEY proxyRoot, const QString &keyWay, const QString &keyName);
    bool creatReg(HKEY proxyRoot, const QString &keyWay, const QString &keyName, const QString &keyValue);
    QMessageBox::StandardButton message(dialogKinds question, const QString &text);
#endif

private slots:
    void on_buttonBox_accepted();
    
    void on_SettingsDialog_finished(int /*result*/);

    void on_chkAutoClose_pressed();

private:
	Ui::SettingsDialog *ui;
	//QSettings *config;
};

#endif // SETTINGSDIALOG_H
