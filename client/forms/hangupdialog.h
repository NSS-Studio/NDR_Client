#ifndef HANGUPDIALOG_H
#define HANGUPDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QSharedPointer>

namespace Ui {
class HangupDialog;
}

class HangupDialog final: public QDialog
{
    Q_OBJECT
public:
    explicit HangupDialog(QWidget *parent = nullptr);
    ~HangupDialog();
    void waitForUserAccept();
	void setMessage(const QString &);
	void setAcceptButtonText(const QString &);
	void setDelayButtonText(const QString &);
private slots:
    void on_btnDelay_clicked();
    void on_btnAccept_clicked();

private:
    QSharedPointer<Ui::HangupDialog> ui;
    void closeEvent(QCloseEvent * event);
    bool event(QEvent * e);
    void timerEvent(QTimerEvent *);
    QEventLoop loop;
    int restTime;
    int timerId;
    bool close_now;
};

#endif // HANGUPDIALOG_H
