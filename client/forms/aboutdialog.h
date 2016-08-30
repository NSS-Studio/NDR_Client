#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "common.h"
#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    
private slots:
    void on_AboutDialog_finished(int result);
    
    void on_pushButton_clicked();
    
private:
    Ui::AboutDialog *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // ABOUTDIALOG_H
