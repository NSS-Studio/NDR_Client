#include "qmlaboutdialog.hpp"
#include <QUrl>
#include <QQuickView>

QMLAboutDialog::QMLAboutDialog(QObject *parent) : QObject(parent) {
  view.setSource(QUrl{"qrc:/qmlforms/mainWIndows.qml"});
  view.show();
}

QQuickView& QMLAboutDialog::getView()
{
    return this -> view;
}
