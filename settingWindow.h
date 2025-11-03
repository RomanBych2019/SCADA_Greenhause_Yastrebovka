#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>

namespace Ui {
  class SettingWindow;
}

class SettingWindow : public QDialog
{
  Q_OBJECT

public:
  SettingWindow(QWidget *parent = nullptr, qint16 level = 0);
  qint16 getOutLevel();
  ~SettingWindow();

private slots:
  void on_Setting_window_valueChanged(int value);
  void on_buttonBox_accepted();
signals:
    void applied();

private:
  Ui::SettingWindow *ui;
};

#endif // SETTINGWINDOW_H
