#include "common.hpp"
#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>

QString networkInterfaceCardName;

QString appHome;

void __initAppHome() {
    QDir home = QDir::home();
    appHome = home.path() + "/.nssdr";
    if (!QFile::exists(appHome)) {
        /** 第一个Bug出现的位置，把代码竟然TMD写到了调试信息里.....**/
        home.mkdir(".nssdr");
    }
    qDebug() << appHome;
}

QString tempDir;
void __initTempDir() {
    QDir temp = QDir::temp();
    tempDir = temp.path() + "/ndr";
    if (!QFile::exists(tempDir)) {
        temp.mkdir("ndr");
    }
    qDebug() << tempDir;
}

SettingsSet *settings;

void __initSettingsSet() {
    settings = new SettingsSet(appHome + "/config.ini");
}

QTranslator ndr_tr, qt_tr;
void __initLanguage() {
    QString lang_dir = utils::getLangDir();

    qDebug() << "lang_dir" << lang_dir;
    qDebug() << "language" << settings->language;
    QString current_locale = settings->language;
    if (current_locale.isEmpty())
        current_locale = QLocale::system().name();
    qDebug() << "current_locale" << current_locale;
    if (!ndr_tr.load("ndr." + current_locale + ".qm", lang_dir)) {
        qDebug() << "Load Qm file error";
    }

    qt_tr.load("qt_" + current_locale + ".qm",
               QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&ndr_tr);
    qApp->installTranslator(&qt_tr);
}

void __initFont(QString fontFile) {
    int fontId = QFontDatabase::addApplicationFont(fontFile);

    qDebug("font id:%d\n", fontId);
    if (fontId >= 0) {
        QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(msyh, 10);
        //将此字体设为QApplication的默认字体
        QApplication::setFont(font);
    }
}

//void __initStyleSheet(NdrApplication *a, QString sheetFile) {
//    // QString sheetName = "default";
//    QFile file(sheetFile);
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());
//#if defined Q_OS_WIN || defined Q_OS_MAC
//    QString appath = a->applicationDirPath();
//#ifdef Q_OS_WIN
//    styleSheet.replace("$[SKIN_PATH]", appath + "/skin");
//#else
//    styleSheet.replace("$[SKIN_PATH]", appath + "/../Resources/skin");
//#endif
//#else
//    styleSheet.replace("$[SKIN_PATH]", "/usr/share/ndr-client/skin");
//#endif
//    qDebug() << styleSheet;
//    // a->setStyleSheet(styleSheet);
//    a->setStyleSheet(styleSheet);
//}
