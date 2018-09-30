#include "common.hpp"
#include <QtCore/QDebug>

QString networkInterfaceCardName;



void __initFont(QString fontFile) {
    int fontId = QFontDatabase::addApplicationFont(fontFile);

    qDebug("font id:%d\n", fontId);
    if (fontId >= 0) {
        QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(msyh, 10);
        //将此字体设为QApplication的默认字体
        QApplication::setFont(font);
    }
}

//void __initStyleSheet(NdrApplication *a, QString sheetFile) {
//    // QString sheetName = "default";
//    QFile file(sheetFile);
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());
//#if defined Q_OS_WIN || defined Q_OS_MAC
//    QString appath = a->applicationDirPath();
//#ifdef Q_OS_WIN
//    styleSheet.replace("$[SKIN_PATH]", appath + "/skin");
//#else
//    styleSheet.replace("$[SKIN_PATH]", appath + "/../Resources/skin");
//#endif
//#else
//    styleSheet.replace("$[SKIN_PATH]", "/usr/share/ndr-client/skin");
//#endif
//    qDebug() << styleSheet;
//    // a->setStyleSheet(styleSheet);
//    a->setStyleSheet(styleSheet);
//}
