#include "qmlaboutdialog.hpp"
#include <QUrl>

QMLAboutDialog::QMLAboutDialog(QObject *parent) : QObject(parent)
{
    view.setSource(QUrl{"qrc:/qmlforms/test.qml"});
    view.show();
}
