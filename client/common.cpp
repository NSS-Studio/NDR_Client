#include "common.h"
#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>

QString networkInterfaceCardName;

QString __getVersionString() {
    quint8 majorVersion = VERSION_MAJOR;
    quint8 minorVersion = VERSION_MINOR;
    Q_ASSERT(minorVersion < 100);
    return QString("%1").arg(majorVersion + minorVersion / 100.0, 0, 'f', 2);
}

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

#define DR_MODEL_ID_CAMPAS ""
#define DR_MODEL_ID_EDU "@edu"
#define DR_MODEL_ID_UNICOM "@cnc"
#define DR_MODEL_ID_UNICOM_CONTRACT "@cnc30a"
#define DR_MODEL_ID_UNICOM_CONTRACT_50 "@cnc50a"
#define DR_MODEL_ID_TELECOM "@ct10"
#define DR_MODEL_ID_TELECOM_CONTRACT "@ct10a"

QStringList __getDrModelPostfixTable() {
    QStringList table;
    table.append(DR_MODEL_ID_EDU);
    table.append(DR_MODEL_ID_UNICOM);
    table.append(DR_MODEL_ID_UNICOM_CONTRACT);
    table.append(DR_MODEL_ID_UNICOM_CONTRACT_50);
    table.append(DR_MODEL_ID_TELECOM);
    table.append(DR_MODEL_ID_TELECOM_CONTRACT);
    table.append(DR_MODEL_ID_CAMPAS); //顺序很重要
    return table;
}

bool __getDrModelCaption(QString postfix, QString &caption) {
    qDebug() << __func__ << "postfix:" << postfix;
    if (postfix == DR_MODEL_ID_CAMPAS)
        caption = QObject::tr("考试专用");
    else if (postfix == DR_MODEL_ID_EDU)
        caption = QObject::tr("教育网2M");
    else if (postfix == DR_MODEL_ID_UNICOM)
        caption = QObject::tr("联通20M");
    else if (postfix == DR_MODEL_ID_UNICOM_CONTRACT)
        caption = QObject::tr("联通合约30M");
    else if (postfix == DR_MODEL_ID_UNICOM_CONTRACT_50)
        caption = QObject::tr("联通合约50M");
    else if (postfix == DR_MODEL_ID_TELECOM)
        caption = QObject::tr("电信10M");
    else if (postfix == DR_MODEL_ID_TELECOM_CONTRACT)
        caption = QObject::tr("电信合约10M");
    else
        return false;
    return true;
}

QString getLangDir() {
    qDebug("function: getLangDir()");
    /*
#ifdef Q_OS_WIN
    QString r = QApplication::applicationDirPath() + "/" LANGUAGE_DIR_NAME;
#elif defined Q_OS_MAC
    QString r = QApplication::applicationDirPath() + "/../" LANGUAGE_DIR_NAME;
#else
    QString r = "/usr/share/ndr-client/" LANGUAGE_DIR_NAME;
#endif
    */
    QString r = ":/translate/" LANGUAGE_DIR_NAME;
    return QFile::exists(r) ? r : QString();
}

QStringList __getLangFileNameTable() {
    static QString lang_dir = getLangDir();
    qDebug() << "lang_dir" << lang_dir;
    if (lang_dir.isEmpty())
        return QStringList();
    QDir dir(lang_dir);
    QStringList filter("ndr.*.qm");
    QStringList filelist =
        dir.entryList(filter, QDir::Files | QDir::CaseSensitive);
    qDebug() << "filelist" << filelist;
    if (filelist.isEmpty())
        return filelist;
    filelist.replaceInStrings("ndr.", QString());
    filelist.replaceInStrings(".qm", QString());
#if 0
	QStringList r;
	QString file_name;
	foreach(file_name, filelist) {
		qDebug() << "file_name" << file_name;
		QString lang_name;
		if(__getLanguageName(file_name, lang_name)) r << lang_name;
	}
	return r;
#else
    return filelist;
#endif
}

bool __getLanguageName(QString fileName, QString &langName) {
    static const struct {
        QString file_name;
        QString lang_name;
    } table[] = {{"en_US", "English"},
                 {"ja_JP", "日本語"},
                 {"ko_KR", "한국의"},
                 {"zh_CN", "简体中文"}};
    for (unsigned int i = 0; i < (sizeof table) / (sizeof *table); i++) {
        qDebug() << table[i].lang_name;
        if (fileName == table[i].file_name) {
            langName = table[i].lang_name;
            return true;
        }
    }
    return false;
}

QTranslator ndr_tr, qt_tr;
void __initLanguage() {
    QString lang_dir = getLangDir();

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

void __initStyleSheet(SingleApplication *a, QString sheetFile) {
    // QString sheetName = "default";
    QFile file(sheetFile);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
#if defined Q_OS_WIN || defined Q_OS_MAC
    QString appath = a->applicationDirPath();
#ifdef Q_OS_WIN
    styleSheet.replace("$[SKIN_PATH]", appath + "/skin");
#else
    styleSheet.replace("$[SKIN_PATH]", appath + "/../Resources/skin");
#endif
#else
    styleSheet.replace("$[SKIN_PATH]", "/usr/share/ndr-client/skin");
#endif
    qDebug() << styleSheet;
    // a->setStyleSheet(styleSheet);
    a->setStyleSheet(styleSheet);
}
