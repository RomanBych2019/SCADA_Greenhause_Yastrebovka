#include <QVariant>
#include <iostream>

#include "weather_dounload.h"

Weather_dounload::Weather_dounload(QObject *pobj/*=0*/) : QObject(pobj)
{
  mngr = new QNetworkAccessManager(this);
  connect(mngr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotFinished(QNetworkReply*)));
}

bool Weather_dounload::online()
{
  /*
   * QNetworkConfigurationManager *c = new QNetworkConfigurationManager (this);
  return c->isOnline();
*/
      return true;
}
void  Weather_dounload::sendData()
{
//  url =  "https://api.openweathermap.org/data/2.5/weather?appid=dd7735c2f2ce6f4c12e3ae0cc087c72e&lon=36.393515&lat=54.6043066667&units=metric";
//  QVariant head("897dddd3-f0d8-47a4-a692-30c2baad0116");

//  QUrl url;
//  url =  "https://api.weather.yandex.ru/v2/informers?lat=54.5293000&lon=36.2754200&lang=ru_RU";
  QNetworkRequest request;
  request.setUrl(QUrl("https://api.weather.yandex.ru/v2/informers?lat=54.5293000&lon=36.2754200&lang=ru_RU"));
  request.setRawHeader("X-Yandex-API-Key", "fe8a0d7d-44b1-4645-9e8c-14588d39e67f");
  repl = mngr->get(request);
}

void Weather_dounload::slotFinished(QNetworkReply *prn) {

  if (prn->error() == QNetworkReply::NoError)
    {emit done(prn->readAll());
    }
      prn->deleteLater();
}
