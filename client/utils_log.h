#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <QObject>

class utils_log : public QObject
{
    Q_OBJECT
public:
    explicit utils_log(QObject *parent = nullptr);

signals:

public slots:
};

#endif // UTILS_LOG_H