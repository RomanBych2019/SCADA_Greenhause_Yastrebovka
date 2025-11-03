#include "refrigerator.h"
#include "ui_refrigerator.h"

#include <QTimer>

refrigerator::refrigerator(QWidget *parent, quint8 id, quint16 mb_adr_write, const QString &name, SoundPlayer *sound, bool air) :
    QWidget(parent),
    ui(new Ui::refrigerator)
{
    ui->setupUi(this);
    id_ = id;
    sound_ = sound;
    ui->groupBox->setTitle(name);

    ui->Air->setVisible(air);
    ui->label_air->setVisible(air);
    ui->line_air->setVisible(air);

    alarm_window = new Alarmwindow(nullptr, id_);

    timer_resetAlarm = new QTimer();
}

refrigerator::~refrigerator()
{
    delete ui;
}

void refrigerator::data(FormTeplica::ShowDate &data)
{
    if (data.errTemperature == 1){
        // индикация температуры
        ui->Temperature->setStyleSheet("color: black");
        ui->Temperature->setText(QString::number(data.temperature / 100.0, 'f', 1) );
        // индикация влажности
        ui->Hum->setStyleSheet("color: black;");
        ui->Hum->setText(QString::number(data.hum / 100.0, 'f', 1));
        ui->Air->setStyleSheet("color: gray;");
        ui->Air->setText(QString::number(data.airQuality));
      }
    else {
        ui->Temperature->setStyleSheet("color: crimson;");
        ui->Temperature->setText(QString::number(data.errTemperature, 16));
        ui->Hum->setStyleSheet("color: crimson;");
        ui->Hum->setText("---");
        ui->Air->setStyleSheet("color: crimson;");
        ui->Air->setText("---");
    }
}

void refrigerator::dataError()
{
    ui->Temperature->setStyleSheet("color: crimson;");
    ui->Temperature->setText("---");
    ui->Hum->setStyleSheet("color: crimson;");
    ui->Hum->setText("---");
    ui->Air->setStyleSheet("color: crimson;");
    ui->Air->setText("---");
}

quint8 refrigerator::getId()
{
    return id_;
}

void refrigerator::resetAlarm()
{

}
