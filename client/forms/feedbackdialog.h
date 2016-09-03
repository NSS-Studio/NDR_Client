#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H
#include "common.h"
#include <QDialog>
#include <QMessageBox>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QDesktopServices>
#include <QDomDocument>

namespace Ui {
	class FeedbackDialog;
}

class FeedbackDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FeedbackDialog(QWidget *parent = 0);
	~FeedbackDialog();
	void setLoginData(QString account,QString combo);
	void show();

private slots:
	void on_sldScore_sliderMoved(int position);
	void on_chkToScore_clicked(bool checked);
	void on_btnSubmit_clicked();
	void postFinished();

private:
	Ui::FeedbackDialog *ui;
	QNetworkAccessManager nam;
	QNetworkRequest request;
	QNetworkReply *reply;
	QString account;
	QString combo;
};

#endif // FEEDBACKDIALOG_H
