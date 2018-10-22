#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QSharedPointer>
namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();
private slots:
    void on_AboutDialog_finished(int /*result*/);
    void on_pushButton_clicked();
private:
    QSharedPointer<Ui::AboutDialog> ui;
    void closeEvent(QCloseEvent *event);
};

#endif // ABOUTDIALOG_H
