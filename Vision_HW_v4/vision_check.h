#ifndef VISION_CHECK_H
#define VISION_CHECK_H

#include <QDialog>
#include "mainwindow.h"
#include <QKeyEvent>
#include <QTimer>
#include "play.h"
#include <QtSql>
#include <QDate>
#include <QFileSystemWatcher>
#include <QStandardItemModel>
namespace Ui {
class Vision_check;
}

class Vision_check : public QDialog
{
    Q_OBJECT

public:
    explicit Vision_check(QWidget *parent = nullptr);
    ~Vision_check();

private:
    Ui::Vision_check *ui;
    QTimer *Vision_timer;

    QImage TOP_CHECK_VIEW;
    int VISION_STEP;

    float POINT1_Y_VALUE, POINT2_Y_VALUE;

    int mvr1_cnt;
    int mvr2_cnt;

    uint8_t stage_cnt;
    QString tilt_dir;
    QString date_string;
private slots :
    void View_Update();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    float V_MOVING_AVR2(float input);
    float V_MOVING_AVR1(float input);


    void on_stage_down_clicked();

    void on_stage_up_clicked();

    void tilt_save(QString LOT_NUM, QString MODE, QString DATE, QString STAGE, QString VALUE);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // VISION_CHECK_H
