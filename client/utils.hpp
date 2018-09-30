#ifndef UTILS_HPP
#define UTILS_HPP

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <ndrconfig.hpp>
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
}

#endif // UTILS_HPP
