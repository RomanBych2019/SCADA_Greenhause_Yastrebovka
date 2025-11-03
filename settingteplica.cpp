#include "settingteplica.h"
#include "ui_settingteplica.h"

SettingTeplica::SettingTeplica(QWidget *parent, quint16 arr_setting_tepl[]) :
  QDialog(parent),
  ui(new Ui::SettingTeplica)
{
  ui->setupUi(this);
  ui->verticalSlider_Histeresis->setValue(arr_setting_tepl[0] / 10);
  ui->label_histeresis->setText(QString::number(arr_setting_tepl[0] / 100.0, 'f', 1) + " " + '\xb0' + "C");
//  ui->verticalSlider_Time_open_Windows->setValue(arr_setting_tepl[1]);
//  ui->label_time_open_win->setText(QString::number(arr_setting_tepl[1]) + " сек");
  ui->verticalSlider_Time_open_Windows->setHidden(true);
  ui->label_time_open_win->setHidden(true);
  ui->label->setHidden(true);
  ui->line->setHidden(true);

  ui->verticalSlider_Histeresis_2->setValue(arr_setting_tepl[2] / 10);
  ui->label_histeresis_2->setText(QString::number(arr_setting_tepl[2] / 100.0, 'f', 1) + " " + '\xb0' + "C");
  ui->verticalSlider_Time_open_Windows_2->setValue(arr_setting_tepl[3]);
  ui->label_time_open_win_2->setText(QString::number(arr_setting_tepl[3]) + " сек");

  ui->verticalSlider_Histeresis_3->setValue(arr_setting_tepl[4] / 10);
  ui->label_histeresis_3->setText(QString::number(arr_setting_tepl[4] / 100.0, 'f', 1) + " " + '\xb0' + "C");
//  ui->verticalSlider_Time_open_Windows->setValue(arr_setting_tepl[05]);
//  ui->label_time_open_win->setText(QString::number(arr_setting_tepl[5]) + " сек");
  ui->verticalSlider_Time_open_Windows_3->setHidden(true);
  ui->label_time_open_win_3->setHidden(true);
  ui->label_6->setHidden(true);
  ui->line_3->setHidden(true);
}

quint16 *SettingTeplica::getNewData()
{
  arr[0] = ui->verticalSlider_Histeresis->value() * 10;
  arr[1] = ui->verticalSlider_Time_open_Windows->value();
  arr[2] = ui->verticalSlider_Histeresis_2->value() * 10;
  arr[3] = ui->verticalSlider_Time_open_Windows_2->value();
  arr[4] = ui->verticalSlider_Histeresis_3->value() * 10;
  arr[5] = ui->verticalSlider_Time_open_Windows_3->value();
  return arr;
}

void SettingTeplica::on_buttonBox_accepted()
{
  QDialog::accept();
}

void SettingTeplica::on_verticalSlider_Histeresis_valueChanged(int value)
{
    ui->label_histeresis->setText(QString::number(value / 10.0) + " " + '\xb0' + "C");
}

void SettingTeplica::on_verticalSlider_Time_open_Windows_valueChanged(int value)
{
    ui->label_time_open_win->setText(QString::number(value) + " сек");
}

void SettingTeplica::on_verticalSlider_Histeresis_2_valueChanged(int value)
{
  ui->label_histeresis_2->setText(QString::number(value / 10.0) + " " + '\xb0' + "C");
}

void SettingTeplica::on_verticalSlider_Histeresis_3_valueChanged(int value)
{
  ui->label_histeresis_3->setText(QString::number(value / 10.0) + " " + '\xb0' + "C");
}

void SettingTeplica::on_verticalSlider_Time_open_Windows_2_valueChanged(int value)
{
  ui->label_time_open_win_2->setText(QString::number(value) + " сек");
}

void SettingTeplica::on_verticalSlider_Time_open_Windows_3_valueChanged(int value)
{
  ui->label_time_open_win_3->setText(QString::number(value) + " сек");
}

SettingTeplica::~SettingTeplica()
{
  delete ui;
}

void SettingTeplica::on_buttonBox_rejected()
{
  QDialog::close();
}

