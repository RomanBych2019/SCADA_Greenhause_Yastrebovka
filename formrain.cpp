#include "formrain.h"
//#include "formteplica.h"
//#include "formteplica_short.h"
#include "ui_formrain.h"
#include "QDebug"

FormRain::FormRain(QWidget *parent, const QString &name) :
    QWidget(parent),
    ui(new Ui::FormRain)
{
    ui->setupUi(this);
    pix_sunny = new  QPixmap (":/icons/icons/weather-sunny.png");
    pix_few_clouds = new  QPixmap (":/icons/icons/weather-few-clouds.png");
    pix_sunny_very_few_clouds = new  QPixmap (":/icons/icons/weather-sunny-very-few-clouds.png");
    pix_overcast = new  QPixmap (":/icons/icons/weather-overcast.png");
    pix_sleet = new  QPixmap (":/icons/icons/weather-showers.png");
    pix_icy = new  QPixmap (":/icons/icons/weather-icy.png");

    ui->groupBox->setTitle(name);
}

void FormRain::showIcon(int uv, int temper, int rainfall)
{
    ui->image_weather->setHidden(false);

    if (temper > -150)
    {
        if (rainfall < RAIN)
        {
            if (uv < 20)
                ui->image_weather->setPixmap(pix_overcast->scaled(ui->image_weather->width(), ui->image_weather->height(), Qt::KeepAspectRatio));
            else if (uv < 40)
                ui->image_weather->setPixmap(pix_sunny_very_few_clouds->scaled(ui->image_weather->width(), ui->image_weather->height(), Qt::KeepAspectRatio));
            else if (uv < 60)
                ui->image_weather->setPixmap(pix_few_clouds->scaled(ui->image_weather->width(), ui->image_weather->height(), Qt::KeepAspectRatio));
            else
                ui->image_weather->setPixmap(pix_sunny->scaled(ui->image_weather->width(), ui->image_weather->height(), Qt::KeepAspectRatio));
        }
        else
            ui->image_weather->setPixmap(pix_sleet->scaled(ui->image_weather->width(), ui->image_weather->height(), Qt::KeepAspectRatio));
    }
    else
        ui->image_weather->setPixmap(pix_icy->scaled(ui->image_weather->width(), ui->image_weather->height(), Qt::KeepAspectRatio));

    this->update();
}

void FormRain::date(MeteoDate &date) {
    if (date.error == 1) {
        // индикация температуры
        ui->Temperature->setText(QString::number(date.temperature / 10.0, 'f', 1));
        // индикация влажности
        ui->Hum->setText(QString::number(date.humidity));
        // индикация давления
        ui->Pressure->setText(QString::number(date.pressure / 1000.0, 'f', 3));
        // индикация скорости ветра и направления
        QString wind_direction{};
        if (date.wind_direction > 338 || date.wind_direction < 20)
            wind_direction = "С";
        else if (date.wind_direction < 67)
            wind_direction = "СВ";
        else if (date.wind_direction < 113)
            wind_direction = "В";
        else if (date.wind_direction < 158)
            wind_direction = "ЮВ";
        else if (date.wind_direction < 202)
            wind_direction = "Ю";
        else if (date.wind_direction < 247)
            wind_direction = "ЮЗ";
        else if (date.wind_direction < 292)
            wind_direction = "З";
        else
            wind_direction = "CЗ";

        ui->Wind_speed->setText(QString::number(date.wind_speed / 100.0, 'f', 1) + " (" + wind_direction + ")");
        // индикация кол-ва осадков
        ui->Rainfall->setText(QString::number(date.rainfall / 100.0, 'f', 1));
        // индикация освещенности
        ui->Light->setText(QString::number(date.light / 10));
        // индикация УФ фактора
        int uvi = 0;
        if (date.uv < 432)
            uvi = 0;
        else if (date.uv < 851)
            uvi = 1;
        else if (date.uv < 1201)
            uvi = 2;
        else if (date.uv < 1570)
            uvi = 3;
        else if (date.uv < 2017)
            uvi = 4;
        else if (date.uv < 2450)
            uvi = 5;
        else if (date.uv < 2761)
            uvi = 6;
        else if (date.uv < 3100)
            uvi = 7;
        else if (date.uv < 3512)
            uvi = 8;
        else if (date.uv < 3918)
            uvi = 9;
        else if (date.uv < 4277)
            uvi = 10;
        else if (date.uv < 4650)
            uvi = 11;
        else if (date.uv < 5029)
            uvi = 12;
        else
            uvi = 13;
        ui->UV->setText(QString::number(uvi));
        showIcon(date.uv, date.temperature, date.rainfall);

    }
    else {
        dateError();
    }
}

void FormRain::dateError()
{
    ui->Temperature->setText("---");
    ui->Hum->setText("---");
    ui->Pressure->setText("---");
    ui->Wind_speed->setText("---");
    ui->Rainfall->setText("---");
    ui->Light->setText("---");
    ui->UV->setText("---");
    ui->image_weather->setHidden(true);
}

FormRain::~FormRain()
{
    delete ui;
}

void FormRain::dateTemperature(qint16 t) {
        // индикация температуры
        ui->Temperature->setText(QString::number(t / 100.0, 'f', 1));
}
