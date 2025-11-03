#include "water.h"
#include "ui_water.h"
#include "QDebug"


Water::Water(QWidget *parent) : QWidget(parent),
ui(new Ui::Form_water)
{
  ui->setupUi(this);
}

QVector<quint16> Water::getNewData()
{

  return vector_to_kontroller;
}

void Water::data(const QModbusDataUnit &unit)
{
  Water::ShowData dat{};
  dat.Zone_ACT_1 = unit.value(0);
  dat.Zone_ACT_2 = unit.value(1);
  dat.Zone_ACT_3 = unit.value(2);
  dat.Zone_ACT_4 = unit.value(3);
  dat.Timer_1 = unit.value(4);
  dat.Timer_2 = unit.value(5);
  dat.Timer_3 = unit.value(6);
  dat.Timer_4 = unit.value(7);
  dat.State_1 = unit.value(8);
  dat.State_2 = unit.value(9);
  dat.State_3 = unit.value(10);
  dat.State_4 = unit.value(11);
  dat.Mode = unit.value(12);
  dat.Duration1 = unit.value(13);
  dat.Duration2 = unit.value(14);
  dat.Duration3 = unit.value(15);
  dat.Timer = unit.value(16);

  ui->label_mode1->setText(setTextString(dat.State_1));
  ui->label_mode2->setText(setTextString(dat.State_2));
  ui->label_mode3->setText(setTextString(dat.State_3));
  ui->label_mode4->setText(setTextString(dat.State_4));

  setTextColor(ui->label_mode1, dat.State_1);
  setTextColor(ui->label_mode2, dat.State_2);
  setTextColor(ui->label_mode3, dat.State_3);
  setTextColor(ui->label_mode4, dat.State_4);

  if (dat.Mode)
    {
      ui->checkBox_1->setEnabled(false);
      ui->checkBox_2->setEnabled(false);
      ui->checkBox_3->setEnabled(false);
      ui->checkBox_4->setEnabled(false);
    }
  else
    {
      ui->checkBox_1->setEnabled(true);
      ui->checkBox_2->setEnabled(true);
      ui->checkBox_3->setEnabled(true);
      ui->checkBox_4->setEnabled(true);
    }

  ui->label_last1->setText(QString::number(dat.Timer_1/60) + ":" + QString::number(dat.Timer_1%60).rightJustified(2, '0'));
  ui->label_last2->setText(QString::number(dat.Timer_2/60) + ":" + QString::number(dat.Timer_2%60).rightJustified(2, '0'));
  ui->label_last3->setText(QString::number(dat.Timer_3/60) + ":" + QString::number(dat.Timer_3%60).rightJustified(2, '0'));
  ui->label_last4->setText(QString::number(dat.Timer_4/60) + ":" + QString::number(dat.Timer_4%60).rightJustified(2, '0'));

  ui->pushButton_start->setDown(dat.Mode == 1);
  ui->pushButton_pause->setDown(dat.Mode == 2);
  ui->pushButton_stop->setDown(dat.Mode == 0);

  ui->pushButton_dur1->setText(QString::number(dat.Duration1) + " мин");
  ui->pushButton_dur2->setText(QString::number(dat.Duration2) + " мин");
  ui->pushButton_dur3->setText(QString::number(dat.Duration3) + " мин");

  if (dat.Mode == 1 || dat.Mode == 2)
    {
      ui->pushButton_dur1->setEnabled(false);
      ui->pushButton_dur2->setEnabled(false);
      ui->pushButton_dur3->setEnabled(false);

      ui->checkBox_1->setChecked(dat.Zone_ACT_1);
      ui->checkBox_2->setChecked(dat.Zone_ACT_2);
      ui->checkBox_3->setChecked(dat.Zone_ACT_3);
      ui->checkBox_4->setChecked(dat.Zone_ACT_4);

      if (dat.Timer == dat.Duration1) {
          ui->pushButton_dur1->setDown(true);
          ui->pushButton_dur2->setDown(false);
          ui->pushButton_dur3->setDown(false);
        }
       else if (dat.Timer == dat.Duration2) {
          ui->pushButton_dur2->setDown(true);
          ui->pushButton_dur1->setDown(false);
          ui->pushButton_dur3->setDown(false);
        }
       else if (dat.Timer == dat.Duration3) {
          ui->pushButton_dur3->setDown(true);
          ui->pushButton_dur1->setDown(false);
          ui->pushButton_dur2->setDown(false);
        }
      else  {
         ui->pushButton_dur3->setDown(false);
         ui->pushButton_dur1->setDown(false);
         ui->pushButton_dur2->setDown(false);
       }
    }
  else
    {
      ui->pushButton_dur1->setEnabled(true);
      ui->pushButton_dur2->setEnabled(true);
      ui->pushButton_dur3->setEnabled(true);
    }
}

const QString Water::setTextString(int state)
{
  switch (state) {
    case 1:
      return "полив";
      break;
    case 2:
      return "пауза";
      break;
    default:
      return "выкл";
    }
}

void Water::setTextColor(QLabel *lebel, int state)
{
  switch (state) {
    case 1:
      lebel->setStyleSheet("color: tomato;");
      break;
    case 2:
      lebel->setStyleSheet("color: dodgerblue;");
      break;
    default:
      lebel->setStyleSheet("color: black;");
    }
}

void Water::on_pushButton_start_clicked()
{
  vector_to_kontroller.clear();
  vector_to_kontroller.push_back(1);
  vector_to_kontroller.push_back(ui->checkBox_1->isChecked() + (ui->checkBox_2->isChecked() << 1)
                                 + (ui->checkBox_3->isChecked() << 2) + (ui->checkBox_4->isChecked() << 3));
  vector_to_kontroller.push_back(duration);
  vector_to_kontroller.push_back(1);
  emit send_to_gui();
}


void Water::on_pushButton_pause_clicked()
{
  vector_to_kontroller.clear();
  vector_to_kontroller.push_back(1);
  vector_to_kontroller.push_back(ui->checkBox_1->isChecked() + (ui->checkBox_2->isChecked() << 1)
                                 + (ui->checkBox_3->isChecked() << 2) + (ui->checkBox_4->isChecked() << 3));
  vector_to_kontroller.push_back(duration);
  vector_to_kontroller.push_back(2);
  emit send_to_gui();
}


void Water::on_pushButton_stop_clicked()
{
  vector_to_kontroller.clear();
  vector_to_kontroller.push_back(1);
  vector_to_kontroller.push_back(ui->checkBox_1->isChecked() + (ui->checkBox_2->isChecked() << 1)
                                 + (ui->checkBox_3->isChecked() << 2) + (ui->checkBox_4->isChecked() << 3));
  vector_to_kontroller.push_back(duration);
  vector_to_kontroller.push_back(0);
  emit send_to_gui();
}


void Water::on_pushButton_dur1_clicked()
{
  duration = 0;
  ui->pushButton_dur1->setDown(true);
  ui->pushButton_dur2->setDown(false);
  ui->pushButton_dur3->setDown(false);
}


void Water::on_pushButton_dur2_clicked()
{
  duration = 1;
  ui->pushButton_dur2->setDown(true);
  ui->pushButton_dur1->setDown(false);
  ui->pushButton_dur3->setDown(false);
}


void Water::on_pushButton_dur3_clicked()
{
  duration = 2;
  ui->pushButton_dur3->setDown(true);
  ui->pushButton_dur2->setDown(false);
  ui->pushButton_dur1->setDown(false);
}




