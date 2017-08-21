#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QSet>
#include <QLabel>
#include <QString>
#include <QTextBrowser>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslConfiguration>
#include <QUrl>
#include <QDomDocument>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QCryptographicHash>
#include <QByteArray>

class popUpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit popUpDialog(QWidget *parent = nullptr);
    ~popUpDialog();

    void getXmlFromNSS(const QString &url);

private slots:
    void getXmlDone();
    void goNextPage();
    void goPrePage();

private:
    QDialog *popUp;
    QLabel *laTitle;
    QLabel *laAuth;
    QLabel *page;
    QLineEdit *liAuth;
    QLineEdit *liTitle;
    QTextBrowser *teText;
    QPushButton *nextPage;
    QPushButton *prePage;

    QHBoxLayout *hbAuth;
    QHBoxLayout *hbTitle;
    QHBoxLayout *hbPage;

    QVBoxLayout *mainLay;

    QSslConfiguration sslConf;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QDesktopWidget desktop;

    struct message{
        bool isShow;
        QString title;
        QString auth;
        QString text;
    };
    QVector<message> group;
    QSet<QByteArray> md5;
    QCryptographicHash getMd5{QCryptographicHash::Md5};
    QByteArray tempA, tempB;
    int pageNow;

    void showMessage();
};

#endif // POPUPDIALOG_H
