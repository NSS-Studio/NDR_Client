#ifndef ABOUTDIALOG_HPP
#define ABOUTDIALOG_HPP

#include <QObject>
#include <QQuickItem>
#include <QQuickView>

class QMLAboutDialog : public QObject
{
    Q_OBJECT
public:
    explicit QMLAboutDialog(QObject *parent = nullptr);

signals:

public slots:

private:
    QQuickView view;
};

#endif // ABOUTDIALOG_HPP
