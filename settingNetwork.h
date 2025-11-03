#ifndef SETTINGNETWORK_H
#define SETTINGNETWORK_H

#include <QAbstractButton>
#include <QDialog>

#include <soundplayer.h>

struct adress
{
  QString host;
  quint8 mb_adress;
};

namespace Ui {
  class SettingNetwork;
}

class SettingNetwork : public QDialog
{
  Q_OBJECT

public:
  explicit SettingNetwork(QWidget *parent, adress &tepl1, adress &tepl2, adress &tepl3, adress &ref, adress &meteo, adress &wt);
  QVector<adress> getServerAdress ();
  ~SettingNetwork();

private slots:
  void on_buttonBox_accepted();
  void on_radioButton_1_3_clicked();
  void on_radioButton_4_7_clicked();
  void on_radioButton_8_clicked();
  void on_buttonBox_clicked(QAbstractButton *button);

  void on_radioButton_ref_clicked();

  void on_radioButton_meteo_clicked();

  void on_radioButton_wt_clicked();

private:
  Ui::SettingNetwork *ui;
  quint8 teplNumber = 0;

  adress _tepl1;
  adress _tepl2;
  adress _tepl3;
  adress _ref;
  adress _meteo;
  adress _wt;
};

#endif // SETTINGNETWORK_H
