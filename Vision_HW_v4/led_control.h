#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <QDialog>

namespace Ui {
class LED_CONTROL;
}

class LED_CONTROL : public QDialog
{
    Q_OBJECT

public:
    explicit LED_CONTROL(QWidget *parent = nullptr);
    ~LED_CONTROL();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_spinBox_2_valueChanged(int arg1);

    void on_TOP_LED_ON_clicked();

    void on_TOP_LED_OFF_clicked();

    void on_BOTTOM_LED_ON_clicked();

    void on_BOTTOM_LED_OFF_clicked();

private:
    Ui::LED_CONTROL *ui;
};

#endif // LED_CONTROL_H
