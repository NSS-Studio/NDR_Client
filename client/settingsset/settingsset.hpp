#pragma once
#include <QSettings>
#include <QString>
#define KEY_HOTKEY "hotkey"
#define KEY_QUIT_WHEN_CLOSE "close_to_quit"
#define KEY_AUTO_DIAL "auto_redial"
#define KEY_AUTO_MINIMAIE "auto_minimize"
#define KEY_LANGUAGE "lang"
#define KEY_WEBUP_ENABLE "webUpEnable"
#define KEY_AUTO_CLOSE "autoClose"
#define KEY_AUTO_STARTUP "autoStartUp"
class SettingsSet final {
public:
  explicit SettingsSet(QString fileName);
  QString hotkey;
  QString language;
  bool autoRasdial;
  bool quitWhileCloseWindow;
  bool autoMinimize;
  bool webUpEnable;
  bool autoClose;
  bool autoStartup;

  bool readAll();
  void writeAll();

private:
  QString fileName;
};

