#ifndef LOGINDIALOG_HPP
#define LOGINDIALOG_HPP
//点击登陆，发送了一个自定义的消息，应该改为this->accept();
//同时修改mainwindow 构造函数中的消息绑定
#include "localstorage.hpp"
#include "resourcemanager.hpp"
#include <QDialog>
#include <QMessageBox>
#include <QSharedPointer>

namespace Ui {
class LoginDialog;
}

class LoginDialog final : public QDialog {
  Q_OBJECT

public:
  explicit LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog() = default;
  void getFormData(QString &username, QString &password, QString &type,
                   QString &device_name, bool *autoLogin = nullptr,
                   bool *savePassword = nullptr);
  // bool show(QString &username,QString password, int &type);
  void show();
  void set_interface_list(const QStringList &);

protected:
  void keyPressEvent(QKeyEvent *);

private slots:
  void on_btnLogin_clicked();
  // void on_cmbAccount_currentIndexChanged(const QString &arg1);
  void on_btnDelUserInfo_clicked();
  // void on_cmbAccount_currentIndexChanged(int index);
  void on_cmbAccount_editTextChanged(const QString &arg1);
  void on_chkAutoLogin_clicked(bool checked);
  void on_chkSavePasswd_clicked(bool checked);
  void handle_model_change(int);
  void on_btnShowPassword_clicked(bool checked);

#ifdef Q_OS_WIN
  void on_btnWinsockReset_clicked();
#endif
private:
  QSharedPointer<Ui::LoginDialog> ui;
  QString autoLoginUser;
  void set_device_name(const QString &);
  void closeEvent(QCloseEvent *e);
signals:
  void myaccepted();
};

#endif // LOGINDIALOG_HPP
