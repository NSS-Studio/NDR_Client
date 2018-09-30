#include "utils.hpp"

namespace utils {

    QMap<QString, QString> const& getDrModelId() {
        static QMap<QString, QString> const drModelId = {
            {QString{QObject::tr("考试专用")}, QString{""}},
            {QString{QObject::tr("教育网2M")}, QString{"@edu"}},
            {QString{QObject::tr("联通合约20M")}, QString{"@cnc"}},
            {QString{QObject::tr("联通合约30M")}, QString{"@cnc30a"}},
            {QString{QObject::tr("联通合约50M")}, QString{"@cnc50a"}},
            {QString{QObject::tr("电信10M")}, QString{"@ct10"}},
            {QString{QObject::tr("电信合约10M")}, QString{"@ct10a"}}
        };
        return drModelId;
    }

    QStringList getDrModelPostfixTable() {
        return getDrModelId().keys();
    }

    QString getDrModel(QString const& postfix) {
        return getDrModelId()[postfix];
    }

    bool getDrModelCaption(QString postfix, QString &caption) {
        qDebug() << "Function :"<< __PRETTY_FUNCTION__ << "postfix:" << postfix;
        auto drModelId = getDrModelId();
        if (drModelId.find(postfix) == drModelId.cend()) {
            return false;
        }
        caption = postfix;
        return true;
    }

    QString getLangDir() {
        qDebug() << "Function :"<< __PRETTY_FUNCTION__;
        QString r = ":/translate/" LANGUAGE_DIR_NAME;
        return QFile::exists(r) ? r : QString();
    }

    QString getVersionString() {
        quint8 majorVersion = VERSION_MAJOR;
        quint8 minorVersion = VERSION_MINOR;
        Q_ASSERT(minorVersion < 100);
        return QString("%1").arg(majorVersion + minorVersion / 100.0, 0, 'f', 2);
    }

    QStringList getLangFileNameTable() {
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

    bool getLanguageName(QString fileName, QString &langName) {
        QMap<QString, QString> table {
            {"en_US", "English"},
            {"ja_JP", "日本語"},
            {"ko_KR", "한국의"},
            {"zh_CN", "简体中文"}
        };
        qDebug() << "Function :"<< __PRETTY_FUNCTION__;
        if (table.find(fileName) == table.cend()) {
            return false;
        }
        langName = table[fileName];
        return true;
    }
}

