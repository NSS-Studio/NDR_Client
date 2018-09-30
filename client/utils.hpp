#ifndef UTILS_HPP
#define UTILS_HPP

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QFile>

/** 语言存放文件夹*/
#define LANGUAGE_DIR_NAME "lang"

namespace utils {
    /** 获取账户名套餐后缀列表*/
    QStringList getDrModelPostfixTable();

    /** 根据套餐后缀获取套餐描述*/
    bool getDrModelCaption(QString postfix, QString &caption);

    QString getDrModel(QString const& postfix);

    /** 获取语言所在路径，失败返回空字符串*/
    QString getLangDir();
}

#endif // UTILS_HPP
