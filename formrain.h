#ifndef FORMRAIN_H
#define FORMRAIN_H

#include <QPixmap>
#include <QWidget>

//#include "formteplica.h"
//#include "formteplica_short.h"

//#define SUN  1
//#define HUMID  2
//#define RAIN   3

namespace Ui {
class FormRain;
}

class FormRain : public QWidget
{
    Q_OBJECT

public:
    explicit FormRain(QWidget *parent = nullptr, const QString &name  = nullptr);

    struct MeteoDate
    {
        int wind_direction;
        int wind_speed;
        qint16 temperature;
        int humidity;
        int gust_speed;
        int rainfall;
        int uv;
        int light;
        int pressure;
        int error;
    };

    void showIcon(int uv, int temper, int rainfall);
    void date(MeteoDate &date);
    void dateError();
    void dateTemperature(qint16 t);


    ~FormRain();

private:
    QPixmap *pix_sunny;
    QPixmap *pix_few_clouds;
    QPixmap *pix_sunny_very_few_clouds;
    QPixmap *pix_overcast;
    QPixmap *pix_sleet;
    QPixmap *pix_icy;
    Ui::FormRain *ui;
    const qint16 RAIN = 300; // осадки


};

#endif // FORMRAIN_H
