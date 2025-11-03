#ifndef SETTINGTEPLICA_H
#define SETTINGTEPLICA_H

#include <QDialog>

namespace Ui {
  class SettingTeplica;
}

class SettingTeplica : public QDialog
{
  Q_OBJECT

public:
  explicit SettingTeplica(QWidget *parent = nullptr, quint16 arr_setting_tepl[] = nullptr);
  ~SettingTeplica();

  quint16 *getNewData();

private slots:
  void on_buttonBox_accepted();
  void on_verticalSlider_Histeresis_valueChanged(int value);
  void on_verticalSlider_Time_open_Windows_valueChanged(int value);
  void on_verticalSlider_Histeresis_2_valueChanged(int value);
  void on_verticalSlider_Histeresis_3_valueChanged(int value);
  void on_verticalSlider_Time_open_Windows_2_valueChanged(int value);
  void on_verticalSlider_Time_open_Windows_3_valueChanged(int value);

  void on_buttonBox_rejected();

private:
  Ui::SettingTeplica *ui;
  quint16 arr[7];

};

#endif // SETTINGTEPLICA_H
