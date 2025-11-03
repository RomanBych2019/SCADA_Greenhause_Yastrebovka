#include <QDebug>
#include <QMouseEvent>
#include <QTimer>

#include "soundplayer.h"
#include "formteplica_short.h"
#include "ui_formteplica_short.h"
#include "formteplica.h"
#include "alarmwindow.h"

Formteplica_short::Formteplica_short(QWidget *parent, quint8 id, const QString &name, SoundPlayer *sound) :
  QWidget(parent),
  ui(new Ui::Formteplica_short)
{
  ui->setupUi(this);
  id_ = id;
  sound_ = sound;
  ui->label_title->setText(name);
  ui->label_win->setText("0%");
  ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: cornflowerblue;}");

  pix_on = new  QPixmap (":/icons/icons/pump_on.png");
  pix_off = new  QPixmap (":/icons/icons/pump_off.png");
  ui->label_pump->setPixmap(pix_off->scaled(ui->label_pump->width() * 0.95, ui->label_pump->height() * 0.95, Qt::KeepAspectRatio));
  ui->label_heat->setPixmap(pix_off->scaled(ui->label_heat->width() * 0.95 , ui->label_heat->height() * 0.95, Qt::KeepAspectRatio));

  alarm_window = new Alarmwindow(nullptr, id_);

  connect(alarm_window, SIGNAL(press_ok()), this, SLOT(resetAlarm()));
  timer_resetAlarm = new QTimer();
}

Formteplica_short::~Formteplica_short()
{
  delete ui;
}

void Formteplica_short::shortView(bool flag)
{
  if (flag)
    {
      ui->label_22->setVisible(false);
      ui->label_23->setVisible(false);
      ui->Ustavka->setVisible(false);
      ui->UstavkaWin->setVisible(false);
      ui->progressBar->setVisible(false);
      ui->groupBox->setVisible(false);
      ui->label_win->setVisible(false);
    }
}

void Formteplica_short::viewWithoutWindow(bool flag)
{
  if (flag)
    {
      ui->UstavkaWin->setVisible(false);
      ui->label_23->setVisible(false);
      ui->progressBar->setVisible(false);
      ui->label_win->setVisible(false);
    }
}

void Formteplica_short::data(FormTeplica::ShowDate &data) {
  qint16 temperature = static_cast<qint16>(data.temperature);
  if (data.errTemperature == 1){
      // индикация температуры
      ui->Temperature->setText(QString::number(temperature / 100.0, 'f', 1));
      // индикация влажности
      ui->Hum->setStyleSheet("color: black;");
      if(data.hum == 0)
        ui->Hum->setText("---");
      else
        ui->Hum->setText(QString::number(data.hum / 100.0, 'f', 1));
      // индикация цветом температуры
      if (temperature > data.ustavkaWin + MAXDELTA) {
          ui->Temperature->setStyleSheet("color: tomato;");
        } else if (temperature < data.ustavkaPump - MINDELTA) {
          ui->Temperature->setStyleSheet("color: dodgerblue;");
        } else ui->Temperature->setStyleSheet("color: black;");

      //  оповещение при критическо низкой температуре (за исключением датчика на улице - label_title = "Улица")
      if (ui->label_title->text() != "Улица") {
          if (temperature < static_cast<qint16>(data.ustavkaHeat - ALARMDELTA)) {
              alarm();
            }
          if (temperature < MINTEMPERATURE) {
              alarm();
            }
        }

      // индикация включения насоса
      data.pump ?
        ui->label_pump->setPixmap(pix_on->scaled(ui->label_pump->width() * 0.95, ui->label_pump->height() * 0.95, Qt::KeepAspectRatio)) :
        ui->label_pump->setPixmap(pix_off->scaled(ui->label_pump->width() * 0.95, ui->label_pump->height() * 0.95, Qt::KeepAspectRatio));
      // индикация включения дополнительного обогревателя
      data.heat ?
        ui->label_heat->setPixmap(pix_on->scaled(ui->label_heat->width() * 0.95, ui->label_heat->height() * 0.95, Qt::KeepAspectRatio)) :
        ui->label_heat->setPixmap(pix_off->scaled(ui->label_heat->width() * 0.95, ui->label_heat->height() * 0.95, Qt::KeepAspectRatio));
    }
  else {
      ui->Temperature->setStyleSheet("color: crimson;");
      ui->Hum->setStyleSheet("color: crimson;");
      ui->Temperature->setText(QString::number(data.errTemperature, 16));
      ui->Hum->setText("---");
    }

  // индикация уставок
  ui->Ustavka->setText(QString::number(data.ustavkaPump / 100.0, 'f', 1));
  ui->UstavkaWin->setText(QString::number(data.ustavkaWin / 100.0, 'f', 1));
  ui->UstavkaHeat->setText(QString::number(data.ustavkaHeat / 100.0, 'f', 1));

  // индикация уровня открытия окна
  ui->label_win->setText(QString::number(data.levelwindow) + " %");
  ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: cornflowerblue;}");
  ui->progressBar->setValue(data.levelwindow);

}

void Formteplica_short::dataError()
{
  ui->Temperature->setStyleSheet("color: red;");
  ui->Temperature->setText("---");
  ui->Hum->setStyleSheet("color: red;");
  ui->Hum->setText("---");
  ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: gray;}");
}

void Formteplica_short::alarm()
{
  if (!timer_resetAlarm->isActive()) {
      alarm_window->setModal(true);
      alarm_window->show();
      ui->frame->setStyleSheet("background-color: rgba(255, 115, 115, 249);");
      if (!sound_->isMuted()) {
          sound_->play();
        }
    }
}

void Formteplica_short::resetAlarm()
{
  sound_->stopPlay();
  alarm_window->close();
  ui->frame->setStyleSheet("background-color: rgb(232, 232, 232);");

  //таймер отсрочки повторного вызова окна Alarm
  timer_resetAlarm->setInterval(static_cast<quint32>(PAUSE_ALARM * 1'000 * 60));
  timer_resetAlarm->setSingleShot(true);
  timer_resetAlarm->start();
}

quint8 Formteplica_short::getId()
{
  return id_;
}

void Formteplica_short::mousePressEvent(QMouseEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent *mouseEvent = (QMouseEvent *) event;
      if (mouseEvent->button() == Qt::LeftButton)
        emit mousePress(id_);
    }
}
