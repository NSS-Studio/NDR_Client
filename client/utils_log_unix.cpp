#include <stdio.h>
#include <qapplication.h>

#define RED   "\x1B[31m"
#define BRED  "\x1B[91m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BYEL  "\x1B[93m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

namespace utils {
	void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, YEL "Debug:" RESET " %s " GRN "(%s:%u, %s)\n" RESET, localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtInfoMsg:
            fprintf(stderr, WHT "Info:" RESET " %s " GRN "(%s:%u, %s)\n" RESET, localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, BYEL "Warning:" RESET " %s " GRN "(%s:%u, %s)\n" RESET, localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, BRED "Critical: " RESET " %s " GRN "(%s:%u, %s)\n" RESET, localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, RED "Fatal:" RESET " %s " GRN "(%s:%u, %s)\n" RESET, localMsg.constData(), context.file, context.line, context.function);
            break;
        }
    }
}
