#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTcpSocket>
#include <QMouseEvent>
#include <QModbusTcpClient>
#include <QUrl>
#include <QFont>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QtGui>
#include <QLabel>
#include <QVector>
#include <QPixmap>
#include <QString>
#include <QFile>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>

#include "charts_tepl.h"
#include "charts_ref.h"
#include "charts_sclad.h"
#include "water.h"

#include "settingNetwork.h"
#include "formteplica.h"
#include "refrigerator.h"
#include "soundplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Formteplica_short;
class FormTeplica;
class SettingNetwork;
class SettingWindow;
class SettingTeplica;
class Weather_dounload;
class FormYandex;
class FormRain;
class WaterTemper;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  QModbusClient *modbusDevice_1_3 = nullptr;
  QModbusClient *modbusDevice_4_7 = nullptr;
  QModbusClient *modbusDevice_8 = nullptr;
  QModbusClient *modbusDevice_ref = nullptr;
  QModbusClient *modbusDevice_meteostation = nullptr;
  QModbusClient *modbusDevice_water = nullptr;

  QLabel *ErrConnectYandexstatusbar;
  QLabel *ErrConnectModbusstatusbar;

  ~MainWindow();

private slots:
  void slotDoneYandex(const QByteArray&);
  void slotTimerConnecttoYandex();
  void mouseClick(quint8 id);
  void slotTimerConnectModbus();
  bool new_connect_tcp(const QString &addres, quint16 port, QModbusClient *modbus_devise);
  void read(quint8 num_device, quint16 reg, qint16 size, QModbusClient *mbclient);
  void write(quint8 num_device, quint16 start_reg, const QVector<quint16> &data, QModbusClient *modbus_devise = nullptr);
  void response_1_3();
  void response_4_7();
  void response_8();
  void response_ref();
  void response_meteostation();
  void responce_water();

  void errorConnectModbus(int num);
  void on_settingNetwork_triggered();
  bool createQueryRef(QSqlDatabase &db, QVector<qint16> &vector);
  bool createQuerySclad(QSqlDatabase &db, QVector<qint16> &vector);
  bool createQueryTepl1(QSqlDatabase &db, QVector<qint16> &vector);
  bool createQueryTepl2(QSqlDatabase &db, QVector<qint16> &vector);
  bool createQueryTepl3(QSqlDatabase &db, QVector<qint16> &vector);
  bool createQueryMeteo(QSqlDatabase &db, QVector<qint16> &vector);

  void get_from_water();


private:

#define TIME_UPDATE_MODBUS 5000
  struct Teplic
  {
    adress adr;
    QModbusClient *mb;
  };

  const QString _PORT = "502";

  QSqlDatabase dbQSql;
  QSqlQuery *query;
//  QSqlQuery *query_sclad;
//  QSqlQuery *query_tepl1;
//  QSqlQuery *query_tepl2;
//  QSqlQuery *query_tepl3;

  QLabel *ErrStatusbar;
  Weather_dounload *weatherYandex;
  QTimer *timer_connect_to_Yandex, *timer_modbus_connect, *timer_saveQSql;
  bool sendYandex = false;
  quint16 counterQSql;

  void initActionsConnections();
  void fileSettingNetwork(QString str);
//  void on_settingNetwork_triggered();
//  void on_settingTepl_triggered();
// дата; номер теплицы; режим работы; насос; доп. нагреватель; уставка; температура; влажность; ошибки темп.датчика; уровень открытия окна; сдвиг уставки для окон;
//  QString strTable = "CREATE TABLE Tepl(DateTime DATETIME, Номер INT, Режим INT, Насос INT, ДопОбогрев INT, Уставка INT, Температура INT, Влажность INT, ОшибкиДТ INT, Окна INT, УставкаОкно INT, Гистерезис INT, ВремяОткрОкна INT);";
//  QString strF = "INSERT INTO Tepl(DateTime, Номер, Режим, Насос, ДопОбогрев, Уставка, Температура, Влажность, ОшибкиДТ, Окна, УставкаОкно, Гистерезис, ВремяОткрОкна) "
//                 "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13'); " ;

  QString strTable_Ref = "CREATE TABLE Ref(DateTime DATETIME, Холодильник_1t INT, Холодильник_1h INT, Холодильник_2t INT, "
                         "Холодильник_2h INT, Холодильник_3t INT, Холодильник_3h INT, Холодильник_4t INT, Холодильник_4h INT, "
                         "Холодильник_5t INT, Холодильник_5h INT, Холодильник_6t INT, Холодильник_6h INT)";
  QString strF_Ref = "INSERT INTO Ref(DateTime, Холодильник_1t, Холодильник_1h, Холодильник_2t, Холодильник_2h, Холодильник_3t, Холодильник_3h, Холодильник_4t, Холодильник_4h, Холодильник_5t, Холодильник_5h, Холодильник_6t, Холодильник_6h)"
                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13')";

  QString strTable_Sclad = "CREATE TABLE Sclad(DateTime DATETIME, Склад_1t INT, Склад_1h INT, Склад_1l INT, Склад_1a INT)";
    QString strF_Sclad = "INSERT INTO Sclad(DateTime, Склад_1t, Склад_1h, Склад_1l, Склад_1a)"
                       "VALUES ('%1', '%2', '%3', '%4', '%5')";


  QString strTable_Tepl1 = "CREATE TABLE Tepl4(DateTime DATETIME, Теплица_4t INT, Теплица_4h INT, Теплица_5t INT, "
                         "Теплица_5h INT, Теплица_6t INT, Теплица_6h INT, Теплица_7t INT, Теплица_7h INT)";
  QString strF_Tepl1 = "INSERT INTO Tepl4(DateTime, Теплица_4t, Теплица_4h, Теплица_5t, Теплица_5h, Теплица_6t, Теплица_6h, Теплица_7t, Теплица_7h)"
                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')";

  QString strTable_Tepl2 = "CREATE TABLE Tepl2(DateTime DATETIME, Теплица_1t INT, Теплица_1h INT, Теплица_2t INT, "
                           "Теплица_2h INT, Теплица_3t INT, Теплица_3h INT)";
  QString strF_Tepl2 = "INSERT INTO Tepl2(DateTime, Теплица_1t, Теплица_1h, Теплица_2t, Теплица_2h, Теплица_3t, Теплица_3h)"
                       "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')";

  QString strTable_Tepl3 = "CREATE TABLE Tepl3(DateTime DATETIME, Теплица_8t INT, Теплица_8h INT)";
  QString strF_Tepl3 = "INSERT INTO Tepl3(DateTime, Теплица_8t, Теплица_8h)"
                       "VALUES ('%1', '%2', '%3')";

  QString strTable_Meteo = "CREATE TABLE Meteo(DateTime DATETIME, Температура INT, Влажность INT, Давление INT, Ветер_скорость INT, Ветер_напр INT, Осадки INT, Освещенность INT, УФ INT)";
  QString strF_Meteo = "INSERT INTO Meteo(DateTime, Температура, Влажность, Давление, Ветер_скорость, Ветер_напр, Осадки, Освещенность, УФ)"
                       "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')";

  FormTeplica *tepl1;
  FormTeplica *tepl2;
  FormTeplica *tepl3;
  FormTeplica *tepl4;
  FormTeplica *tepl5;
  FormTeplica *tepl6;
  FormTeplica *tepl7;
  FormTeplica *tepl8;

  refrigerator *ref1;
  refrigerator *ref2;
  refrigerator *ref3;
  refrigerator *ref4;
  refrigerator *ref5;
  refrigerator *ref6;
  refrigerator *ref7;

  Formteplica_short *tepl_short1;
  Formteplica_short *tepl_short2;
  Formteplica_short *tepl_short3;
  Formteplica_short *tepl_short4;
  Formteplica_short *tepl_short5;
  Formteplica_short *tepl_short6;
  Formteplica_short *tepl_short7;
  Formteplica_short *tepl_short8;


  // Объект теплоноситель
  WaterTemper *wat_temper;
  // Объект полив
  Water *water;

  //Formteplica_short *tepl_short_outdoor;
  FormRain *meteostation;
  Charts *charts;
  Charts_Ref *charts_ref;
  Charts_Sclad *charts_sclad;

  FormYandex *yand;
  Ui::MainWindow *ui;
  SettingNetwork *settingNetwork = nullptr;
  SettingWindow *settingWindow = nullptr;
  SettingTeplica *settingTeplica = nullptr;
  Teplic *tp1 = nullptr;
  Teplic *tp2 = nullptr;
  Teplic *tp3 = nullptr;
  Teplic *ref = nullptr;
  Teplic *meteo = nullptr;
  Teplic *wt = nullptr;

  quint16 arr_setting_tepl[6];

  SoundPlayer *alarm;

  FormTeplica::ShowDate qreateDate(const QModbusDataUnit &unit, int l);
  FormTeplica::ShowDate qreateDate_old(const QModbusDataUnit &unit, int l);

};
#endif // MAINWINDOW_H
