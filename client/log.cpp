#include "log.h"
#include <stdint.h>

QString Log::filename = "";
QMutex Log::mutex;



void Log::setLogFilename(QString filename)
{
    Log::filename = filename;
    //Log::mutex.unlock();
}

void Log::write(QString /*msg*/,char */*byteArray*/, int /*size*/)
{
    /*
    QString buf=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") + " " ;
    buf += msg + " ";
    if(byteArray)
    {
        buf += QString("[%1]").arg(size,5,10,QChar(' '));
        for(int i=0;i<size;i++)
        {
            buf.append((QString::number((uint8_t)byteArray[i]/16,16)+QString::number((uint8_t)byteArray[i]%16,16)).toUpper());
        }
    }
    if(!filename.isEmpty())
    {
        QFile logFile(filename);
        
        if(logFile.open(QIODevice::WriteOnly|QIODevice::Append))
        {
            
            QTextStream out(&logFile);
            //QTextStream out_stdout(stdout);
            mutex.lock();
            out << buf << endl;
            mutex.unlock();
            logFile.close();
        }
    }
    */

    
}
