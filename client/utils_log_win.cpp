#include <qapplication.h>
#include <windows.h>
#include <iostream>

namespace utils {
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// https://ss64.com/nt/color.html
// color your text in Windows console mode
// colors are 0=black 1=blue 2=green and so on to 15=white
// colorattribute = foreground + background * 16
// to get red text on yellow use 4 + 14*16 = 228
// light red on yellow would be 12 + 14*16 = 236
void messageHandler(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  switch (type) {
  case QtDebugMsg:
    SetConsoleTextAttribute(hConsole, 0x0E);
    std::cerr << QString{"Debug: %1"}.arg(msg).toStdString() << std::endl;
    break;
  case QtInfoMsg:
    SetConsoleTextAttribute(hConsole, 0x0F);
    std::cerr << QString{"Info: %1"}.arg(msg).toStdString() << std::endl;
    break;
  case QtWarningMsg:
    SetConsoleTextAttribute(hConsole, 0xE0);

    fprintf(stderr, "Warning: %s \n(%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtCriticalMsg:
    SetConsoleTextAttribute(hConsole, 0x0C);
    fprintf(stderr, "Critical: %s \n(%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtFatalMsg:
    SetConsoleTextAttribute(hConsole, 0xC0);
    fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  }
}
} // namespace utils
