#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formyandex.h"
#include "formteplica.h"
#include "formteplica_short.h"
#include "settingNetwork.h"
#include "weather_dounload.h"
#include "formrain.h"
#include "charts_tepl.h"
#include "charts_ref.h"
#include "watertemper.h"
#include "water.h"

//#include "alarmwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QDir::currentPath());

    dbQSql = QSqlDatabase::addDatabase("QSQLITE");
    dbQSql.setDatabaseName("./dbQSql/tepl.db");

    ErrStatusbar = new QLabel(this);
    if (!dbQSql.open()) {
        qDebug() << "База теплиц не открыта";
        ErrStatusbar->setText(dbQSql.lastError().text());
        ui->statusbar->addWidget(ErrStatusbar);
    }

    query = new QSqlQuery(dbQSql);

    if (query->exec(strTable_Ref))
        qDebug() << "В базе нет таблицы холодильников";

    if (query->exec(strTable_Sclad))
        qDebug() << "В базе нет таблицы склада";

    if (query->exec(strTable_Meteo))
        qDebug() << "В базе нет таблицы метеостанции";

    if (query->exec(strTable_Tepl1))
        qDebug() << "В базе нет таблицы теплиц 4-7";

    if (query->exec(strTable_Tepl2))
        qDebug() << "В базе нет таблицы теплиц 1-3";

    if (query->exec(strTable_Tepl3))
        qDebug() << "В базе нет таблицы теплиц 8";

    tp1 = new Teplic;
    tp2 = new Teplic;
    tp3 = new Teplic;
    ref = new Teplic;
    meteo = new Teplic;
    wt = new Teplic;

    alarm = new  SoundPlayer();

    fileSettingNetwork("read");

    modbusDevice_1_3 = new QModbusTcpClient(this);
    modbusDevice_4_7 = new QModbusTcpClient(this);
    modbusDevice_8 = new QModbusTcpClient(this);
    modbusDevice_ref = new QModbusTcpClient(this);
    modbusDevice_meteostation = new QModbusTcpClient(this);
    modbusDevice_water = new QModbusTcpClient(this);


    new_connect_tcp(tp1->adr.host, _PORT.toInt(), modbusDevice_1_3);
    new_connect_tcp(tp2->adr.host, _PORT.toInt(), modbusDevice_4_7);
    new_connect_tcp(tp3->adr.host, _PORT.toInt(), modbusDevice_8);
    new_connect_tcp(ref->adr.host, _PORT.toInt(), modbusDevice_ref);
    new_connect_tcp(meteo->adr.host, _PORT.toInt(), modbusDevice_meteostation);
    new_connect_tcp(wt->adr.host, _PORT.toInt(), modbusDevice_water);

    ErrConnectModbusstatusbar = new QLabel(this);

    yand = new FormYandex;
    ErrConnectYandexstatusbar = new QLabel(this);
    weatherYandex = new Weather_dounload(this);

    slotTimerConnecttoYandex();
    timer_connect_to_Yandex = new QTimer();
    timer_connect_to_Yandex->start(3600000);

    tepl1 = new FormTeplica(nullptr, 1, 300);
    tepl2 = new FormTeplica(nullptr, 2, 311);
    tepl3 = new FormTeplica(nullptr, 3, 322);
    tepl4 = new FormTeplica(nullptr, 4, 300);
    tepl5 = new FormTeplica(nullptr, 5, 311);
    tepl6 = new FormTeplica(nullptr, 6, 322);
    tepl7 = new FormTeplica(nullptr, 7, 333);
    tepl8 = new FormTeplica(nullptr, 8, 300);

    tepl_short1 = new Formteplica_short(nullptr, 1, "Теплица 1", alarm);
    tepl_short2 = new Formteplica_short(nullptr, 2, "Теплица 2", alarm);
    tepl_short3 = new Formteplica_short(nullptr, 3, "Теплица 3", alarm);
    tepl_short4 = new Formteplica_short(nullptr, 4, "Теплица 4", alarm);
    tepl_short5 = new Formteplica_short(nullptr, 5, "Теплица 5", alarm);
    tepl_short6 = new Formteplica_short(nullptr, 6, "Теплица 6", alarm);
    tepl_short7 = new Formteplica_short(nullptr, 7, "Теплица 7", alarm);
    tepl_short8 = new Formteplica_short(nullptr, 8, "Теплица 8", alarm);

    meteostation = new FormRain(nullptr, "Метеостанция");

    ref1 = new refrigerator(nullptr, 9, 400, "Холодильник 1", alarm);
    ref2 = new refrigerator(nullptr, 10, 403, "Холодильник 2", alarm);
    ref3 = new refrigerator(nullptr, 11, 406, "Холодильник 3", alarm);
    ref4 = new refrigerator(nullptr, 12, 409, "Холодильник 4", alarm);
    ref5 = new refrigerator(nullptr, 13, 412, "Склад улица", alarm);
    ref6 = new refrigerator(nullptr, 14, 415, "Холодильник 5", alarm);
    ref7 = new refrigerator(nullptr, 15, 418, "Холодильник 6", alarm, true);

    wat_temper = new WaterTemper(nullptr);

    water = new Water(nullptr);

    ui->refrigerator->addWidget(ref1);
    ui->refrigerator->addWidget(ref2);
    ui->refrigerator->addWidget(ref3);
    ui->refrigerator->addWidget(ref4);
    ui->refrigerator->addWidget(ref5);
    ui->refrigerator->addWidget(ref6);
    ui->refrigerator->addWidget(ref7);

    ui->horizontalLayout->addWidget(tepl_short1);
    ui->horizontalLayout->addWidget(tepl_short1);
    ui->horizontalLayout->addWidget(tepl_short2);
    ui->horizontalLayout->addWidget(tepl_short3);
    ui->horizontalLayout->addWidget(tepl_short4);
    ui->horizontalLayout->addWidget(water);

    ui->horizontalLayout_4->addWidget(tepl_short5);
    ui->horizontalLayout_4->addWidget(tepl_short6);
    ui->horizontalLayout_4->addWidget(tepl_short7);
    ui->horizontalLayout_4->addWidget(tepl_short8);
    ui->horizontalLayout_4->addWidget(wat_temper);

    ui->horizontalLayout_3->addWidget(meteostation);

    ui->horizontalLayout_2->addWidget(yand);

    tp1->mb = modbusDevice_1_3;
    tp2->mb = modbusDevice_4_7;
    tp3->mb = modbusDevice_8;
    ref->mb = modbusDevice_ref;
    meteo->mb = modbusDevice_meteostation;
    wt->mb = modbusDevice_water;

    timer_modbus_connect = new QTimer();
    timer_modbus_connect->start(TIME_UPDATE_MODBUS);
    initActionsConnections();

    charts = new Charts(dbQSql, nullptr);
    ui->verticalLayout_charts->addWidget(charts);

    charts_ref = new Charts_Ref(dbQSql, nullptr);
    ui->verticalLayout_chart_ref->addWidget(charts_ref);

    charts_sclad = new Charts_Sclad(dbQSql, nullptr);
    ui->verticalLayout_chart_sclad->addWidget(charts_sclad);
}

void MainWindow::initActionsConnections()
{
    connect(timer_modbus_connect, SIGNAL(timeout()), this, SLOT(slotTimerConnectModbus()));
    connect(weatherYandex, SIGNAL(done(const QByteArray&)), this, SLOT(slotDoneYandex(const QByteArray&)));
    connect(timer_connect_to_Yandex, SIGNAL(timeout()), this, SLOT(slotTimerConnecttoYandex()));
    connect(ui->quit, &QAction::triggered, this, &MainWindow::close);

    connect(tepl_short1, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short2, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short3, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short4, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short5, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short6, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short7, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));
    connect(tepl_short8, SIGNAL(mousePress(quint8)), this, SLOT(mouseClick(quint8)));

    connect(water,SIGNAL(send_to_gui()),this,SLOT(get_from_water()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settingNetwork_triggered()
{
    settingNetwork = new SettingNetwork(nullptr, tp1->adr, tp2->adr, tp3->adr, ref->adr, meteo->adr, wt->adr);
    settingNetwork->setModal(true);
    if ( settingNetwork->exec() == QDialog::Accepted ) {
        QVector<adress> vec;
        vec = settingNetwork->getServerAdress();
        tp1->adr = vec[0];
        tp2->adr = vec[1];
        tp3->adr = vec[2];
        ref->adr = vec[3];
        meteo->adr = vec[4];
        wt->adr = vec[5];
        fileSettingNetwork("write");
    }
}

bool MainWindow::createQueryRef(QSqlDatabase &db, QVector<qint16> &vector)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QString str;
    QVector<qint16>::iterator it = vector.begin();
    str = strF_Ref.arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)));


    QSqlQuery *query = new QSqlQuery(db);
    if (!query->exec(str))
    {
        qDebug() << "Нельзя вставить запись в таблицу холодильники";
        return true;
    }
    return false;
}

bool MainWindow::createQuerySclad(QSqlDatabase &db, QVector<qint16> &vector)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QString str;
    QVector<qint16>::iterator it = vector.begin();
    str = strF_Sclad.arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++) * 100))
            .arg(QString::number(*(it++) * 100));


    QSqlQuery *query = new QSqlQuery(db);
    if (!query->exec(str))
    {
        qDebug() << "Нельзя вставить запись в таблицу склад";
        return true;
    }
    return false;
}

bool MainWindow::createQueryTepl1(QSqlDatabase &db, QVector<qint16> &vector)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QString str;

    QVector<qint16>::iterator it = vector.begin();
    str = strF_Tepl1.arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)));

    QSqlQuery *query = new QSqlQuery(db);
    if (!query->exec(str))
    {
        qDebug() << "Нельзя вставить запись в таблицу теплица1 (4-7)";
        return true;
    }
    return false;
}

bool MainWindow::createQueryTepl2(QSqlDatabase &db, QVector<qint16> &vector)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QString str;

    QVector<qint16>::iterator it = vector.begin();
    str = strF_Tepl2.arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)));

    QSqlQuery *query = new QSqlQuery(db);
    if (!query->exec(str))
    {
        qDebug() << "Нельзя вставить запись в таблицу теплица2 (1-3)";
        return true;
    }
    return false;
}

bool MainWindow::createQueryTepl3(QSqlDatabase &db, QVector<qint16> &vector)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QString str;

    QVector<qint16>::iterator it = vector.begin();
    str = strF_Tepl3.arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)));

    QSqlQuery *query = new QSqlQuery(db);
    if (!query->exec(str))
    {
        qDebug() << "Нельзя вставить запись в таблицу теплица3 (8)";
        return true;
    }
    return false;
}

bool MainWindow::createQueryMeteo(QSqlDatabase &db, QVector<qint16> &vector)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    QString str;

    QVector<qint16>::iterator it = vector.begin();
    str = strF_Meteo.arg(timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)))
            .arg(QString::number(*(it++)));

    QSqlQuery *query = new QSqlQuery(db);
    if (!query->exec(str))
    {
        qDebug() << "Нельзя вставить запись в таблицу метеостанция";
        return true;
    }
    return false;
}

FormTeplica::ShowDate MainWindow::qreateDate(const QModbusDataUnit &unit, int leght)
{
    FormTeplica::ShowDate data;
    data.errTemperature = unit.value(8 + leght);
    data.temperature = static_cast<qint16>(unit.value(6 + leght));
    data.hum = unit.value(7 + leght);
    data.ustavkaPump = unit.value(3 + leght);
    data.ustavkaHeat = unit.value(4 + leght);
    data.ustavkaWin = unit.value(5 + leght);
    data.pump = unit.value(1 + leght);
    data.heat = unit.value(2 + leght);
    data.levelwindow = unit.value(9 + leght);
    data.hyster = unit.value(10 + leght);
    data.openTimeWin = unit.value(11 + leght);
    data.mode = unit.value(0 + leght);
    data.soilSensorT = unit.value(12 + leght);
    data.soilSensorH = unit.value(13 + leght);
    data.soilSensorC = unit.value(14 + leght);
    data.soilSensorS = unit.value(15 + leght);
    data.soilSensorTDS = unit.value(16 + leght);

    data.light = unit.value(0 + leght);

    data.airQuality = unit.value(1 + leght);

    return data;
}

//------------ файл настроек сети----------//
void MainWindow::fileSettingNetwork(QString str) {
    QFile file("./set/networkset.txt");
    if(str == "read") {
        if(QFileInfo("./set/networkset.txt").exists()){
            file.open(QIODevice::ReadOnly);
            tp1->adr.host = file.readLine().trimmed();
            tp1->adr.mb_adress = file.readLine().trimmed().toInt();
            tp2->adr.host = file.readLine().trimmed();
            tp2->adr.mb_adress = file.readLine().trimmed().toInt();
            tp3->adr.host = file.readLine().trimmed();
            tp3->adr.mb_adress = file.readLine().trimmed().toInt();
            ref->adr.host = file.readLine().trimmed();
            ref->adr.mb_adress = file.readLine().trimmed().toInt();
            meteo->adr.host = file.readLine().trimmed();
            meteo->adr.mb_adress = file.readLine().trimmed().toInt();
            wt->adr.host = file.readLine().trimmed();
            wt->adr.mb_adress = file.readLine().trimmed().toInt();
        }
    }
    else
    {
        QFile file("./set/networkset.txt");
        QFile::remove("./set/networkset.txt");
        file.open(QIODevice::ReadWrite);
        file.write((tp1->adr.host + "\n").toStdString().c_str());
        file.write((QString::number(tp1->adr.mb_adress) + "\n").toStdString().c_str());
        file.write((tp2->adr.host + "\n").toStdString().c_str());
        file.write((QString::number(tp2->adr.mb_adress) + "\n").toStdString().c_str());
        file.write((tp3->adr.host + "\n").toStdString().c_str());
        file.write((QString::number(tp3->adr.mb_adress) + "\n").toStdString().c_str());
        file.write((ref->adr.host + "\n").toStdString().c_str());
        file.write((QString::number(ref->adr.mb_adress) + "\n").toStdString().c_str());
        file.write((meteo->adr.host + "\n").toStdString().c_str());
        file.write((QString::number(meteo->adr.mb_adress) + "\n").toStdString().c_str());
        file.write((wt->adr.host + "\n").toStdString().c_str());
        file.write((QString::number(wt->adr.mb_adress) + "\n").toStdString().c_str());
    }
    file.close();
}
/*--------------------------------------------- парсинг данных погоды от Yandex ---------------------------------------------*/
void MainWindow::slotDoneYandex(const QByteArray& text)
{
    yand->setData(text);
}
void MainWindow::slotTimerConnecttoYandex()
{
    if (weatherYandex->online())
    {
        weatherYandex->sendData();
        ErrConnectYandexstatusbar->clear();
    }
    else {
        ErrConnectYandexstatusbar->setText("Нет связи с сервисом погоды Yandex");
        ui->statusbar->addWidget(ErrConnectYandexstatusbar);
    }
}

void MainWindow::mouseClick(quint8 id)
{
    switch (id) {
    case 1:
        tepl1->show();
        if (tepl1->exec() == QDialog::Accepted){
            write(tp1->adr.mb_adress, tepl1->getmb_adr_write(), tepl1->getNewData(), tp1->mb);
        }
        break;;
    case 2:
        tepl2->show();
        if (tepl2->exec() == QDialog::Accepted){
            write(tp1->adr.mb_adress, tepl2->getmb_adr_write(), tepl2->getNewData(), tp1->mb);
        }
        break;
    case 3:
        tepl3->show();
        if (tepl3->exec() == QDialog::Accepted){
            write(tp1->adr.mb_adress, tepl3->getmb_adr_write(), tepl3->getNewData(), tp1->mb);
        }
        break;
    case 4:
        tepl4->show();
        if (tepl4->exec() == QDialog::Accepted){
            write(tp2->adr.mb_adress, tepl4->getmb_adr_write(), tepl4->getNewData(), tp2->mb);
        }
        break;
    case 5:
        tepl5->show();
        if (tepl5->exec() == QDialog::Accepted){
            write(tp2->adr.mb_adress, tepl5->getmb_adr_write(), tepl5->getNewData(), tp2->mb);
        }
        break;
    case 6:
        tepl6->show();
        if (tepl6->exec() == QDialog::Accepted){
            write(tp2->adr.mb_adress, tepl6->getmb_adr_write(), tepl6->getNewData(), tp2->mb);
        }
        break;
    case 7:
        tepl7->show();
        if (tepl7->exec() == QDialog::Accepted){
            write(tp2->adr.mb_adress, tepl7->getmb_adr_write(), tepl7->getNewData(), tp2->mb);
        }
        break;
    case 8:
        tepl8->show();
        if (tepl8->exec() == QDialog::Accepted){
            write(tp3->adr.mb_adress, tepl8->getmb_adr_write(), tepl8->getNewData(), tp3->mb);
        }
        break;
    }
}

void MainWindow:: errorConnectModbus(int num) {
    if (num == tp1->adr.mb_adress)
    {
        tepl_short1->dataError();
        tepl_short2->dataError();
        tepl_short3->dataError();
    }
    if (num == tp2->adr.mb_adress)
    {
        tepl_short4->dataError();
        tepl_short5->dataError();
        tepl_short6->dataError();
        tepl_short7->dataError();
    }
    if (num == tp3->adr.mb_adress)
    {
        tepl_short8->dataError();
    }
    if (num == ref->adr.mb_adress)
    {
        ref1->dataError();
        ref2->dataError();
        ref3->dataError();
        ref4->dataError();
        ref5->dataError();
        ref6->dataError();
        ref7->dataError();
    }
    if ( num == meteo->adr.mb_adress)
        meteostation->dateError();
}

//    if ( num == wt->adr.mb_adress)

bool MainWindow::new_connect_tcp(const QString &addres, quint16 port, QModbusClient *modbus_devise)
{
    if (modbus_devise->state() == QModbusDevice::UnconnectedState){
        const QUrl url = QUrl::fromUserInput(addres + ":" + QString::number(port));
        modbus_devise->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        modbus_devise->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        if (!modbus_devise->connectDevice()){
            delete modbus_devise;
            modbus_devise = nullptr;
            return false;
        }
    }
    return true;
}

void MainWindow::slotTimerConnectModbus()
{
    QString tx = "";
    if (tp1->mb->state() == QModbusDevice::ConnectedState) {
        read(tp1->adr.mb_adress, 100, 51, tp1->mb);
    }
    else {
        tx = "Нет связи с контроллером теплиц 1-3 ";
        errorConnectModbus(tp1->adr.mb_adress);
        new_connect_tcp(tp1->adr.host, _PORT.toInt(), tp1->mb);
    }
    if (tp2->mb->state() == QModbusDevice::ConnectedState) {
        read(tp2->adr.mb_adress, 100, 59, tp2->mb);
    }
    else {
        tx.length()? tx += "| Нет связи с контроллером теплиц 4-7 ": tx += "Нет связи с контроллером теплиц 4-7 ";
        errorConnectModbus(tp2->adr.mb_adress);
        new_connect_tcp(tp2->adr.host, _PORT.toInt(), tp2->mb);
    }
    if (tp3->mb->state() == QModbusDevice::ConnectedState) {
        read(tp3->adr.mb_adress, 100, 13, tp3->mb);
    }
    else {
        tx.length()? tx += "| Нет связи с контроллером теплиц 8 ": tx += "Нет связи с контроллером теплиц 8 ";
        errorConnectModbus(tp3->adr.mb_adress);
        new_connect_tcp(tp3->adr.host, _PORT.toInt(), tp3->mb);
    }
    if (ref->mb->state() == QModbusDevice::ConnectedState) {
        read(ref->adr.mb_adress, 100, 84, ref->mb);
    }
    else {
        tx.length()? tx += "| Нет связи с контроллером холодильников ": tx += "Нет связи с контроллером холодильников ";
        errorConnectModbus(ref->adr.mb_adress);
        new_connect_tcp(ref->adr.host, _PORT.toInt(), ref->mb);
    }

    if (meteo->mb->state() == QModbusDevice::ConnectedState) {
        read(meteo->adr.mb_adress, 0, 10, meteo->mb);
    }
    else {
        tx.length()? tx += "| Нет связи с контроллером метеостанции ": tx += "Нет связи с контроллером метеостанции ";
        errorConnectModbus(meteo->adr.mb_adress);
        new_connect_tcp(meteo->adr.host, _PORT.toInt(), meteo->mb);
    }
    if (wt->mb->state() == QModbusDevice::ConnectedState) {
        read(wt->adr.mb_adress, 100, 17, wt->mb);
      }
    else {
        tx.length()? tx += "| Нет связи с контроллером полива ": tx += "Нет связи с контроллером полива ";
        errorConnectModbus(wt->adr.mb_adress);
        new_connect_tcp(wt->adr.host, _PORT.toInt(), wt->mb);
      }
    if (tx.length()) {
        statusBar()->showMessage(tx, 5000);
      }

    if (tx.length()) {
        statusBar()->showMessage(tx, 5000);
    }
}

void MainWindow::read(quint8 num_device, quint16 reg, qint16 size, QModbusClient *mbclient)
{
    if(!mbclient)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit readUnit = QModbusDataUnit(QModbusDataUnit::InputRegisters, reg, size);
    QModbusReply *lastRequest = mbclient->sendReadRequest(readUnit, num_device);
    if (!lastRequest->isFinished())
    {
        if(num_device == tp1->adr.mb_adress)
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::response_1_3);
        if(num_device == tp2->adr.mb_adress)
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::response_4_7);
        if(num_device == tp3->adr.mb_adress)
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::response_8);
        if(num_device == ref->adr.mb_adress)
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::response_ref);
        if(num_device == meteo->adr.mb_adress)
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::response_meteostation);
        if(num_device == wt->adr.mb_adress)
          connect(lastRequest, &QModbusReply::finished, this, &MainWindow::responce_water);
    }
    else lastRequest->deleteLater();
}

void MainWindow::write(quint8 num_device, quint16 start_reg, const QVector<quint16> &data, QModbusClient *modbus_devise)
{
    if(!modbus_devise)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, start_reg, data.size());
    writeUnit.setValues(data);

    if (QModbusReply *lastRequest = modbus_devise->sendWriteRequest(writeUnit, num_device)) {
        if (!lastRequest->isFinished()) {
            if (lastRequest->error() == QModbusDevice::ProtocolError) {
                statusBar()->showMessage(tr("Write responce error: %1 (Mobus exception: 0x%2)")
                                         .arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16),
                                         5000);
            } else if (lastRequest->error() != QModbusDevice::NoError) {
                statusBar()->showMessage(tr("Write responce error: %1 (code: 0x%2)").
                                         arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
            }
        }
        else lastRequest->deleteLater();
    }
    else {
        statusBar()->showMessage(tr("Write error: ") + modbus_devise->errorString(), 5000);
    }
}

void MainWindow::response_1_3()
{
    QModbusReply *lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest) return;
    if (lastRequest->error() == QModbusDevice::TimeoutError)
    {
        errorConnectModbus(tp1->adr.mb_adress);
        return;
    }
    //   вывод данных контроллера теплиц 1-3
    if (lastRequest->error() == QModbusDevice::NoError){
        const QModbusDataUnit unit = lastRequest->result();
        int position = 0; // расположение данных от разных теплиц в структере данных Modbus
        const int lengDataModbusTepl = 17; // кол-во данных для одной теплицы ( с датчиком почвы для теплиц 1-3 заменить на 17 !!!!!)

        FormTeplica::ShowDate data = qreateDate(unit, position);

        QVector<qint16> v_db{};

        tepl_short1->data(data);
        tepl1->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        tepl_short2->data(data);
        tepl2->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        tepl_short3->data(data);
        tepl3->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        createQueryTepl2(dbQSql, v_db);
    }
    else if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
    }
    lastRequest->deleteLater();
}

void MainWindow::response_4_7()
{
    QModbusReply *lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest) return;
    if (lastRequest->error() == QModbusDevice::TimeoutError)
    {
        errorConnectModbus(tp2->adr.mb_adress);
        return;
    }
    //   вывод данных контроллера теплиц 4-7
    if (lastRequest->error() == QModbusDevice::NoError){
        const QModbusDataUnit unit = lastRequest->result();
        int position = 0; // расположение данных от разных теплиц в структере данных Modbus
        const int lengDataModbusTepl = 12; // кол-во данных для одной теплицы
        FormTeplica::ShowDate data;

        QVector<qint16> v_db{};

        data = qreateDate(unit, position);
        tepl_short4->data(data);
        tepl4->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        tepl_short5->data(data);
        tepl5->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        tepl_short6->data(data);
        tepl6->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        tepl_short7->data(data);
        tepl7->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

//        meteostation->dateTemperature(unit.value(48)); // убрать, когда будет подключена метеостанция

        WaterTemper::ShowDate date_wt{};
        date_wt.ControlTemper1 = unit.value(53);
        date_wt.ControlTemper2 = unit.value(54);
        date_wt.ControlTemper3 = unit.value(55);
        date_wt.ControlTemper4 = unit.value(56);
        date_wt.ControlTemper5 = unit.value(57);
        date_wt.ControlTemper6 = unit.value(58);

        wat_temper->date(date_wt);

        createQueryTepl1(dbQSql, v_db);
    }
    else if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
    }
    else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
    }
    lastRequest->deleteLater();
}

void MainWindow::response_8()
{
    QModbusReply *lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest) return;
    if (lastRequest->error() == QModbusDevice::TimeoutError)
    {
        errorConnectModbus(tp3->adr.mb_adress);
        return;
    }

    //   вывод данных контроллера теплиц 8
    if (lastRequest->error() == QModbusDevice::NoError){
        const QModbusDataUnit unit = lastRequest->result();
        int position = 0; // расположение данных от разных теплиц в структере данных Modbus
        //const int lengDataModbusTepl = 12; // кол-во данных для одной теплицы
        FormTeplica::ShowDate data = qreateDate(unit, position);

        QVector<qint16> v_db{};

        tepl_short8->data(data);
        tepl8->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);
        createQueryTepl3(dbQSql, v_db);

        arr_setting_tepl[4] = unit.value(42);
        arr_setting_tepl[5] = unit.value(41);
    }
    else if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
    }
    else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
    }
    lastRequest->deleteLater();
}

void MainWindow::response_ref()
{
    QVector<qint16> v_db{};

    QModbusReply *lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest) return;
    if (lastRequest->error() == QModbusDevice::TimeoutError)
    {
        errorConnectModbus(ref->adr.mb_adress);
        return;
    }
    //   вывод данных контроллера холодильников
    if (lastRequest->error() == QModbusDevice::NoError){

        const QModbusDataUnit unit = lastRequest->result();

        int position = 0; // расположение данных от разных теплиц в структере данных Modbus
        const int lengDataModbusTepl = 12; // кол-во данных для одного холодильника

        FormTeplica::ShowDate data;
        data = qreateDate(unit, position);
        ref1->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        ref2->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        ref3->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        ref4->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        ref5->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        position += lengDataModbusTepl;
        data = qreateDate(unit, position);
        ref6->data(data);
        v_db.push_back(data.temperature);
        v_db.push_back(data.hum);

        createQueryRef(dbQSql, v_db);

        FormTeplica::ShowDate dataSclad;
        v_db.clear();

        position += lengDataModbusTepl;
        dataSclad = qreateDate(unit, position);
        ref7->data(dataSclad);
        v_db.push_back(dataSclad.temperature);
        v_db.push_back(dataSclad.hum);
        v_db.push_back(dataSclad.light);

        // Усреднение показаний качества воздуха
        const float K = 0.1;
        if (ref7->airAveraging == 0)
            ref7->airAveraging = dataSclad.airQuality;
        else
            ref7->airAveraging = K * dataSclad.airQuality + (1.0 - K) * ref7->airAveraging;
        v_db.push_back(ref7->airAveraging);

        createQuerySclad(dbQSql, v_db);
    }
    else   if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
    }
    lastRequest->deleteLater();
}

void MainWindow::response_meteostation()
{
    QModbusReply *lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest) return;
    if (lastRequest->error() == QModbusDevice::TimeoutError)
    {
        errorConnectModbus(meteo->adr.mb_adress);
        return;
    }
    FormRain::MeteoDate date{};

    QVector<qint16> v_db{};


    //   вывод данных контроллера метеостанции
    if (lastRequest->error() == QModbusDevice::NoError){
        const QModbusDataUnit unit = lastRequest->result();

        date.wind_direction = unit.value(0);
        date.wind_speed = unit.value(1);
        date.temperature = unit.value(2);
        date.humidity = unit.value(3);
        date.gust_speed = unit.value(4);
        date.rainfall = unit.value(5);
        date.uv = unit.value(6);
        date.light = unit.value(7);
        date.pressure = unit.value(8);
        date.error = unit.value(9);

        v_db.push_back(date.temperature);
        v_db.push_back(date.humidity);
        v_db.push_back(date.pressure);
        v_db.push_back(date.wind_speed);
        v_db.push_back(date.wind_direction);
        v_db.push_back(date.rainfall);
        v_db.push_back(date.light);
        v_db.push_back(date.uv);

        createQueryMeteo(dbQSql, v_db);

    }
    else if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
        date.error = lastRequest->errorString().toInt();
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
        date.error = lastRequest->errorString().toInt();
    }

    meteostation->date(date);

    lastRequest->deleteLater();
}

void MainWindow::responce_water()
{
  QModbusReply *lastRequest = qobject_cast<QModbusReply *>(sender());
  if (!lastRequest) return;
  if (lastRequest->error() == QModbusDevice::TimeoutError)
    {
      errorConnectModbus(tp3->adr.mb_adress);
      return;
    }

  //   вывод данных контроллера полива
  if (lastRequest->error() == QModbusDevice::NoError){
      const QModbusDataUnit unit = lastRequest->result();
      water->data(unit);
    }
  else if (lastRequest->error() == QModbusDevice::ProtocolError) {
      statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                               arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode(), -1, 16), 5000);
    }
  else {
      statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                               arg(lastRequest->errorString()).arg(lastRequest->error(), -1, 16), 5000);
    }
  lastRequest->deleteLater();
}

void MainWindow::get_from_water()
{
  write(wt->adr.mb_adress, 300, water->getNewData(), wt->mb);
}

