#ifndef WATERTEMPER_H
#define WATERTEMPER_H

#include <QObject>
#include <QWidget>

namespace Ui {
  class Form_temper;
}
class WaterTemper : public QWidget
{
  Q_OBJECT

public:
  WaterTemper(QWidget *parent);
  struct ShowDate
  {
    qint16 ControlTemper1;
    qint16 ControlTemper2;
    qint16 ControlTemper3;
    qint16 ControlTemper4;
    qint16 ControlTemper5;
    qint16 ControlTemper6;
  };
signals:
  void mousePress();
public:
  void date( ShowDate &date) const;
private:
  void mousePressEvent(QMouseEvent *event) override;
  Ui::Form_temper *ui;
};

#endif // WATERTEMPER_H
