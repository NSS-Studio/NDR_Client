#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include <QVector>
#include <assert.h>

LoginDialog::LoginDialog(LocalStorage *profile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
	Qt::WindowFlags flags=Qt::Dialog;
	flags |= Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
	ui->setupUi(this);
#ifndef Q_OS_MAC
	int edit_height = ui->cmbAccount->height();
	qDebug("edit_height = %d", edit_height);
	ui->editPassword->setMaximumHeight(edit_height);
	ui->btnDelUserInfo->setMaximumHeight(edit_height);
#endif
	//this->ui->chkAutoLogin->setEnabled(false);
	this->profile = profile;

	this->ui->cmbModel->clear();
	QStringList captionList;
	QStringList postfitList;
	postfitList = __getDrModelPostfixTable();
    
	foreach(QString postfit, postfitList)
	{
		qDebug() << postfit;
		QString caption;
		__getDrModelCaption(postfit,caption);
		this->ui->cmbModel->addItem(caption);
	}

	QCompleter *completer = new QCompleter(this);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	ui->cmbAccount->setCompleter(completer);
	this->setWindowTitle("[" + __getVersionString() +"] " + this->windowTitle() );
#ifdef Q_OS_WIN
	ui->comboIface->setEnabled(false);
#endif
    /*
    int desktop_width = QApplication::desktop()->width();
    int desktop_height = QApplication::desktop()->height();
    this->move((desktop_width-this->width())/2,(desktop_height-this->height())/2-200);
    */
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
/*
bool LoginDialog::show(QString &username,QString password,int &type)
{
    if(this->exec()==QDialog::Accepted)
    {
        username=this->ui->cmbAccount->currentText();
        password=this->ui->editPassword->text();
        type = this->ui->comboBox->currentIndex();
        return true;
    }else
    {
        return false;
    }
}
*/
void LoginDialog::getFormData(QString &username, QString &password, QString &model, QString &device_name, bool *autoLogin,bool *savePassword)
{
    username=this->ui->cmbAccount->currentText();
    password=this->ui->editPassword->text();
    device_name = this->ui->comboIface->currentText();
    //model = this->ui->cmbModel->currentText();
    int index = ui->cmbModel->currentIndex();
    qDebug("current index: %d", index);
    assert(index >= 0);
    model=__getDrModelPostfixTable().at(index);
    if(autoLogin)
        *autoLogin = this->ui->chkAutoLogin->isChecked();
    if(savePassword)
        *savePassword = this->ui->chkSavePasswd->isChecked();
}

void LoginDialog::on_btnLogin_clicked()
{
    if(this->ui->cmbAccount->currentText().isEmpty())
    {
        QMessageBox::information(this,tr("提示"),tr("账号不能为空"));
        this->ui->cmbAccount->setFocus();
        return;
    }
    if(this->ui->editPassword->text().isEmpty())
    {
        QMessageBox::information(this,tr("提示"),tr("密码不能为空"));
        this->ui->editPassword->setFocus();
        return;
    }
    
    //this->accept();
    emit myaccepted();
    this->hide();
    
}

/*
void LoginDialog::on_LoginDialog_accepted()
{

}

void LoginDialog::on_LoginDialog_rejected()
{
    //qApp->exit(0);
}
*/

void LoginDialog::on_cmbAccount_currentIndexChanged(const QString &arg1)
{


}

void LoginDialog::on_btnDelUserInfo_clicked()
{
    if(profile->open())
    {
        int index;
        index=ui->cmbAccount->findText(ui->cmbAccount->currentText());
        if(index>=0)
        {
            if(QMessageBox::question(this,tr("提示"),
                                  QString(("确定立即删除账号 %1 的所有数据吗？"))
                                  .arg(ui->cmbAccount->currentText()),
                                  QMessageBox::Ok,QMessageBox::Cancel)==QMessageBox::Ok)
            {
                if(profile->removeLoginInfo(ui->cmbAccount->itemText(index)))
                {
                    qDebug() << "Remove succeed";
                    this->ui->cmbAccount->removeItem(index);
                    QMessageBox::information(this,tr("提示"),tr("删除成功  "));
                }else
                    qDebug() << "Remove failed";
            }

        }else
        {
            QMessageBox::information(this,tr("提示"),tr("本地不存在该账号的任何信息"));
        }
        /*
        if(profile->removeLoginInfo(this->ui->cmbAccount->itemText(index)))
        {
            this->ui->cmbAccount->removeItem(index);
        }*/
        profile->close();
    }
}

void LoginDialog::on_cmbAccount_currentIndexChanged(int index)
{
    //qDebug() << "index" << index;
}

void LoginDialog::show()
{
    QStringList list;

    
    if(profile->open())
    {
        QString preUsername,prePassword;
	QString saved_device_name;
        bool needSave;
        qDebug() << "db opened";
        
        list = QStringList::fromVector(profile->getUserNameList());
        preUsername = this->ui->cmbAccount->currentText();
        prePassword = this->ui->editPassword->text();
        needSave = this->ui->chkSavePasswd->isChecked();
        qDebug() << preUsername << prePassword;
        qDebug() << "不科学";
        this->ui->cmbAccount->clear();
        this->ui->cmbAccount->addItems(list);
        qDebug() << "不科学";
        //this->ui->cmbAccount->setEditText(preUsername);
        //this->ui->editPassword->setText(prePassword);
	if(profile->getDeviceName(saved_device_name)) {
		// Deprecated
		qDebug() << "saved_device_name" << saved_device_name;
		set_device_name(saved_device_name);
	} else {
		QSettings conn_cfg(appHome + "/connection.cfg", QSettings::IniFormat);
		saved_device_name = conn_cfg.value("Interface/Etherface").toString();
		if(!saved_device_name.isEmpty()) set_device_name(saved_device_name);
	}
        if(profile->getAutoLoginUser(autoLoginUser)
                && !autoLoginUser.isEmpty() 
                && preUsername.isEmpty())
        {
            qDebug() << "auto login user" << autoLoginUser;
            this->ui->cmbAccount->setEditText(autoLoginUser);
            qDebug() << "password" << this->ui->editPassword->text();
            if(!this->ui->editPassword->text().isEmpty())
	    {
                this->ui->btnLogin->click();
                profile->close();
                return;
            }
        }
        this->QDialog::show();
        if(preUsername.isEmpty())
        {
            //QString autoLoginUser;
            qDebug() << "preUsername" << preUsername;
            
            
            
            QString lastLoginUsername;
            if(profile->getLastLoginUser(lastLoginUsername) && !lastLoginUsername.isEmpty())
            {
                
                qDebug() << "last login user" << lastLoginUsername;
                int index;
                index = ui->cmbAccount->findText(lastLoginUsername);
                this->ui->cmbAccount->setCurrentIndex(index>=0?index:0);
		this->ui->editPassword->setFocus();
            }
        }else
        {
            this->ui->cmbAccount->setEditText(preUsername);
            this->ui->editPassword->setText(prePassword);
            this->ui->chkSavePasswd->setChecked(needSave);
	    this->ui->editPassword->setFocus();
	    //this->ui->editPassword->setFocus();
        }

        profile->close();
    }else
    {
        this->QDialog::show();
    }
    
}

void LoginDialog::on_cmbAccount_editTextChanged(const QString &arg1)
{
    if (arg1.length()>1)
    {
        if(profile->open())
        {
            
            QString password;
            QString manner;
            int index;
            if(profile->getLoginInfo(arg1,password,manner))
            {
                this->ui->chkSavePasswd->setChecked(!password.isEmpty());
                this->ui->editPassword->setText(password);
                index =__getDrModelPostfixTable().indexOf(manner);
                index=(index<0)?0:index;
                this->ui->cmbModel->setCurrentIndex(index);

                //this->ui->chkAutoLogin->setChecked(!password.isEmpty()&&password==arg1);

                this->ui->chkAutoLogin->setChecked(arg1 == this->autoLoginUser);

            }
            profile->close();
        }
    }
}

void LoginDialog::on_chkAutoLogin_clicked(bool checked)
{
	if(checked)
		this->ui->chkSavePasswd->setChecked(true);
}

void LoginDialog::on_chkSavePasswd_clicked(bool checked)
{
    if(checked==false)
        this->ui->chkAutoLogin->setChecked(false);
}

void LoginDialog::handle_model_change(int index) {
	qDebug("slot: handle_model_change(%d)", index);
	if(index < 0) ui->cmbModel->setCurrentIndex(0);
}

void LoginDialog::set_interface_list(const QStringList &list) {
	ui->comboIface->addItems(list);
}

void LoginDialog::closeEvent(QCloseEvent *e) {
	qDebug("function: LoginDialog::closeEvent(%p)", e);
	qApp->exit();
	e->accept();
}

void LoginDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
    {
	qApp->quit();
    }
}

void LoginDialog::set_device_name(const QString &name) {
	int index = ui->comboIface->findText(name, Qt::MatchExactly);
	if(index >= 0) ui->comboIface->setCurrentIndex(index);
}

void LoginDialog::on_btnShowPassword_clicked(bool checked)
{
    if(checked) {
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    }else{
        ui->editPassword->setEchoMode(QLineEdit::Password);
    }
}
