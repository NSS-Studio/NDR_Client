#include "aboutdialog.hpp"
#include "ui_aboutdialog.h"
#include <utils.hpp>
AboutDialog::AboutDialog(QWidget *parent)
    : QDialog{parent}, ui{new Ui::AboutDialog} {
  setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
  ui->setupUi(this);
  setFixedSize(this->width(), this->height());
  this->ui->lblVersion->setText(" " + tr("版本") + " " +
                                utils::getVersionString());
  this->ui->lblBuildTime->setText(tr("构建于") + " " + COMPILE_DATA_TIME);
}

void AboutDialog::on_AboutDialog_finished(int /*result*/) {}

void AboutDialog::on_pushButton_clicked() { this->hide(); }

void AboutDialog::closeEvent(QCloseEvent *event) {
  event->ignore();
  this->hide();
}
