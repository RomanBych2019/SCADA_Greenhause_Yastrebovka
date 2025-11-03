#include "QDebug"
#include <QVector>

#include "settingNetwork.h"
#include "ui_settingNetwork.h"

SettingNetwork::SettingNetwork(QWidget *parent, adress &tepl1, adress &tepl2, adress &tepl3, adress &ref, adress &meteo, adress &wt):
  QDialog(parent),
  ui(new Ui::SettingNetwork)
{
  ui->setupUi(this);
  _tepl1 = tepl1;
  _tepl2 = tepl2;
  _tepl3 = tepl3;
  _ref = ref;
  _meteo = meteo;
  _wt = wt;

  ui->lineAdress->setText(_tepl1.host);
  ui->linePort->setText(QString::number(_tepl1.mb_adress));
  ui->radioButton_1_3->setChecked(1);
}

SettingNetwork::~SettingNetwork()
{
  delete ui;
}

void SettingNetwork::on_buttonBox_accepted()
{
  QDialog::accept();
}


QVector<adress> SettingNetwork::getServerAdress()
{
  QVector<adress> vec;
  vec.clear();
  vec.push_back(_tepl1);
  vec.push_back(_tepl2);
  vec.push_back(_tepl3);
  vec.push_back(_ref);
  vec.push_back(_meteo);
  vec.push_back(_wt);

  return vec;
}

void SettingNetwork::on_radioButton_1_3_clicked()
{
    ui->lineAdress->setText(_tepl1.host);
    ui->linePort->setText(QString::number(_tepl1.mb_adress));
}

void SettingNetwork::on_radioButton_4_7_clicked()
{
    ui->lineAdress->setText(_tepl2.host);
    ui->linePort->setText(QString::number(_tepl2.mb_adress));
}

void SettingNetwork::on_radioButton_8_clicked()
{
    ui->lineAdress->setText(_tepl3.host);
    ui->linePort->setText(QString::number(_tepl3.mb_adress));
}

void SettingNetwork::on_buttonBox_clicked(QAbstractButton *button)
{
  if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply)
    {
      if (ui->radioButton_1_3->isChecked()) {
          _tepl1.host = ui->lineAdress->text();
          _tepl1.mb_adress = ui->linePort->text().toInt();
        }
      if (ui->radioButton_4_7->isChecked())
        {
          _tepl2.host = ui->lineAdress->text();
          _tepl2.mb_adress = ui->linePort->text().toInt();
        }
      if (ui->radioButton_8->isChecked())
        {
          _tepl3.host = ui->lineAdress->text();
          _tepl3.mb_adress = ui->linePort->text().toInt();
        }
      if (ui->radioButton_ref->isChecked())
        {
          _ref.host = ui->lineAdress->text();
          _ref.mb_adress = ui->linePort->text().toInt();
        }
      if (ui->radioButton_meteo->isChecked())
        {
          _meteo.host = ui->lineAdress->text();
          _meteo.mb_adress = ui->linePort->text().toInt();
        }
      if (ui->radioButton_wt->isChecked())
      {
          _wt.host = ui->lineAdress->text();
          _wt.mb_adress = ui->linePort->text().toInt();
      }
    }
}

void SettingNetwork::on_radioButton_ref_clicked()
{
    ui->lineAdress->setText(_ref.host);
    ui->linePort->setText(QString::number(_ref.mb_adress));
}


void SettingNetwork::on_radioButton_meteo_clicked()
{
    ui->lineAdress->setText(_meteo.host);
    ui->linePort->setText(QString::number(_meteo.mb_adress));
}


void SettingNetwork::on_radioButton_wt_clicked()
{
    ui->lineAdress->setText(_wt.host);
    ui->linePort->setText(QString::number(_wt.mb_adress));
}

