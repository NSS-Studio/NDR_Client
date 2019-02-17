#ifndef ABOUTDIALOG_HPP
#define ABOUTDIALOG_HPP

#include <QObject>
#include <QQuickItem>
#include <QQuickView>
#include <QQmlApplicationEngine>

class QMLAboutDialog : public QObject {
  Q_OBJECT
public:

  explicit QMLAboutDialog(QObject *parent = nullptr);

  ~QMLAboutDialog(){
      delete comp;
  }

signals:

public slots:

private:
  QQmlApplicationEngine* engine; // save qml engine
  QQmlComponent* comp;
  QWindow* root; // save window pointer
};

#endif // ABOUTDIALOG_HPP
