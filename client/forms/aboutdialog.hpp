#pragma once

#include <QCloseEvent>
#include <QDialog>
#include <QSharedPointer>
namespace Ui {
class AboutDialog;
}

class AboutDialog final : public QDialog {
  Q_OBJECT
public:
  explicit AboutDialog(QWidget *parent = nullptr);
  ~AboutDialog() = default;
private slots:
  void on_AboutDialog_finished(int /*result*/);
  void on_pushButton_clicked();

private:
  QSharedPointer<Ui::AboutDialog> ui;
  void closeEvent(QCloseEvent *event);
};

