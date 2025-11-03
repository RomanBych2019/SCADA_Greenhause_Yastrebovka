#ifndef CHARTS_TEPL_H
#define CHARTS_TEPL_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGraphicsGridLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtCharts/QLineSeries>
#include <QDialog>
#include <QSqlDatabase>

#include "qcustomplot.h"
#include <QDebug>


QT_CHARTS_USE_NAMESPACE

namespace Ui {
  class Charts;
}

class Charts : public QDialog
{
  Q_OBJECT

public:
  explicit Charts(QSqlDatabase &db, QWidget *parent = nullptr);
  ~Charts();

private slots:

  void slotRangeChanged (const QCPRange &newRange);
  void slotMousePress(QMouseEvent * event);

  void on_checkBoxTepl1_stateChanged(int arg1);
  void on_checkBoxTepl_2_stateChanged(int arg1);
  void on_checkBoxTepl_3_stateChanged(int arg1);
  void on_checkBoxTepl_4_stateChanged(int arg1);
  void on_checkBoxTepl_5_stateChanged(int arg1);
  void on_checkBoxTepl_6_stateChanged(int arg1);
  void on_checkBoxTepl_7_stateChanged(int arg1);
  void on_checkBoxTepl_8_stateChanged(int arg1);

  void on_checkBox_temper_stateChanged(int arg1);

  void on_pushButton_clicked();

  void on_checkBoxOutDor_stateChanged(int arg1);

  void on_checkBox_hum_stateChanged(int arg1);

  void on_pushButtonAutoScale_clicked();

private:
  QChart *m_chart;

  enum GRAPH_TEPL{
      Tepl1 = 0,
      Tepl2 = 1,
      Tepl3 = 2,
      Tepl4 = 3,
      Tepl5 = 4,
      Tepl6 = 5,
      Tepl7 = 6,
      Tepl8 = 7,
      Outdor = 8,
  } GRAPH_TEPL;

  QList<QLineSeries *> m_series;

  QChartView *m_chartView;
  QGridLayout *m_mainLayout;
  QGridLayout *m_fontLayout;

  Ui::Charts *ui;
  QSqlDatabase *dbQSgl;

  QCustomPlot *customPlot;    // Объявляем графическое полотно
  QCPCurve *verticalLine;     // Объявляем объект для вертикальной линии
  QCPItemTracer *tracer;      // Трасировщик по точкам графика

  QVector <QCPGraph*> vec_grapics_temper{};
  QVector <QCPGraph*> vec_grapics_hum{};
  QVector <QCheckBox*> vec_checkbox_tepl{};
  void set_visible_graph(bool st, QCPGraph *graph_t, QCPGraph *graph_h);
  virtual void keyReleaseEvent(QKeyEvent *event);
  virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CHARTS_TEPL_H
