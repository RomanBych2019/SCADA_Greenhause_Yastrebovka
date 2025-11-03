#include <QDebug>
#include <QString>
#include <QRegularExpression>

#include "formteplica.h"
#include "ui_formteplica.h"

FormTeplica::FormTeplica(QDialog *parent, quint8 id, quint16 mb_adr_write) :
  QDialog(parent),
  ui(new Ui::FormTeplica)
{
  ui->setupUi(this);
  _id = id;
  _mb_adr_write = mb_adr_write;
  ui->dial_UstTemper->setNotchesVisible(true);
  ui->dial_UstTemper->setNotchTarget(6);
  ui->dial_Ust_Win->setNotchesVisible(true);
  ui->dial_Ust_Win->setNotchTarget(6);
  ui->dial_Ust_Heat->setNotchTarget(6);

  ui->dial_Ust_Heat->setNotchesVisible(true);
  QString str = "Настройка теплицы " + QString::number(id);
  this->setWindowTitle(str);
  ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: cornflowerblue;}");
 }

FormTeplica::~FormTeplica()
{
  delete ui;
}

void FormTeplica::setTitle(QString &name)
{
  ui->groupBox->setTitle(name);
}

void FormTeplica::setUnVisibleUstWin(bool flag)
{
  ui->progressBar->setEnabled(!flag);
  ui->label_7->setEnabled(!flag);
  ui->label_23->setEnabled(!flag);
  ui->dial_Ust_Win->setEnabled(!flag);
  ui->UstavkaWin->setEnabled(!flag);
  ui->radioButton_win->setEnabled(!flag);
  ui->radioButton_decrease_in_humidity->setEnabled(!flag);
  ui->label_8->setEnabled(!flag);
  ui->Setting_window_3->setEnabled(!flag);
  ui->Setting_window->setEnabled(!flag);
  ui->label_12->setEnabled(!flag);
  ui->line_8->setEnabled(!flag);
}

void FormTeplica::data(ShowDate &data) const
{
  if(!this->isVisible())
  {
    if (data.errTemperature == 1){
        // индикация температуры
        ui->Temperature->setText(QString::number(data.temperature / 100.0, 'f', 1) + " " + '\xb0' + "C");
        // индикация влажности
        ui->Hum->setStyleSheet("color: black;");
        ui->Hum->setText(QString::number(data.hum / 100.0, 'f', 1) + " %");
      }
    else {
        ui->Temperature->setStyleSheet("color: crimson;");
        ui->Hum->setStyleSheet("color: crimson;");
        ui->Temperature->setText(QString::number(data.errTemperature, 16));
        ui->Hum->setText("---");
      }

    //  индикация данных почвы
    ui->soilT->setText(QString::number(data.soilSensorT / 10.0, 'f', 1) + " " + '\xb0' + "C");
    ui->soilH->setText(QString::number(data.soilSensorH / 10.0, 'f', 1) + " %");
    ui->soilPH->setText(QString::number(data.soilSensorC, 'd', 1));
    ui->soilC->setText(QString::number(data.soilSensorS, 'd', 1));

    // индикация уставок
    ui->dial_UstTemper->setValue((data.ustavkaPump / 100.0 - 3) * (50 - 0) / ((28 - 3) + 0));
    ui->Ustavka->setText(QString::number(data.ustavkaPump / 100.0, 'f', 1) + " " + '\xb0' + "C");
    ui->dial_Ust_Win->setMinimum(data.ustavkaPump / 50.0 + 2);
    ui->dial_Ust_Win->setMaximum(data.ustavkaPump / 50.0 + 20);
    ui->dial_Ust_Win->setValue(data.ustavkaWin / 50);

    ui->UstavkaWin->setText(QString::number(data.ustavkaWin / 100.0, 'f', 1) + " " + '\xb0' + "C");

    ui->UstavkaHeat->setText(QString::number(data.ustavkaHeat / 100.0, 'f', 1) + " " + '\xb0' + "C");
    ui->dial_Ust_Heat->setMinimum(data.ustavkaPump / 50.0 - 20);
    ui->dial_Ust_Heat->setMaximum(data.ustavkaPump / 50.0 - 2);
    ui->dial_Ust_Heat->setValue(data.ustavkaHeat / 50);



    // индикация уровня открытия окна
    ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: cornflowerblue;}");
    ui->progressBar->setValue(data.levelwindow);
    ui->Setting_window->setValue(data.levelwindow);

    // кнопка Обогрев
    QFont font = ui->pushButtonPump->font();
    if (data.pump) {
        font.setBold(true);
        ui->pushButtonPump->setFont(font);
        ui->pushButtonPump->setText("ВКЛ");
        ui->pushButtonPump->setStyleSheet("background-color: tomato;");
      }
    else {
        font.setBold(false);
        ui->pushButtonPump->setFont(font);
        ui->pushButtonPump->setText("ВЫКЛ");
        ui->pushButtonPump->setStyleSheet("background-color: ;");
      }
    // кнопка Дополнительный обогрев
    font = ui->pushButtonHeat->font();
    if (data.heat) {
        font.setBold(true);
        ui->pushButtonHeat->setFont(font);
        ui->pushButtonHeat->setText("ВКЛ");
        ui->pushButtonHeat->setStyleSheet("background-color: tomato;");
      }
    else {
        font.setBold(false);
        ui->pushButtonHeat->setFont(font);
        ui->pushButtonHeat->setText("ВЫКЛ");
        ui->pushButtonHeat->setStyleSheet("background-color: ;");
      }
    // кнопки выбора режима
    switch (data.mode) {
      case Mode::AUTO:
        ui->radioButton_auto->setChecked(true);
        ui->Setting_window->setEnabled(false);
        ui->pushButtonPump->setEnabled(false);
        ui->pushButtonHeat->setEnabled(false);
        break;
      case Mode::MANUAL:
        ui->radioButton_manual->setChecked(true);
        ui->pushButtonPump->setEnabled(true);
        ui->pushButtonHeat->setEnabled(true);
        break;
      case Mode::AIR:
        ui->radioButton_win->setChecked(true);
        ui->Setting_window->setEnabled(false);
        ui->pushButtonPump->setEnabled(false);
        ui->pushButtonHeat->setEnabled(false);
        break;
      case Mode::DECREASE_IN_HUMIDITY:
        ui->radioButton_decrease_in_humidity->setChecked(true);
        ui->Setting_window->setEnabled(false);
        ui->pushButtonPump->setEnabled(false);
        ui->pushButtonHeat->setEnabled(false);
        break;
      default:
        break;
      }
    // индикация значения гистерезиса
    ui->Setting_window_2->setValue(data.hyster / 10);
    ui->label_11->setText(QString::number(data.hyster / 100.0, 'f', 1) + " " + '\xb0' + "C");
    // индикация времени открытия окна
    ui->Setting_window_3->setValue(data.openTimeWin);
    ui->label_12->setText(QString::number(data.openTimeWin) + "  сек");
  }
}

QLabel *FormTeplica::getTemperature()
{
  return ui->Temperature;
}
QLabel *FormTeplica::getHum()
{
  return ui->Hum;
}

QLabel *FormTeplica::getUstavka()
{
  return ui->Ustavka;
}

QLabel *FormTeplica::getUstavkaWin()
{
  return ui->UstavkaWin;
}
QDial *FormTeplica::getdial_UstTemper()
{
  return ui->dial_UstTemper;
}
QDial *FormTeplica::getdial_UstWin()
{
  return ui->dial_Ust_Win;
}

QProgressBar *FormTeplica::getprogressBar()
{
  return ui->progressBar;
}

quint16 FormTeplica::getmb_adr_write()
{
  return _mb_adr_write;
}

QVector<quint16> FormTeplica::getNewData()
{
  QVector<quint16> vector;
  vector.push_back(1);
  vector.push_back(getNumberFromQString(ui->Ustavka->text()) * 100); // уставка температуры насоса
  vector.push_back(getNumberFromQString(ui->UstavkaHeat->text()) * 100); // уставка температуры доп.нагревателя
  vector.push_back(getNumberFromQString(ui->UstavkaWin->text()) * 100); // уставка температуры окна
  vector.push_back(ui->Setting_window->value()); // уровень открытия окна
  if(ui->radioButton_auto->isChecked()){    // режим теплицы
        vector.push_back(0);
      }
    if(ui->radioButton_manual->isChecked()){
        vector.push_back(1);
      }
    if(ui->radioButton_win->isChecked()){
        vector.push_back(2);
      }
    if(ui->radioButton_decrease_in_humidity->isChecked()){
        vector.push_back(3);
      }
    ui->pushButtonPump->text() == "ВКЛ"? vector.push_back(1): vector.push_back(0);
    ui->pushButtonHeat->text() == "ВКЛ"? vector.push_back(1): vector.push_back(0);
    vector.push_back(getNumberFromQString(ui->label_11->text()) * 100); // уставка гистерезиса
    vector.push_back(getNumberFromQString(ui->label_12->text())); // уставка времени открытия окна
    return vector;
 }

void FormTeplica::on_buttonBox_accepted()
{
  QDialog::accept();
}

void FormTeplica::on_buttonBox_rejected()
{
  QDialog::close();
}

void FormTeplica::on_radioButton_auto_clicked() const
{
    ui->Setting_window->setEnabled(false);
    ui->pushButtonPump->setEnabled(false);
    ui->pushButtonHeat->setEnabled(false);
}

void FormTeplica::on_radioButton_manual_clicked() const
{
  if (ui->dial_Ust_Win->isEnabled()) {
      ui->Setting_window->setEnabled(true);
    }
  ui->pushButtonPump->setEnabled(true);
  ui->pushButtonHeat->setEnabled(true);
}

void FormTeplica::on_radioButton_win_clicked() const
{
    ui->Setting_window->setValue(25);
    ui->Setting_window->setEnabled(false);
    ui->pushButtonPump->setEnabled(false);
    ui->pushButtonHeat->setEnabled(false);
}

void FormTeplica::on_radioButton_decrease_in_humidity_clicked() const
{
  ui->Setting_window->setValue(25);
  ui->Setting_window->setEnabled(false);

  QFont font = ui->pushButtonPump->font();
  font.setBold(true);
  ui->pushButtonPump->setFont(font);
  ui->pushButtonPump->setText("ВКЛ");
  ui->pushButtonPump->setStyleSheet("background-color: tomato;");  ui->pushButtonPump->setEnabled(false);
  ui->pushButtonHeat->setEnabled(false);
}

void FormTeplica::on_dial_UstTemper_valueChanged(int value)
{
  ui->Ustavka->setText(QString::number(3.0 + 0.5 * value, 'f', 1) + " " + '\xb0' + "C");

  ui->dial_Ust_Win->setMinimum(10 + value);
  ui->dial_Ust_Win->setMaximum(26 + value);
  ui->dial_Ust_Win->setValue(0);

  ui->dial_Ust_Heat->setMaximum(value + 2);
  if (value - 20 > 0)
    ui->dial_Ust_Heat->setMinimum(value - 20);
  else ui->dial_Ust_Heat->setMinimum(2);
  ui->dial_Ust_Heat->setValue(value);

  ui->UstavkaWin->setText(QString::number(ui->dial_Ust_Win->value() / 2.0, 'f', 1) + " " + '\xb0' + "C");
  ui->UstavkaHeat->setText(QString::number(ui->dial_Ust_Heat->value() / 2.0, 'f', 1) + " " + '\xb0' + "C");
}

void FormTeplica::on_dial_Ust_Win_valueChanged(int value)
{
  ui->UstavkaWin->setText(QString::number(value / 2.0, 'f', 1) + " " + '\xb0' + "C");
}

void FormTeplica::on_dial_Ust_Heat_valueChanged(int value)
{
  ui->UstavkaHeat->setText(QString::number(value / 2.0, 'f', 1) + " " + '\xb0' + "C");
}

void FormTeplica::on_Setting_window_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

void FormTeplica::on_pushButtonPump_clicked()
{
  QFont font = ui->pushButtonPump->font();
  if (ui->pushButtonPump->text() == "ВЫКЛ")
    {
      font.setBold(true);
      ui->pushButtonPump->setFont(font);
      ui->pushButtonPump->setText("ВКЛ");
      ui->pushButtonPump->setStyleSheet("background-color: tomato;");
    }
  else {
      font.setBold(false);
      ui->pushButtonPump->setFont(font);
      ui->pushButtonPump->setText("ВЫКЛ");
      ui->pushButtonPump->setStyleSheet("background-color: ;");
    }
}

void FormTeplica::on_pushButtonHeat_clicked()
{
  QFont font = ui->pushButtonHeat->font();
  if (ui->pushButtonHeat->text() == "ВЫКЛ")
    {
      font.setBold(true);
      ui->pushButtonHeat->setFont(font);
      ui->pushButtonHeat->setText("ВКЛ");
      ui->pushButtonHeat->setStyleSheet("background-color: tomato;");
    }
  else {
      font.setBold(false);
      ui->pushButtonHeat->setFont(font);
      ui->pushButtonHeat->setText("ВЫКЛ");
      ui->pushButtonHeat->setStyleSheet("background-color: ;");
    }
}

void FormTeplica::on_Setting_window_2_valueChanged(int value)
{
    ui->label_11->setText(QString::number(value / 10.0, 'f', 1) + " " + '\xb0' + "C");
}

void FormTeplica::on_Setting_window_3_valueChanged(int value)
{
  ui->label_12->setText(QString::number(value) + "  сек");
}

float FormTeplica::getNumberFromQString(const QString &xString)
{
  QRegExp xRegExp("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
  xRegExp.indexIn(xString);
  QStringList xList = xRegExp.capturedTexts();
  if (true == xList.empty()) {
    return 0.0;
  }
  return xList.begin()->toFloat();
}
