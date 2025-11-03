#include "QDebug"

#include "settingWindow.h"
#include "ui_settingWindow.h"

SettingWindow::SettingWindow(QWidget *parent, qint16 level) :
  QDialog(parent),
  ui(new Ui::SettingWindow)
{
  ui->setupUi(this);
  ui->progressBar->setValue(level);
  ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: cornflowerblue;}");
  ui->Setting_window->setValue(level);
}

qint16 SettingWindow::getOutLevel()
{
   return ui->progressBar->value();
}

SettingWindow::~SettingWindow()
{
  delete ui;
}

void SettingWindow::on_Setting_window_valueChanged(int value)
{
   ui->progressBar->setValue(value);
}

void SettingWindow::on_buttonBox_accepted()
{
  QDialog::accept();
}

