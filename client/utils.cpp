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
}

