#include <QtCharts>
#include <QDateTime>
#include <QSqlQuery>
#include <QDebug>
#include <QPen>

#include "charts_sclad.h"
#include "ui_charts_sclad.h"

Charts_Sclad::Charts_Sclad(QSqlDatabase &db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Charts_Sclad)
{
    ui->setupUi(this);

    ui->dateTimeEditBeg->setDateTime(QDateTime::currentDateTime().addSecs(-3600 * 2));
    ui->dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

    customPlot = new QCustomPlot(); // Инициализируем графическое полотно
    ui->verticalLayout_3->addWidget(customPlot);  // Устанавливаем customPlot в окно проложения

    customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси

    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d MMMM\nyyyy");
    customPlot->xAxis->setTicker(dateTicker);

    // Настраиваем шрифт по осям координат
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    /* Делаем видимыми оси X и Y по верхней и правой границам графика,
     * но отключаем на них тики и подписи координат
     * */
    customPlot->xAxis->setVisible(true);
    customPlot->yAxis->setVisible(true);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setVisible(true);


    customPlot->yAxis->setTickLabelColor(QColor(Qt::black)); // Черный цвет подписей тиков по Оси Y
    customPlot->legend->setVisible(true);   //Включаем Легенду графика
    customPlot->yAxis->setLabel("Температура, ˚C");

    // Устанавливаем Легенду в левый верхний угол графика
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    customPlot->yAxis2->setTickLabelColor(QColor(Qt::red));
    customPlot->yAxis2->setRange(0.0, 2000.0);
    customPlot->yAxis2->setLabel("Освещенность, люм. Влажность, %. Качество воздуха");


// Добавление графиков температур
    for (int i = 0; i < NUMBER_ACTIV_SCLAD; i++) {
        QCPGraph *graphic;          // Объявляем график
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
        graphic->setName("Температура_" + QString::number(i+1) + ", ˚C");       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine); //
        vec_grapics_temper.push_back(graphic);
    }

    // Добавление графиков влажности
    for (int i = 0; i < NUMBER_ACTIV_SCLAD; i++) {
        QCPGraph *graphic;          // Объявляем график
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis2);
        graphic->setName("Влажность_" + QString::number(i+1) + ", %");       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine); //
        graphic->setVisible(false);
        vec_grapics_hum.push_back(graphic);
    }

    // Добавление графиков освещенности
    for (int i = 0; i < NUMBER_ACTIV_SCLAD; i++) {
        QCPGraph *graphic;          // Объявляем график
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis2);
        graphic->setName("Освещенность_" + QString::number(i+1));       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine); //
        graphic->setVisible(true);
        vec_grapics_light.push_back(graphic);
    }

    // Добавление графиков качества воздуха
    for (int i = 0; i < NUMBER_ACTIV_SCLAD; i++) {
        QCPGraph *graphic;          // Объявляем график
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis2);
        graphic->setName("Качество воздуха_" + QString::number(i+1));       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine); //
        graphic->setVisible(true);
        vec_grapics_air.push_back(graphic);
    }

    QPen Pen;
    Pen.setWidthF(2);                //ширина линии
    Pen.setColor(QColor(Qt::blue)); // Устанавливаем цвет графика
    vec_grapics_temper.at(GRAPH_SCLAD::Sclad1)->setPen(Pen);
    Pen.setColor(QColor(Qt::yellow));

    Pen.setColor(QColor(Qt::darkBlue));
    vec_grapics_hum.at(GRAPH_SCLAD::Sclad1)->setPen(Pen);

    Pen.setColor(QColor(Qt::gray));
    vec_grapics_light.at(GRAPH_SCLAD::Sclad1)->setPen(Pen);

    Pen.setColor(QColor(Qt::red));
    vec_grapics_air.at(GRAPH_SCLAD::Sclad1)->setPen(Pen);


    vec_checkbox_tepl.push_back(ui->checkBoxTepl1);


    // Подключаем сигнал от Оси X об изменении видимого диапазона координат к СЛОТу для переустановки формата времени оси.
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(slotRangeChanged(QCPRange)));

    // Подключаем сигналы событий мыши от полотна графика к слотам для их обработки
    connect(customPlot, &QCustomPlot::mouseDoubleClick, this, &Charts_Sclad::slotMousePress);

    // Инициализируем трассировщик
    tracer = new QCPItemTracer(customPlot);

    dbQSql = &db;
}
Charts_Sclad::~Charts_Sclad()
{
    delete ui;
}

void Charts_Sclad::slotRangeChanged(const QCPRange &newRange)
{
    /* Если область видимости графика меньше одного дня,
     * то отображаем часы и минуты по Оси X,
     * в противном случае отображаем дату "День Месяц Год"
     * */
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat((newRange.size() <= 86400)? "hh:mm" : "d MMMM\nyyyy");
    customPlot->xAxis->setTicker(dateTicker);
}

void Charts_Sclad::slotMousePress(QMouseEvent *event)
{
    // Определяем координату X на графике, где был произведён клик мышью
    double coordX = customPlot->xAxis->pixelToCoord(event->pos().x());

    // По координате X клика мыши определим ближайшие координаты для трассировщика
    tracer->setGraphKey(coordX);
    tracer->updatePosition();

    // Выводим координаты точки графика, где установился трассировщик, в lineEdit
    QDateTime timestamp;
    timestamp.setTime_t(tracer->position->key());

   int counter = 0;

    for ( int i = 0; i < vec_grapics_temper.size();  i++) {
        tracer->setGraph(customPlot->graph(counter));   // Трассировщик будет работать с графиком i
        tracer->setGraphKey(coordX);
        tracer->updatePosition();
        customPlot->graph(counter)->setName("Температура_" + QString::number(i + 1) + ": " + QString::number(tracer->position->value()) + "˚C");
        counter++;
    }
    for ( int i = 0; i < vec_grapics_hum.size();  i++) {
        tracer->setGraph(customPlot->graph(counter));
        tracer->setGraphKey(coordX);
        tracer->updatePosition();
        customPlot->graph(counter)->setName("Влажность_" + QString::number(i + 1) + ": " + QString::number(tracer->position->value()) + "%");
        counter ++;
    }
    for ( int i =  0; i < vec_grapics_light.size();  i++) {
        tracer->setGraph(customPlot->graph(counter));
        tracer->setGraphKey(coordX);
        tracer->updatePosition();
        customPlot->graph(counter)->setName("Освещенность_" + QString::number(i + 1) + ": " + QString::number(tracer->position->value()));
        counter++;
    }
    for ( int i = 0; i < vec_grapics_light.size();  i++) {
        tracer->setGraph(customPlot->graph(counter));
        tracer->setGraphKey(coordX);
        tracer->updatePosition();
        customPlot->graph(counter)->setName("Качество воздуха_" + QString::number(i + 1) + ": " + QString::number(tracer->position->value()));
        counter++;
    }
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl1_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad1), vec_grapics_hum.at(GRAPH_SCLAD::Sclad1));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl_2_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad2), vec_grapics_hum.at(GRAPH_SCLAD::Sclad2));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl_3_stateChanged(int arg1)
{  
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad3), vec_grapics_hum.at(GRAPH_SCLAD::Sclad3));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl_4_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad4), vec_grapics_hum.at(GRAPH_SCLAD::Sclad4));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl_5_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad5), vec_grapics_hum.at(GRAPH_SCLAD::Sclad5));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl_6_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad6), vec_grapics_hum.at(GRAPH_SCLAD::Sclad6));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts_Sclad::on_checkBoxTepl_7_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad7), vec_grapics_hum.at(GRAPH_SCLAD::Sclad7));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}
void Charts_Sclad::on_checkBoxTepl_8_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_SCLAD::Sclad6), vec_grapics_hum.at(GRAPH_SCLAD::Sclad6));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}


void Charts_Sclad::on_checkBox_temper_stateChanged(int arg1)
{
    on_off_graph(arg1, vec_grapics_temper);
}

void Charts_Sclad::set_visible_graph(bool st, QCPGraph *graph_t, QCPGraph *graph_h)
{
    bool res = st && ui->checkBox_temper->checkState();
    graph_t->setVisible(res);
    res? graph_t->addToLegend(): graph_t->removeFromLegend();
    res = st && ui->checkBox_hum->checkState();
    graph_h->setVisible(res);
    res? graph_h->addToLegend(): graph_h->removeFromLegend();
}


void Charts_Sclad::on_pushButton_clicked()
{
    //  if (ui->checkBoxTepl1->checkState() == Qt::CheckState::Unchecked && ui->checkBoxTepl1_2->checkState() == Qt::CheckState::Unchecked)
    //    return;
    QString begin_ = ui->dateTimeEditBeg->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString end_ = ui->dateTimeEditEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query = QSqlQuery(*dbQSql);
    QString sel = "SELECT DateTime, Склад_1t, Склад_1h, Склад_1l, Склад_1a FROM Sclad WHERE DateTime BETWEEN '" + begin_ + "' and '" + end_ + "'";
    query.exec(sel);

    QVector <QVector<double>> data(5);


    while (query.next())
    {
        data[0].push_back(query.value(0).toDateTime().toTime_t());
        for (int i = 1; i < data.size(); i++)
            data[i].push_back(query.value(i).toDouble() / 100.0);
    }

    vec_grapics_temper.at(GRAPH_SCLAD::Sclad1)->setData(data.at(0), data.at(1));

     vec_grapics_hum.at(GRAPH_SCLAD::Sclad1)->setData(data.at(0), data.at(2)); // Устанавливаем данные

     vec_grapics_light.at(GRAPH_SCLAD::Sclad1)->setData(data.at(0), data.at(3)); // Устанавливаем данные
     vec_grapics_air.at(GRAPH_SCLAD::Sclad1)->setData(data.at(0), data.at(4)); // Устанавливаем данные

     on_pushButtonAutoScale_clicked();
}

void Charts_Sclad::on_checkBox_hum_stateChanged(int arg1)
{
    on_off_graph(arg1, vec_grapics_hum);
}

void Charts_Sclad::on_pushButtonAutoScale_clicked()
{
    customPlot->rescaleAxes();      // Масштабируем график по данным
    customPlot->yAxis->setRange(customPlot->yAxis->range().lower > 0 ? 0.0 : customPlot->yAxis->range().lower - 0.2, customPlot->yAxis->range().upper * 2);
    customPlot->yAxis2->setRange(0, customPlot->yAxis2->range().upper * 1.02);
    customPlot->replot();
}


void Charts_Sclad::on_checkBox_air_stateChanged(int arg1)
{
    on_off_graph(arg1, vec_grapics_air);
}


void Charts_Sclad::on_checkBox_light_stateChanged(int arg1)
{
    on_off_graph(arg1, vec_grapics_light);
}


void Charts_Sclad::on_off_graph(int arg,  QVector <QCPGraph*> &graph) {
    if(!arg)
        for(auto gr : graph)
        {
            gr->setVisible(false);
        }
    else
        for(auto i = 0; i < vec_checkbox_tepl.size(); i++)
        {
            graph.at(i)->setVisible(vec_checkbox_tepl.at(i)->isChecked());
        }
    customPlot->replot();
}


// Включаем перетаскивание по X
void Charts_Sclad::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
        customPlot->axisRect()->setRangeDrag(Qt::Horizontal) ;
    }
}

// Включаем перетаскивание по Y
void Charts_Sclad::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control){
        customPlot->axisRect()->setRangeZoom(Qt::Vertical);
        customPlot->axisRect()->setRangeDrag(Qt::Vertical);
    }
}
