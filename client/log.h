#ifndef LOG_H
#define LOG_H
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QDateTime>
class Log
{
public:

    static void write(QString msg,char * /*byteArray*/ =0,int /*size*/=0);
    static void setLogFilename(QString filename);
private:
    static QString filename;
    static QMutex mutex;
};

#endif // LOG_H
