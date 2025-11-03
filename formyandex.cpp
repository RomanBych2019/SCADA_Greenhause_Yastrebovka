#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>

#include "formyandex.h"
#include "ui_formyandex.h"

FormYandex::FormYandex(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FormYandex)
{
  ui->setupUi(this);
}

void FormYandex::setData(const QByteArray& text)
{
  QDate cd = QDate::currentDate(); // возвращаем текущую дату
  QTime ct = QTime::currentTime(); // возвращаем текущее время
  ui->label_dateYandex->setText("Прогноз получен: " + ct.toString() + "  " + cd.toString("MMMM, dd"));

  QJsonDocument d = QJsonDocument::fromJson(text);
  QJsonObject sett = d.object();
  QJsonObject fact = sett["fact"].toObject();
  QJsonObject forecast = sett["forecast"].toObject();

  QString dateStr = fact["date"].toString();
  QString fmt = "yyyy-MM-dd";
  QDateTime dt;
  dt.setTime(QTime::fromString(fact["obs_time"].toString()));
  QString date = dt.toString("MMMM, dd");
  ui->label_25->setText(date);
  ui->label_15->setText(QString::number(fact["temp"].toDouble(),'f',1));
  ui->label_17->setText(QString::number(fact["humidity"].toDouble(),'f',0));
  ui->label_16->setText(QString::number(fact["wind_speed"].toDouble(),'f',0));

  QJsonArray parts = forecast["parts"].toArray();
  QJsonObject parts1 = parts[0].toObject();
  QJsonObject parts2 = parts[1].toObject();

  if (parts1["temp_min"].toDouble() < parts2["temp_min"].toDouble())
    ui->label_20->setText(QString::number(parts1["temp_min"].toDouble(),'f',1));
  else
    ui->label_20->setText(QString::number(parts2["temp_min"].toDouble(),'f',1));

  if (parts1["temp_max"].toDouble() > parts2["temp_max"].toDouble())
    ui->label_19->setText(QString::number(parts1["temp_max"].toDouble(),'f',1));
  else
    ui->label_19->setText(QString::number(parts2["temp_max"].toDouble(),'f',1));
  ui->label_21->setText(QString::number(parts1["prec_mm"].toDouble() + parts2["prec_mm"].toDouble(),'f',1));
}

FormYandex::~FormYandex()
{
  delete ui;
}
