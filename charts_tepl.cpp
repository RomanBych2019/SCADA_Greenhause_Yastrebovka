#include <QtCharts>
#include <QDateTime>
#include <QSqlQuery>
#include <QDebug>
#include <QPen>

#include "charts_tepl.h"
#include "ui_charts_tepl.h"

Charts::Charts(QSqlDatabase &db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Charts)
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


    customPlot->xAxis->setVisible(true);
    customPlot->yAxis->setVisible(true);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setVisible(true);

    customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y (температура)
    customPlot->legend->setVisible(true);   //Включаем Легенду графика
    customPlot->yAxis->setLabel("Температура, ˚C");

    // Устанавливаем Легенду в левый верхний угол графика
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    customPlot->yAxis2->setTickLabelColor(QColor(Qt::blue)); // Синий цвет подписей тиков по Оси Y (температура)
    customPlot->yAxis2->setRange(0.0, 100.0);
    customPlot->yAxis2->setLabel("Влажность, %");

    // Добавление графиков температур
    for (int i = 0; i < 8; i++) {
        QCPGraph *graphic;          // Объявляем график для теплиц
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
        graphic->setName("Температура_" + QString::number(i+1) + ", ˚C");       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine);
//        graphic->setVisible(false);
        vec_grapics_temper.push_back(graphic);
    }

    {
        QCPGraph *graphic;          // Объявляем график для улицы
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
        graphic->setName("Температура_улица, ˚C");       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine);
        vec_grapics_temper.push_back(graphic);
    }

    // Добавление графиков влажности
    for (int i = 0; i < 8; i++) {
        QCPGraph *graphic;          // Объявляем график
        graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis2);
        graphic->setName("Влажность_" + QString::number(i+1) + ", %");       // Устанавливаем название
        graphic->setLineStyle(QCPGraph::lsLine);
        graphic->setVisible(false);
        vec_grapics_hum.push_back(graphic);
    }

    QPen Pen;
    Pen.setWidthF(2);                //ширина линии
    Pen.setColor(QColor(Qt::blue)); // Устанавливаем цвет графика
    vec_grapics_temper.at(GRAPH_TEPL::Tepl1)->setPen(Pen);
    Pen.setColor(QColor(Qt::yellow));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl2)->setPen(Pen);
    Pen.setColor(QColor(Qt::green));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl3)->setPen(Pen);
    Pen.setColor(QColor(Qt::magenta));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl4)->setPen(Pen);
    Pen.setColor(QColor(Qt::red));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl5)->setPen(Pen);
    Pen.setColor(QColor(Qt::gray));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl6)->setPen(Pen);
    Pen.setColor(QColor(Qt::cyan));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl7)->setPen(Pen);
    Pen.setColor(QColor(Qt::blue));
    vec_grapics_temper.at(GRAPH_TEPL::Tepl8)->setPen(Pen);

    Pen.setWidthF(3);                //ширина линии для улица
    Pen.setColor(QColor(Qt::red)); // Устанавливаем цвет графика для улица
    vec_grapics_temper.at(GRAPH_TEPL::Outdor)->setPen(Pen);

    Pen.setWidthF(2);                //ширина линии
    Pen.setColor(QColor(Qt::darkBlue));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl1)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkYellow));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl2)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkGreen));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl3)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkMagenta));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl4)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkRed));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl5)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkGray));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl6)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkCyan));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl7)->setPen(Pen);
    Pen.setColor(QColor(Qt::darkBlue));
    vec_grapics_hum.at(GRAPH_TEPL::Tepl8)->setPen(Pen);

    vec_checkbox_tepl.push_back(ui->checkBoxTepl1);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_2);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_3);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_4);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_5);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_6);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_7);
    vec_checkbox_tepl.push_back(ui->checkBoxTepl_8);
//    vec_checkbox_tepl.push_back(ui->checkBoxOutDor);

    // первоначальное включение графиков
//    set_visible_graph(ui->checkBoxTepl1->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl1), vec_grapics_hum.at(GRAPH_TEPL::Tepl1));
//    set_visible_graph(ui->checkBoxTepl_2->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl2), vec_grapics_hum.at(GRAPH_TEPL::Tepl2));
//    set_visible_graph(ui->checkBoxTepl_3->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl3), vec_grapics_hum.at(GRAPH_TEPL::Tepl3));
//    set_visible_graph(ui->checkBoxTepl_4->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl4), vec_grapics_hum.at(GRAPH_TEPL::Tepl4));
//    set_visible_graph(ui->checkBoxTepl_5->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl5), vec_grapics_hum.at(GRAPH_TEPL::Tepl5));
//    set_visible_graph(ui->checkBoxTepl_6->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl6), vec_grapics_hum.at(GRAPH_TEPL::Tepl6));
//    set_visible_graph(ui->checkBoxTepl_7->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl7), vec_grapics_hum.at(GRAPH_TEPL::Tepl7));
//    set_visible_graph(ui->checkBoxTepl_8->checkState(), vec_grapics_temper.at(GRAPH_TEPL::Tepl8), vec_grapics_hum.at(GRAPH_TEPL::Tepl8));

//    vec_grapics_temper.at(GRAPH_TEPL::Outdor)->setVisible(ui->checkBoxOutDor->checkState());
//    ui->checkBoxOutDor->checkState()? vec_grapics_temper.at(GRAPH_TEPL::Outdor)->addToLegend(): vec_grapics_temper.at(GRAPH_TEPL::Outdor)->removeFromLegend();

    // Подключаем сигнал от Оси X об изменении видимого диапазона координат к СЛОТу для переустановки формата времени оси.
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(slotRangeChanged(QCPRange)));

    // Подключаем сигналы событий мыши от полотна графика к слотам для их обработки
    connect(customPlot, &QCustomPlot::mouseDoubleClick, this, &Charts::slotMousePress);

    // Инициализируем трассировщик
    tracer = new QCPItemTracer(customPlot);

    dbQSgl = &db;

}

Charts::~Charts()
{
    delete ui;
}


void Charts::slotRangeChanged(const QCPRange &newRange)
{
    /* Если область видимости графика меньше одного дня,
     * то отображаем часы и минуты по Оси X,
     * в противном случае отображаем дату "День Месяц Год"
     * */
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat((newRange.size() <= 86400)? "hh:mm" : "d MMMM\nyyyy");
    customPlot->xAxis->setTicker(dateTicker);
}

void Charts::slotMousePress(QMouseEvent *event)
{
    // Определяем координату X на графике, где был произведён клик мышью
    double coordX = customPlot->xAxis->pixelToCoord(event->pos().x());

    // По координате X клика мыши определим ближайшие координаты для трассировщика
    tracer->setGraphKey(coordX);
    tracer->updatePosition();

    // Выводим координаты точки графика, где установился трассировщик, в lineEdit
    QDateTime timestamp;
    timestamp.setTime_t(tracer->position->key());

    for (int i = 0; i < vec_grapics_temper.size();  i++) {
        tracer->setGraph(customPlot->graph(i));
        tracer->setGraphKey(coordX);
        tracer->updatePosition();
        if (i != 8)
            customPlot->graph(i)->setName("Температура_" + QString::number(i + 1) + ": " + QString::number(tracer->position->value()) + "˚C");
        else
            customPlot->graph(i)->setName("Температура_улица: " + QString::number(tracer->position->value()) + "˚C");
    }

    for (int i = vec_grapics_temper.size(); i < vec_grapics_temper.size() + vec_grapics_hum.size();  i++) {
        tracer->setGraph(customPlot->graph(i));
        tracer->setGraphKey(coordX);
        tracer->updatePosition();
        customPlot->graph(i)->setName("Влажность_" + QString::number(i + 1 - vec_grapics_temper.size()) + ": " + QString::number(tracer->position->value()) + "%");
    }
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl1_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl1), vec_grapics_hum.at(GRAPH_TEPL::Tepl1));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl_2_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl2), vec_grapics_hum.at(GRAPH_TEPL::Tepl2));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl_3_stateChanged(int arg1)
{  
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl3), vec_grapics_hum.at(GRAPH_TEPL::Tepl3));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl_4_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl4), vec_grapics_hum.at(GRAPH_TEPL::Tepl4));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl_5_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl5), vec_grapics_hum.at(GRAPH_TEPL::Tepl5));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl_6_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl6), vec_grapics_hum.at(GRAPH_TEPL::Tepl6));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}

void Charts::on_checkBoxTepl_7_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl7), vec_grapics_hum.at(GRAPH_TEPL::Tepl7));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}
void Charts::on_checkBoxTepl_8_stateChanged(int arg1)
{
    set_visible_graph(arg1, vec_grapics_temper.at(GRAPH_TEPL::Tepl8), vec_grapics_hum.at(GRAPH_TEPL::Tepl8));
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}


void Charts::on_checkBox_temper_stateChanged(int arg1)
{
    if(!arg1)
        for(auto gr : vec_grapics_temper)
        {
            gr->setVisible(false);
        }
    else
        for(auto i = 0; i < vec_checkbox_tepl.size(); i++)
        {
            vec_grapics_temper.at(i)->setVisible(vec_checkbox_tepl.at(i)->isChecked());
        }
    customPlot->replot(); // Перерисовываем содержимое полотна графика

}

void Charts::set_visible_graph(bool st, QCPGraph *graph_t, QCPGraph *graph_h)
{
    bool res = st && ui->checkBox_temper->checkState();
    graph_t->setVisible(res);
    res? graph_t->addToLegend(): graph_t->removeFromLegend();
    res = st && ui->checkBox_hum->checkState();
    graph_h->setVisible(res);
    res? graph_h->addToLegend(): graph_h->removeFromLegend();
}

void Charts::on_pushButton_clicked()
{
    //  if (ui->checkBoxTepl1->checkState() == Qt::CheckState::Unchecked && ui->checkBoxTepl1_2->checkState() == Qt::CheckState::Unchecked)
    //    return;
    QString begin_ = ui->dateTimeEditBeg->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString end_ = ui->dateTimeEditEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query = QSqlQuery(*dbQSgl);

    {
        QString sel = "SELECT DateTime, Теплица_4t, Теплица_5t, Теплица_6t, Теплица_7t, Теплица_4h, Теплица_5h, Теплица_6h, Теплица_7h FROM Tepl4 WHERE DateTime BETWEEN '" + begin_ + "' and '" + end_ + "'";
        query.exec(sel);


        QVector <QVector<double>> data(9);

        while (query.next())
        {
            data[0].push_back(query.value(0).toDateTime().toTime_t());
            for (int i = 1; i < data.size(); i++)
                data[i].push_back(query.value(i).toDouble() / 100.0);
        }

        // Обновляем вектора графиков температур
        vec_grapics_temper.at(GRAPH_TEPL::Tepl4)->setData(data.at(0), data.at(1));
        vec_grapics_temper.at(GRAPH_TEPL::Tepl5)->setData(data.at(0), data.at(2));
        vec_grapics_temper.at(GRAPH_TEPL::Tepl6)->setData(data.at(0), data.at(3));
        vec_grapics_temper.at(GRAPH_TEPL::Tepl7)->setData(data.at(0), data.at(4));


        // Обновляем вектора графиков влажности
        vec_grapics_hum.at(GRAPH_TEPL::Tepl4)->setData(data.at(0), data.at(5));
        vec_grapics_hum.at(GRAPH_TEPL::Tepl5)->setData(data.at(0), data.at(6));
        vec_grapics_hum.at(GRAPH_TEPL::Tepl6)->setData(data.at(0), data.at(7));
        vec_grapics_hum.at(GRAPH_TEPL::Tepl7)->setData(data.at(0), data.at(8));
    }

    {
        QString meteo = "SELECT DateTime, Температура, Влажность, Давление, Ветер_скорость, Ветер_напр, Осадки, Освещенность, УФ FROM Meteo WHERE DateTime BETWEEN '" + begin_ + "' and '" + end_ + "'";
        query.exec(meteo);
        QVector <QVector<double>> data(9);

        while (query.next())
        {
            data[0].push_back(query.value(0).toDateTime().toTime_t());
            for (int i = 1; i < data.size(); i++)
            {
                if (i == 1)
                    data[i].push_back(query.value(i).toDouble() / 10.0);
            }
        }
        vec_grapics_temper.at(GRAPH_TEPL::Outdor)->setData(data.at(0), data.at(1));
    }

    {
        QString sel = "SELECT DateTime, Теплица_1t, Теплица_2t, Теплица_3t, Теплица_1h, Теплица_2h, Теплица_3h FROM Tepl2 WHERE DateTime BETWEEN '" + begin_ + "' and '" + end_ + "'";
        query.exec(sel);

        QVector <QVector<double>> data(7);

        while (query.next())
        {
            data[0].push_back(query.value(0).toDateTime().toTime_t());
            for (int i = 1; i < data.size(); i++)
                data[i].push_back(query.value(i).toDouble() / 100.0);
        }

        // Обновляем вектора графиков температур
        vec_grapics_temper.at(GRAPH_TEPL::Tepl1)->setData(data.at(0), data.at(1));
        vec_grapics_temper.at(GRAPH_TEPL::Tepl2)->setData(data.at(0), data.at(2));
        vec_grapics_temper.at(GRAPH_TEPL::Tepl3)->setData(data.at(0), data.at(3));

        // Обновляем вектора графиков влажности
        vec_grapics_hum.at(GRAPH_TEPL::Tepl1)->setData(data.at(0), data.at(4));
        vec_grapics_hum.at(GRAPH_TEPL::Tepl2)->setData(data.at(0), data.at(5));
        vec_grapics_hum.at(GRAPH_TEPL::Tepl3)->setData(data.at(0), data.at(6));
    }

    {
        QString sel = "SELECT DateTime, Теплица_8t, Теплица_8h FROM Tepl3 WHERE DateTime BETWEEN '" + begin_ + "' and '" + end_ + "'";
        query.exec(sel);

        QVector <QVector<double>> data(3);

        while (query.next())
        {
            data[0].push_back(query.value(0).toDateTime().toTime_t());
            for (int i = 1; i < data.size(); i++)
                data[i].push_back(query.value(i).toDouble() / 100.0);
        }

        // Обновляем вектора графиков температур
        vec_grapics_temper.at(GRAPH_TEPL::Tepl8)->setData(data.at(0), data.at(1));

        // Обновляем вектора графиков влажности
        vec_grapics_hum.at(GRAPH_TEPL::Tepl8)->setData(data.at(0), data.at(2));
    }

    on_pushButtonAutoScale_clicked();
}

void Charts::on_checkBoxOutDor_stateChanged(int arg1)
{
    bool res = arg1 && ui->checkBox_temper->checkState();
    vec_grapics_temper.at(GRAPH_TEPL::Outdor)->setVisible(res);
    res? vec_grapics_temper.at(GRAPH_TEPL::Outdor)->addToLegend(): vec_grapics_temper.at(GRAPH_TEPL::Outdor)->removeFromLegend();
    customPlot->replot(); // Перерисовываем содержимое полотна графика
}


void Charts::on_checkBox_hum_stateChanged(int arg1)
{
    if(!arg1)
        for(auto gr : vec_grapics_hum)
        {
            gr->setVisible(false);
        }
    else
        for(auto i = 0; i < vec_checkbox_tepl.size(); i++)
        {
            vec_grapics_hum.at(i)->setVisible(vec_checkbox_tepl.at(i)->isChecked());
        }
    customPlot->replot();           // Отрисовываем график
}


void Charts::on_pushButtonAutoScale_clicked()
{
    customPlot->rescaleAxes();      // Масштабируем график по данным
    customPlot->yAxis->setRange(customPlot->yAxis->range().lower > 0 ? 0.0 : customPlot->yAxis->range().lower - 0.2, customPlot->yAxis->range().upper * 2);
    customPlot->yAxis2->setRange(0, customPlot->yAxis2->range().upper * 1.02);
    customPlot->replot();
}

// Включаем перетаскивание по X
void Charts::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
        customPlot->axisRect()->setRangeDrag(Qt::Horizontal) ;
    }
}

// Включаем перетаскивание по Y
void Charts::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control){
        customPlot->axisRect()->setRangeZoom(Qt::Vertical);
        customPlot->axisRect()->setRangeDrag(Qt::Vertical);
    }
}
