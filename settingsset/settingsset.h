#ifndef SETTINGSSET_H
#define SETTINGSSET_H
#include <QtCore/QString>
#include <QtCore/QSettings>
#define KEY_HOTKEY          "hotkey"
#define KEY_QUIT_WHEN_CLOSE "close_to_quit"
#define KEY_AUTO_DIAL       "auto_redial"
#define KEY_AUTO_MINIMAIE   "auto_minimize"
#define KEY_LANGUAGE        "lang"
class SettingsSet
{
public:
    SettingsSet(QString filename);
    QString hotkey;
    QString language;
    bool autoRasdial;
    bool quitWhileCloseWindow;
    bool autoMinimize;
    
    bool readAll();
    void writeAll();
private:
    QString filename;
};

#endif // SETTINGSSET_H
