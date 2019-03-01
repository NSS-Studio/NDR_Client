#include <qapplication.h>
#include <windows.h>
#include <iostream>

namespace utils {
void messageHandler(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // https://ss64.com/nt/color.html
  // color your text in Windows console mode
  // colors are 0=black 1=blue 2=green and so on to 15=white
  // colorattribute = foreground + background * 16
  // to get red text on yellow use 4 + 14*16 = 228
  // light red on yellow would be 12 + 14*16 = 236
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
    std::cerr << QString{"Warning: %1 \n(%2:%3, %4)"}.arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString() << std::endl;
    break;
  case QtCriticalMsg:
    SetConsoleTextAttribute(hConsole, 0x0C);
    std::cerr << QString{"Critical: %1 \n(%2:%3, %4)"}.arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString() << std::endl;
    break;
  case QtFatalMsg:
    SetConsoleTextAttribute(hConsole, 0xC0);
    std::cerr << QString{"Fatal: %1 \n(%2:%3, %4)"}.arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString() << std::endl;
    break;
  }
}
} // namespace utils
