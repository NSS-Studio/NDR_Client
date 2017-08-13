#include "settingsset.h"
#include <QDebug>
#include <QtCore/QLocale>

SettingsSet::SettingsSet(QString filename)
{
    //qDebug() << "Call SettingsSet\n" << filename << endl;
    this->filename = filename;
    this->readAll();
}

bool SettingsSet::readAll()
{
    //qDebug() << "Call readAll" << endl;
    QSettings profile(filename,QSettings::IniFormat);
    this->quitWhileCloseWindow = profile.value(KEY_QUIT_WHEN_CLOSE,false).toBool();
    this->hotkey = profile.value(KEY_HOTKEY,"").toString();
    this->autoRasdial = profile.value(KEY_AUTO_DIAL,true).toBool();
    this->autoMinimize = profile.value(KEY_AUTO_MINIMAIE,true).toBool();
    this->language = profile.value(KEY_LANGUAGE).toString();
    //this->language = profile.value(KEY_LANGUAGE, QLocale::system().name()).toString();
	return true;
}

void SettingsSet::writeAll()
{
    qDebug() << "Call writeAll: " << filename << endl;
    QSettings profile(filename,QSettings::IniFormat);
    profile.setValue(KEY_HOTKEY,this->hotkey);
    profile.setValue(KEY_QUIT_WHEN_CLOSE,this->quitWhileCloseWindow);
    profile.setValue(KEY_AUTO_DIAL,this->autoRasdial);
    profile.setValue(KEY_AUTO_MINIMAIE,this->autoMinimize);
    profile.setValue(KEY_LANGUAGE,this->language);
}
