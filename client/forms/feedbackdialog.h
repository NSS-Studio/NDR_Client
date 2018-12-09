#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H
#include <QDialog>
#include <QMessageBox>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QDesktopServices>
#include <QDomDocument>
#include <QSslCertificate>
#include <QSharedPointer>

namespace Ui {
	class FeedbackDialog;
}

class FeedbackDialog final: public QDialog
{
	Q_OBJECT
public:
    explicit FeedbackDialog(QWidget *parent = nullptr);
    ~FeedbackDialog() = default;
	void setLoginData(QString account,QString combo);
	void show();
private slots:
	void on_sldScore_sliderMoved(int position);
	void on_chkToScore_clicked(bool checked);
	void on_btnSubmit_clicked();
	void postFinished();
private:
    QSharedPointer<Ui::FeedbackDialog> ui;
    QSslConfiguration sslConf;
	QNetworkAccessManager nam;
	QNetworkRequest request;
    QNetworkReply *reply{};
	QString account;
	QString combo;
};

#endif // FEEDBACKDIALOG_H
