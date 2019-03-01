#pragma once
#include "localstorage.hpp"
#include "settingsset.hpp"
#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSharedPointer>

#ifdef Q_OS_WIN32
    #include <windows.h>
#endif

namespace Ui {
class SettingsDialog;
}

class SettingsDialog final : public QDialog {
  Q_OBJECT
public:
  explicit SettingsDialog(QWidget *parent = nullptr);
  ~SettingsDialog() = default;
  bool getFormData(SettingsSet *settings);
  bool chkWeb();

#ifdef Q_OS_WIN
  enum class dialogKinds { stander, question, warning, error };

  bool isRunAsAdmin();
  bool delReg(HKEY proxyRoot, const QString &keyWay, const QString &keyName);
  bool creatReg(HKEY proxyRoot, const QString &keyWay, const QString &keyName,
                const QString &keyValue);
  QMessageBox::StandardButton message(dialogKinds question,
                                      const QString &text);
#endif

private slots:
  void on_buttonBox_accepted();
  void on_SettingsDialog_finished(int /*result*/);
  void on_chkAutoClose_pressed();

private:
  QSharedPointer<Ui::SettingsDialog> ui;
};

