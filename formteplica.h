#ifndef FORMTEPLICA_H
#define FORMTEPLICA_H

#include <QDial>
#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QWidget>

enum Mode
  {
      AUTO = 0,
      MANUAL,
      AIR,
      DECREASE_IN_HUMIDITY
  };

namespace Ui {
  class FormTeplica;
}

class FormTeplica : public QDialog
{
  Q_OBJECT
private:
  quint8 _id;
  quint16 _mb_adr_write;
  quint16 arr[9];
  Ui::FormTeplica *ui;
  float getNumberFromQString(const QString &xString);

public:
  FormTeplica(QDialog *parent, quint8 id = 0, quint16 mb_adr_write = 300);

  ~FormTeplica();

  struct ShowDate
  {
    quint16 errTemperature;
    qint16 temperature;
    quint16 hum;
    quint16 ustavkaPump;
    quint16 ustavkaHeat;
    quint16 ustavkaWin;
    quint16 pump;
    quint16 heat;
    quint16 levelwindow;
    quint16 hyster;
    quint16 openTimeWin;
    quint16 mode;
    qint16 soilSensorT;    //  температура почвы
    quint16 soilSensorH;    //  влажность почвы
    quint16 soilSensorC;    //  PH почвы
    quint16 soilSensorS;    //  Nitrogen почвы
    quint16 soilSensorTDS;    //  Conductivity почвы
    quint16 light;    //  Освещенность датчик WB
    quint16 airQuality;    //  Качество воздуха датчик WB
  };

  void setTitle(QString &name);
  void setUnVisibleUstWin(bool flag);
  void data( ShowDate &data) const;
  QLabel *getTemperature();
  QLabel *getHum();
  QLabel *getUstavka();
  QLabel *getUstavkaWin();
  QDial *getdial_UstTemper();
  QDial *getdial_UstWin();
  QProgressBar *getprogressBar();
  quint16 getmb_adr_write();
//  quint16 *getNewData();

  QVector<quint16> getNewData();

private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_rejected();
  void on_radioButton_auto_clicked() const;
  void on_radioButton_manual_clicked() const;
  void on_radioButton_win_clicked() const;
  void on_radioButton_decrease_in_humidity_clicked() const;
  void on_dial_UstTemper_valueChanged(int value);
  void on_dial_Ust_Win_valueChanged(int value);
  void on_Setting_window_valueChanged(int value);
  void on_pushButtonPump_clicked();
  void on_pushButtonHeat_clicked();
  void on_Setting_window_2_valueChanged(int value);
  void on_Setting_window_3_valueChanged(int value);
  void on_dial_Ust_Heat_valueChanged(int value);
};

#endif // FORMTEPLICA_H
