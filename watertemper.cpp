#include "watertemper.h"
#include "ui_temper.h"
#include <QMouseEvent>

WaterTemper::WaterTemper(QWidget *parent) : QWidget(parent),
ui(new Ui::Form_temper)
{
  ui->setupUi(this);
}

void WaterTemper::date(ShowDate &date) const
{
  ui->Temper_1->setText(QString::number( date.ControlTemper1 / 100.0, 'f', 1) + " " + '\xb0' + "C");
  ui->Temper_2->setText(QString::number( date.ControlTemper2 / 100.0, 'f', 1) + " " + '\xb0' + "C");
  ui->Temper_3->setText(QString::number( date.ControlTemper3 / 100.0, 'f', 1) + " " + '\xb0' + "C");
  ui->Temper_4->setText(QString::number( date.ControlTemper4 / 100.0, 'f', 1) + " " + '\xb0' + "C");
  ui->Temper_5->setText(QString::number( date.ControlTemper5 / 100.0, 'f', 1) + " " + '\xb0' + "C");
  ui->Temper_6->setText(QString::number( date.ControlTemper6 / 100.0, 'f', 1) + " " + '\xb0' + "C");
}

void WaterTemper::mousePressEvent(QMouseEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent *mouseEvent = (QMouseEvent *) event;
      if (mouseEvent->button() == Qt::LeftButton)
        emit mousePress();
    }
}
