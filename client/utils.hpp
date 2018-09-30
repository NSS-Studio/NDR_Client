#ifndef UTILS_HPP
#define UTILS_HPP
#include <ndrconfig.hpp>
#include <QObject>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTranslator>
#include <QApplication>
#include <QLibraryInfo>
#include <settingsset.h>

namespace utils {
    /** 获取账户名套餐后缀列表*/
    QStringList getDrModelPostfixTable();

    /** 根据套餐后缀获取套餐描述*/
    bool getDrModelCaption(QString postfix, QString &caption);

    QString getDrModel(QString const& postfix);

    /** 获取语言所在路径，失败返回空字符串*/
    QString getLangDir();

    QString getVersionString();

    QStringList getLangFileNameTable();

    /**
     * @brief getLanguageName 根据文件名获取语言名称
     * @param fileName 文件名
     * @param langName 待返回的语言名称字符串
     * @return 成功/失败
     */
    bool getLanguageName(QString fileName, QString &langName);

    extern QString appHome; //家目录
    void initAppHome();

    extern QString tempDir; //临时文件夹
    void initTempDir();

    /** 设置项*/
    extern QSharedPointer<SettingsSet> settings;
    void initSettingsSet();
    extern QTranslator ndr_tr, qt_tr;
    void initLanguage();
}

#endif // UTILS_HPP
