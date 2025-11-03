#ifndef WATER_H
#define WATER_H

#include <QObject>
#include <QWidget>
#include <QModbusDataUnit>
#include <QLabel>

namespace Ui {
  class Form_water;
  class Form_water_short;
}
class Water: public QWidget
{
  Q_OBJECT

  const QString setTextString(int state);
  void setTextColor(QLabel *, int state);
  QVector<quint16> vector_to_kontroller{};

  quint16 duration = 0;

public:
  Water(QWidget *parent);
  QVector<quint16> getNewData();
  struct ShowData
  {
    bool Zone_ACT_1;   // флаг активности
    bool Zone_ACT_2;
    bool Zone_ACT_3;
    bool Zone_ACT_4;
    qint16 Timer_1;   // оставшееся время полива
    qint16 Timer_2;
    qint16 Timer_3;
    qint16 Timer_4;
    quint16 State_1;  //  режим работы зоны
    quint16 State_2;
    quint16 State_3;
    quint16 State_4;
    quint16 Mode;     // режим всей системы
    quint16 Duration1; // длительность полива (1)
    quint16 Duration2; // длительность полива (2)
    quint16 Duration3; // длительность полива (2)
    quint16 Timer;    // выбранная длительность полива
  };
  void data(const QModbusDataUnit &data);

  Ui::Form_water *ui;
  Ui::Form_water_short *ui_short;

  signals:
    void send_to_gui();

private slots:
  void on_pushButton_start_clicked();
  void on_pushButton_pause_clicked();
  void on_pushButton_stop_clicked();
  void on_pushButton_dur1_clicked();
  void on_pushButton_dur2_clicked();
  void on_pushButton_dur3_clicked();



};

#endif // WATER_H
