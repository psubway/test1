#include "led_control.h"
#include "ui_led_control.h"
#include "mainwindow.h"

extern MainWindow *Main;

LED_CONTROL::LED_CONTROL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LED_CONTROL)
{
    ui->setupUi(this);
}

LED_CONTROL::~LED_CONTROL()
{
    delete ui;
}

void LED_CONTROL::on_horizontalSlider_valueChanged(int value)
{
    Main->on_LED_2_VALUE_valueChanged(value);
    ui->spinBox->setValue(value);
}

void LED_CONTROL::on_spinBox_valueChanged(int arg1)
{
    Main->on_LED_2_VALUE_valueChanged(arg1);
    ui->horizontalSlider->setValue(arg1);
}

void LED_CONTROL::on_horizontalSlider_2_valueChanged(int value)
{
    Main->on_LED_1_VALUE_valueChanged(value);
    ui->spinBox_2->setValue(value);
}

void LED_CONTROL::on_spinBox_2_valueChanged(int arg1)
{
    Main->on_LED_1_VALUE_valueChanged(arg1);
    ui->horizontalSlider_2->setValue(arg1);
}

void LED_CONTROL::on_TOP_LED_ON_clicked()
{
    Main->on_LED2_ON_clicked();
      ui->horizontalSlider->setValue(255);
      ui->spinBox->setValue(255);
}

void LED_CONTROL::on_TOP_LED_OFF_clicked()
{
    Main->on_LED2_OFF_clicked();
    ui->horizontalSlider->setValue(0);
    ui->spinBox->setValue(0);
}

void LED_CONTROL::on_BOTTOM_LED_ON_clicked()
{
    Main->on_LED1_ON_clicked();
    ui->horizontalSlider_2->setValue(255);
    ui->spinBox_2->setValue(255);
}

void LED_CONTROL::on_BOTTOM_LED_OFF_clicked()
{
    Main->on_LED1_OFF_clicked();
    ui->horizontalSlider_2->setValue(0);
    ui->spinBox_2->setValue(0);
}
