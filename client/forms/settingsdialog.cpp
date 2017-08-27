#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "common.h"

SettingsDialog::SettingsDialog( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->setupUi(this);

    this->ui->checkBox_2->setEnabled(false);
    this->ui->checkBox_3->setEnabled(false);
	ui->cmbLanguage->setItemData(1, "zh_CN");
    //config = new QSettings(appHome + "/common.cfg", QSettings::IniFormat);
#ifdef Q_OS_MAC
	ui->cmbHotKey->clear();
	ui->cmbHotKey->addItem("⌘F9");
	ui->cmbHotKey->addItem("⌘F10");
	ui->cmbHotKey->addItem("⌘F11");
#endif
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "settingDialog delete";
	delete ui;
//	delete config;
}

void SettingsDialog::on_buttonBox_accepted()
{
	//config->setValue("Locale", ui->cmbLanguage->itemData(ui->cmbLanguage->currentIndex()));
	this->accept();
}

void SettingsDialog::on_SettingsDialog_finished(int /*result*/)
{
    qDebug() <<  "function: SD finished";
//  死循环
//  this->reject();
}

bool SettingsDialog::getFormData(SettingsSet *settings)
{
    //settings.autoRasdial = this->ui->chk;
    this->ui->chkAutoRedial->setChecked(settings->autoRasdial);
//    this->ui->cmbHotKey->setEditText(settings->hotkey);
//    qDebug() << "hotkey" << settings->hotkey;
    this->ui->cmbHotKey->hide();
    this->ui->label->hide();
    this->ui->label_5->hide();
    this->ui->comboBox->hide();

    this->ui->chkQuitWhileClose->setChecked(settings->quitWhileCloseWindow);
    this->ui->chkAutoMinimize->setChecked(settings->autoMinimize);
    this->ui->chkWebEnable->setChecked(settings->webUpEnable);
    this->ui->chkAutoClose->setChecked(settings->autoClose);

	QString saved_lang = settings->language;
	if(saved_lang.isEmpty()) ui->cmbLanguage->setCurrentIndex(0);	// Auto
	else if(saved_lang == "zh_CN") ui->cmbLanguage->setCurrentIndex(1);
	QStringList lang_file_list = __getLangFileNameTable();
	//ui->cmbLanguage->addItems(lang_file_list);

    if (ui->cmbLanguage->count() != 5){
        QString file_name;
        foreach(file_name, lang_file_list) {
            QString lang_name;
            if(!__getLanguageName(file_name, lang_name)) continue;
            ui->cmbLanguage->addItem(lang_name, file_name);
            if(file_name == saved_lang) ui->cmbLanguage->setCurrentIndex(ui->cmbLanguage->count() - 1);
        }
    }

    if(this->exec()==QDialog::Accepted){
        settings->autoRasdial = this->ui->chkAutoRedial->isChecked();
//        settings->hotkey = this->ui->cmbHotKey->currentText().trimmed();
        settings->quitWhileCloseWindow = this->ui->chkQuitWhileClose->isChecked();
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
        QMessageBox::StandardButton re = QMessageBox::information(NULL, "警告", "勾选自动关闭通知有可能导致你无法看到来自学校的某些重要通知\n"
                                             "请再次确认是否关闭", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (re == QMessageBox::Yes)
            this->ui->chkAutoClose->setChecked(true);
    }
}
