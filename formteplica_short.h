#ifndef FORMTEPLICA_SHORT_H
#define FORMTEPLICA_SHORT_H

#include <QString>
#include <QWidget>

#include "formteplica.h"
#include "soundplayer.h"
#include "alarmwindow.h"

namespace Ui {
  class Formteplica_short;
}

class Formteplica_short : public QWidget
{
  Q_OBJECT

public:
  explicit Formteplica_short(QWidget *parent = nullptr, quint8 id = 0, const QString &name = nullptr, SoundPlayer *sound = nullptr);
  ~Formteplica_short();
  void shortView(bool flag = true);
  void viewWithoutWindow(bool flag = true);
  void data(FormTeplica::ShowDate &data);
  void dataError();
  void alarm();
  quint8 getId();
  static const int MAXDELTA = 50; // максмимальное превышение сверху
  static const int MINDELTA = 50; // максимальное превышение снизу

signals:
  void mousePress(quint8 _id);

private slots:
  void resetAlarm();

private:
  void mousePressEvent(QMouseEvent *event) override;
  quint8 id_;
  QPixmap *pix_on;
  QPixmap *pix_off;
  QPixmap *pix_err;
  QPixmap *image_pump;
  QPixmap *image_heat;
  SoundPlayer *sound_;
  Alarmwindow *alarm_window = nullptr;
  QTimer *timer_resetAlarm;

  const int MINTEMPERATURE = 100; // минимальная температура
  const quint16 PAUSE_ALARM = 30; // минуты
  const quint16 ALARMDELTA = 100; // дельта, после которой вызов alarm

  Ui::Formteplica_short *ui;
};

#endif // FORMTEPLICA_SHORT_H
