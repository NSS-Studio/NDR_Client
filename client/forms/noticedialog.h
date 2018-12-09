#ifndef NOTICEDIALOG_H
#define NOTICEDIALOG_H

#include <QDialog>
#include <QSharedPointer>
#include <QThread>
#include <QtGui>
namespace Ui {
class NoticeDialog;
}

class NoticeDialog final : public QDialog {
  Q_OBJECT
public:
  explicit NoticeDialog(QWidget *parent = nullptr);
  ~NoticeDialog() = default;
  void showMessage(QString const& msg);

protected:
  void mouseMoveEvent(QMouseEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
private:
  QSharedPointer<Ui::NoticeDialog> ui;
  int x{}, y{};
  bool isDown{false};
};

#endif // NOTICEDIALOG_H
