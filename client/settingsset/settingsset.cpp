#include "settingsset.hpp"
#include <QDebug>
#include <QLocale>

SettingsSet::SettingsSet(QString fileName) : fileName{fileName} {
  qDebug() << fileName;
  this->readAll();
}

bool SettingsSet::readAll() {
  // qDebug() << "Call readAll" << endl;
  QSettings profile(fileName, QSettings::IniFormat);
  this->quitWhileCloseWindow =
      profile.value(KEY_QUIT_WHEN_CLOSE, false).toBool();
  this->hotkey = profile.value(KEY_HOTKEY, "").toString();
  this->autoRasdial = profile.value(KEY_AUTO_DIAL, true).toBool();
  this->autoMinimize = profile.value(KEY_AUTO_MINIMAIE, true).toBool();
  this->autoClose = profile.value(KEY_AUTO_CLOSE, false).toBool();
  this->autoStartup = profile.value(KEY_AUTO_STARTUP, false).toBool();
  this->webUpEnable = profile.value(KEY_WEBUP_ENABLE, false).toBool();
  this->language = profile.value(KEY_LANGUAGE).toString();

  // this->language = profile.value(KEY_LANGUAGE,
  // QLocale::system().name()).toString();
  return true;
}

void SettingsSet::writeAll() {
  qDebug() << "Call writeAll: " << fileName << endl;
  QSettings profile(fileName, QSettings::IniFormat);
  profile.setValue(KEY_HOTKEY, this->hotkey);
  profile.setValue(KEY_QUIT_WHEN_CLOSE, this->quitWhileCloseWindow);
  profile.setValue(KEY_WEBUP_ENABLE, this->webUpEnable);
  profile.setValue(KEY_AUTO_DIAL, this->autoRasdial);
  profile.setValue(KEY_AUTO_MINIMAIE, this->autoMinimize);
  profile.setValue(KEY_AUTO_CLOSE, this->autoClose);
  profile.setValue(KEY_AUTO_STARTUP, this->autoStartup);
  profile.setValue(KEY_LANGUAGE, this->language);
}
