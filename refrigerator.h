#pragma once

#include <QTimer>
#include <QWidget>
#include "soundplayer.h"
#include "alarmwindow.h"
#include "formteplica.h"

namespace Ui {
class refrigerator;
}

class refrigerator : public QWidget
{
    Q_OBJECT

public:
    explicit refrigerator(QWidget *parent = nullptr, quint8 id = 0, quint16 mb_adr_write = 300, const QString &name = nullptr, SoundPlayer *sound = nullptr, bool air = false);
    ~refrigerator();
    void data(FormTeplica::ShowDate &data);
    void dataError();
    void alarm();
    quint8 getId();
    static const int MAXDELTA = 50; // максмимальное превышение сверху
    static const int MINDELTA = 50; // максимальное превышение снизу
    quint16 airAveraging = 0;


  signals:
    void mousePress(quint8 _id);

  private slots:
    void resetAlarm();

  private:
    quint8 id_;
    SoundPlayer *sound_;
    Alarmwindow *alarm_window = nullptr;
    QTimer *timer_resetAlarm;

    const int MINTEMPERATURE = 100; // минимальная температура
    const quint16 PAUSE_ALARM = 30; // минуты
    const quint16 ALARMDELTA = 100; // дельта, после которой вызов alarm

private:
    Ui::refrigerator *ui;
};

