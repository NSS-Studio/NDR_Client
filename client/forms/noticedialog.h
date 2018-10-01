#ifndef NOTICEDIALOG_H
#define NOTICEDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QThread>

namespace Ui {
class NoticeDialog;
}

class NoticeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NoticeDialog(QWidget *parent = nullptr);
    ~NoticeDialog();
    void showMessage(QString msg);
protected:
    void mouseMoveEvent ( QMouseEvent  * e );
    void mousePressEvent ( QMouseEvent  * e );
    void mouseReleaseEvent ( QMouseEvent  * e );
    //void mouseDoubleClickEvent( QMouseEvent  * e );
private:
    Ui::NoticeDialog *ui;
    int x,y;
    bool isDown;
};

#endif // NOTICEDIALOG_H
