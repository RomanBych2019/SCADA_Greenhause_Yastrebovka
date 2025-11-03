#ifndef WEATHER_DOUNLOAD_H
#define WEATHER_DOUNLOAD_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>


class Weather_dounload : public QObject
{
  Q_OBJECT
private:
  QNetworkAccessManager *mngr;
  QNetworkReply *repl;
public:

  Weather_dounload (QObject *pobj = 0);
  bool online();
  void sendData();

signals:
  void done(const QByteArray&);
//  void done();
  void error();

private slots:
  void slotFinished(QNetworkReply*);
};

#endif // WEATHER_DOUNLOAD_H
