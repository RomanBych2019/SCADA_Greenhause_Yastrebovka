#ifndef FORMYANDEX_H
#define FORMYANDEX_H

#include <QWidget>

namespace Ui {
  class FormYandex;
}

class FormYandex : public QWidget
{
  Q_OBJECT

public:
  explicit FormYandex(QWidget *parent = nullptr);
  void setData(const QByteArray& text);
  ~FormYandex();

private:
  Ui::FormYandex *ui;
};

#endif // FORMYANDEX_H
