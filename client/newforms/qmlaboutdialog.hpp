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
    void btnLogin_clicked(const QString& username
                     ,const QString& passwd
                     ,const QString& pack_info
                     ,const QString& NIC_info);


private:
  QQmlApplicationEngine* engine; // save qml engine
  QQmlComponent* comp;
  QWindow* root; // save window pointer

  void bind_loginDialog_slot();
  void InitLoginDialog();
  void initMainWindow();
};

#endif // ABOUTDIALOG_HPP
