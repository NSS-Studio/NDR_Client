#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
//点击登陆，发送了一个自定义的消息，应该改为this->accept();
//同时修改mainwindow 构造函数中的消息绑定
#include <QDialog>
#include <QMessageBox>
#include "localstorage.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
	explicit LoginDialog(LocalStorage *profile,QWidget *parent = 0);
	~LoginDialog();
	void getFormData(QString &username, QString &password, QString &type, QString &device_name, bool *autoLogin=0, bool *savePassword=0);
	//bool show(QString &username,QString password, int &type);
	void show();
	void set_interface_list(const QStringList &);

protected:
	void keyPressEvent(QKeyEvent *);
    
private slots:
	void on_btnLogin_clicked();
    //void on_cmbAccount_currentIndexChanged(const QString &arg1);
	void on_btnDelUserInfo_clicked();
    //void on_cmbAccount_currentIndexChanged(int index);
	void on_cmbAccount_editTextChanged(const QString &arg1);
	void on_chkAutoLogin_clicked(bool checked);
	void on_chkSavePasswd_clicked(bool checked);
	void handle_model_change(int);

    void on_btnShowPassword_clicked(bool checked);

private:
	Ui::LoginDialog *ui;
	LocalStorage *profile;
	QString autoLoginUser;
	void set_device_name(const QString &);
	void closeEvent(QCloseEvent * e);

signals:
	void myaccepted();
};

#endif // LOGINDIALOG_H
