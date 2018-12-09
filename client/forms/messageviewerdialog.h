#ifndef MESSAGEVIEWERDIALOG_H
#define MESSAGEVIEWERDIALOG_H

#include <QDialog>
#include <QSharedPointer>
namespace Ui {
class MessageViewerDialog;
}

class MessageViewerDialog final: public QDialog
{
    Q_OBJECT
public:
    explicit MessageViewerDialog(QWidget *parent = nullptr);
    ~MessageViewerDialog() = default;
private:
    QSharedPointer<Ui::MessageViewerDialog> ui;
};

#endif // MESSAGEVIEWERDIALOG_H
