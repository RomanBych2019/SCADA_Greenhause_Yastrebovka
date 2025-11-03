#ifndef ALARMWINDOW_H
#define ALARMWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
  class Alarmwindow;
}

class Alarmwindow : public QDialog
{
  Q_OBJECT

public:
  explicit Alarmwindow(QWidget *parent, char id);
  ~Alarmwindow();

signals:
  void press_ok();

private slots:
  void on_buttonBox_accepted();

private:
  Ui::Alarmwindow *ui;
};

#endif // ALARMWINDOW_H
