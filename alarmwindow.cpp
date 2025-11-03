#include <QString>

#include "alarmwindow.h"
#include "ui_alarmwindow.h"

Alarmwindow::Alarmwindow(QWidget *parent, char id) :
  QDialog(parent),
  ui(new Ui::Alarmwindow)
{
  ui->setupUi(this);
  ui->label_2->setText(QString::number(id));
}

Alarmwindow::~Alarmwindow()
{
  QDialog::accept();
  delete ui;
}

void Alarmwindow::on_buttonBox_accepted()
{
  emit press_ok();
}

