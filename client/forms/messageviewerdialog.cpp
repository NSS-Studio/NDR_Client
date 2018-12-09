#include "messageviewerdialog.h"
#include "ui_messageviewerdialog.h"

MessageViewerDialog::MessageViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageViewerDialog)
{
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());
}
