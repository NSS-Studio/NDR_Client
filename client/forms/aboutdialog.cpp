#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    this->ui->lblVersion->setText(" " + tr("版本") + " " + __getVersionString());
    this->ui->lblBuildTime->setText(tr("构建于")+ " " + COMPL_DATATIME);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_AboutDialog_finished(int /*result*/)
{
    
}

void AboutDialog::on_pushButton_clicked()
{
    //this->reject();
    this->hide();
}

void AboutDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
