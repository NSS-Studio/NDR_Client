#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <utils.hpp>
SettingsDialog::SettingsDialog( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    //this->ui->checkBox_2->setEnabled(false);
	ui->cmbLanguage->setItemData(1, "zh_CN");
    //config = new QSettings(appHome + "/common.cfg", QSettings::IniFormat);
#ifdef Q_OS_WIN
    QObject::connect(ui->checkBox_2, &QCheckBox::clicked, [=](){
        if (!isRunAsAdmin()){
            message(dialogKinds::warning, "请先以管理员权限运行此程序");
            ui->checkBox_2->setChecked(!ui->checkBox_2->isChecked());
            return;
        }

        if (ui->checkBox_2->isChecked())
            ui->checkBox_2->setChecked(creatReg(autoStartRegRoot, autoSatrtKeyWay, autoStartkeyName, QDir::currentPath() + tr("/ndr-client.exe")));
        else
            ui->checkBox_2->setChecked(!delReg(autoStartRegRoot, autoSatrtKeyWay, autoStartkeyName));
    });
#endif
#ifdef Q_OS_MAC
	ui->cmbHotKey->clear();
	ui->cmbHotKey->addItem("⌘F9");
	ui->cmbHotKey->addItem("⌘F10");
	ui->cmbHotKey->addItem("⌘F11");
#endif
}

SettingsDialog::~SettingsDialog() {
}

void SettingsDialog::on_buttonBox_accepted()
{
	//config->setValue("Locale", ui->cmbLanguage->itemData(ui->cmbLanguage->currentIndex()));
	this->accept();
}

void SettingsDialog::on_SettingsDialog_finished(int /*result*/)
{
    qDebug() <<  "function: SD finished";
}

bool SettingsDialog::getFormData(SettingsSet *settings)
{
    this->ui->chkAutoRedial->setChecked(settings->autoRasdial);
    this->ui->cmbHotKey->hide();
    this->ui->label->hide();
    this->ui->label_5->hide();
    this->ui->comboBox->hide();

    this->ui->chkQuitWhileClose->setChecked(settings->quitWhileCloseWindow);
    this->ui->chkAutoMinimize->setChecked(settings->autoMinimize);
    this->ui->checkBox_2->setChecked(settings->autoStartup);
    this->ui->chkWebEnable->setChecked(settings->webUpEnable);
    this->ui->chkAutoClose->setChecked(settings->autoClose);

    QString savedLang = settings->language;
    if(savedLang.isEmpty()) ui->cmbLanguage->setCurrentIndex(0);	// Auto
    else if(savedLang == "zh_CN") ui->cmbLanguage->setCurrentIndex(1);
    QStringList langFileList = utils::getLangFileNameTable();

    if (ui->cmbLanguage->count() != 5){
        for (auto const& fileName: langFileList) {
            auto langName = utils::getLanguageName(fileName);
            if (langName.isNull()) continue;
            ui->cmbLanguage->addItem(langName.value<QString>(), fileName);
            if(fileName == savedLang) ui->cmbLanguage->setCurrentIndex(ui->cmbLanguage->count() - 1);
        }
    }

    if(this->exec()==QDialog::Accepted){
        settings->autoRasdial = this->ui->chkAutoRedial->isChecked();
        settings->quitWhileCloseWindow = this->ui->chkQuitWhileClose->isChecked();
        settings->autoStartup = this->ui->checkBox_2->isChecked();
        settings->autoMinimize = this->ui->chkAutoMinimize->isChecked();
        settings->webUpEnable = this->ui->chkWebEnable->isChecked();
        settings->autoClose = this->ui->chkAutoClose->isChecked();
        settings->language = this->ui->cmbLanguage->itemData(ui->cmbLanguage->currentIndex()).toString();
        return true;
    }
    return false;
}

bool SettingsDialog::chkWeb()
{
    return this->ui->chkWebEnable->isChecked();
}

void SettingsDialog::on_chkAutoClose_pressed()
{
    if (!this->ui->chkAutoClose->isChecked()){
        QMessageBox::StandardButton re = QMessageBox::information(nullptr, "警告", "勾选自动关闭通知有可能导致你无法看到来自学校的某些重要通知\n"
                                             "请再次确认是否关闭", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (re == QMessageBox::Yes)
            this->ui->chkAutoClose->setChecked(true);
    }
}

#ifdef Q_OS_WIN
bool SettingsDialog::isRunAsAdmin()
{
    BOOL bElevated = FALSE;
    HANDLE hToken = NULL;

    // Get current process token
    if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )
        return FALSE;

    TOKEN_ELEVATION tokenEle;
    DWORD dwRetLen = 0;

    // Retrieve token elevation information
    if ( GetTokenInformation( hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen ) )
    {
        if ( dwRetLen == sizeof(tokenEle) )
        {
            bElevated = tokenEle.TokenIsElevated;
        }
    }

    CloseHandle( hToken );
    return bElevated;
}

bool SettingsDialog::creatReg(HKEY proxyRoot, const QString &keyWay, const QString &keyName, const QString &keyValue)
{
    HKEY hKey;
    REGSAM flag = KEY_CREATE_SUB_KEY | KEY_READ | KEY_WRITE | KEY_SET_VALUE;

    LONG ret = RegOpenKeyEx(proxyRoot, LPCTSTR(keyWay.unicode()), 0, KEY_WOW64_64KEY | flag, &hKey);
    if (ret != ERROR_SUCCESS)
        ret = RegOpenKeyEx(proxyRoot, LPCTSTR(keyWay.unicode()), 0, KEY_WOW64_32KEY | flag, &hKey);
    if (ret != ERROR_SUCCESS){
        qDebug() << "Open reg failed: " << ret;
        message(dialogKinds::warning, "设置开机自启动失败，错误代码: " + QString::number(ret, 10));
        return false;
    }

#ifdef UNICODE
    const QChar *pchrName = keyValue.unicode();
#elif
    const unsigned char *pchrName = keyValue.toStdString().c_str();
#endif

    ret = RegSetValueEx(hKey, LPCTSTR(keyName.unicode()), 0, REG_SZ, LPBYTE(pchrName), wcslen((TCHAR*)pchrName)*sizeof(TCHAR)+1); //设置注册表项
    if (ret != ERROR_SUCCESS)
    {
        qDebug() << "Set value failed: " << ret;
        message(dialogKinds::warning, "设置开机自启动失败，错误代码: " + QString::number(ret, 10));
        return false;
    }

    RegCloseKey(hKey); //与RegCreateKeyEx配对写
    message(dialogKinds::stander, "设置开机自启动成功");
    return true;
}

bool SettingsDialog::delReg(HKEY proxyRoot, const QString &keyWay, const QString &keyName)
{
    HKEY hKey;
    REGSAM flag =  KEY_WRITE;

    bool is64BitSys = true;
    LONG ret = RegOpenKeyEx(proxyRoot, LPCTSTR(keyWay.unicode()), 0, KEY_WOW64_64KEY | flag, &hKey);
    if (ret != ERROR_SUCCESS){
        is64BitSys = false;
        ret = RegOpenKeyEx(proxyRoot, LPCTSTR(keyWay.unicode()), 0, KEY_WOW64_32KEY | flag, &hKey);
    }
    qDebug() << "del.Open reg return: " << ret;
    if (ret != ERROR_SUCCESS){
        qDebug() << "del.Open reg failed: " << ret;
        message(dialogKinds::warning, "取消开机自启动失败，错误代码: " + QString::number(ret, 10));
        return false;
    }

    if (is64BitSys)
        flag = KEY_WOW64_64KEY;
    else
        flag = KEY_WOW64_32KEY;
    qDebug() << "is64BitSys: " << is64BitSys;
    ret = RegDeleteValue(hKey, LPCTSTR(keyName.unicode()));
    if (ret != ERROR_SUCCESS){
        qDebug() << "Delete key failed: " << ret;
        message(dialogKinds::warning, "取消开机自启动失败，错误代码: " + QString::number(ret, 10));
        return false;
    }

    RegCloseKey(hKey); //与RegCreateKeyEx配对写
    message(dialogKinds::stander,"取消开机自启动成功");
    return true;
}

QMessageBox::StandardButton SettingsDialog::message(dialogKinds question, const QString &text)
{
    QMessageBox::StandardButton reply;
    switch (question){
    case dialogKinds::stander:
        QMessageBox::information(this, tr("Information"), text);
        break;

    case dialogKinds::question:
        reply = QMessageBox::question(this, tr("Question"),
                                            text,
                                            QMessageBox::Yes | QMessageBox::No);
        break;

    case dialogKinds::warning:
        QMessageBox::warning(this, tr("Warning"), text);
        break;

    case dialogKinds::error:
        reply = QMessageBox::critical(this, tr("Error"),
                                            text,
                                            QMessageBox::Abort | QMessageBox::Retry | QMessageBox::Ignore);
        break;
    }
    return reply;
}
#endif
