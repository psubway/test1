#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <QtSerialPort>
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <QtMath>
#include <algorithm>
#include <QIcon>
#include <QScrollBar>
#include "led_control.h"
#include "play.h"
#include <QDate>
#include <QDesktopServices>
#include "save_thread.h"
#include "stage_change_thread.h"
#include "setting_form.h"
#include "vision_check.h"
#include "clientStuff.h"

namespace Ui {
class MainWindow;
}
using namespace cv;
using namespace std;
class PLAY;
class Setting_Form;
class MainWindow : public QMainWindow
{
#define ON 1
#define OFF 2
#define STOP 3
#define LED_CON_CHECK 0x02
#define LED_VALUE_SET 0x03

#define DEVICE_TYPE_DIRECTION 1
#define DEVICE_TYPE_POWER 2
#define DEVICE_TYPE_NONE 0

#define STAGE_WRITE 0x01
#define STAGE_READ 0x02

#define STAGE_POINT1_MOVE 0x09
#define STAGE_POINT2_MOVE 0x0a
#define STAGE_POINT3_MOVE 0x0b
#define STAGE_WAIT_MOVE 0x0c
#define X1_POS 0x0d
#define X2_POS 0x0e
#define Y_POS 0x0f
#define Z_POS 0x10
#define VISION_ORIGIN 0x11



#define GOAL_STAGE_POINT1 0x12
#define GOAL_STAGE_POINT2 0x13
#define GOAL_STAGE_POINT3 0x14
#define GOAL_STAGE_POINT_WAIT 0x15

#define VISION_CHECK_POINT1 0x20
#define VISION_CHECK_POINT1_MOVE 0x21
#define VISION_CHECK_POINT2 0x30
#define VISION_CHECK_POINT2_MOVE 0x31
#define VISION_ENABLE 0x16
#define POINT_ERROR 0x17

#define BUTTON_STATE 0x26
#define BUTTON_ON 0x27
#define BUTTON_OFF 0x28

#define STAGE_NUM 0x01
#define DOOR_EMG 0x02
#define SAFETY_EMG 0x03
#define STAGE_TURN 0x04
#define TOP_VISION_OK 0x05
#define BOTTOM_VISION_OK 0x06
#define BOTTOM_VISION_NG 0x07
#define PRESS_END 0x08
#define END_BT_CLICK 0x09
#define BOTTOM_END 0x0a
#define BOTTOM_START 0x0b
#define RAMI_STATUS 0x0c
#define PUMP_ONOFF 0x20
#define TOWER_LED 0x21

#define TOWER_AUTO 0x01
#define TOWER_WAIT 0x02
#define TOWER_ERROR 0x03

#define MARGIN_X 8
#define MARGIN_Y 8

#define BOTTOM_OK 0x10
#define BOTTOM_NG 0x20
#define BOTTOM_IDLE 0x30

#define INTER_LOCK 0x33

#define SHIFT_OK 0x01
#define OVER_POS 0x02
#define ECCENTRIC_POS 0x04

#define bottom_image_size_x 640
#define bottom_image_size_y 720

#define WIN_UPDATE_TIME 30 // 0.033ms
#define LAMI_UPDATE_TIME 30 // 0.033ms
#define DISTING_GAP 250
#define DISTING_RS4 100
#define DISTING_RS4_r 100

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void on_CAM_OPEN_clicked();
    void on_CAM_CLOSE_clicked();
    void update_window();
    void result_process();

    void on_LED2_OPEN_clicked();

    void on_LED1_CLOSE_clicked();

    void on_LED2_CLOSE_clicked();

    void on_LED_1_VALUE_valueChanged(int arg1);

    void on_LED_2_VALUE_valueChanged(int arg1);

    void on_LED1_ON_clicked();

    void on_LED1_OFF_clicked();

    void on_LED2_ON_clicked();

    void on_LED2_OFF_clicked();

     void on_LED3_ON_clicked();

    void DataParsing_LED1(uint8_t temp[]);

    void RX_DATA_LED1();
    void RX_DATA_LED2();
    void RX_DATA_RAMI();
    void RX_DATA_VISION_MOTOR();
    void DataParsing_LED2(uint8_t temp[]);
    void DataParsing_VISION_MOTOR(uint8_t temp[]);

    void time_loop(long int time);
    void on_pushButton_clicked();

    void on_VIEW_CHANGE_BOTTOM_clicked();
    void BOTTOM_STAGE_VISION_PRO();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_LED1_OPEN_clicked();
    bool ContourAreaSortPredicate(const std::vector<cv::Point> c1, const std::vector<cv::Point> c2);

    void on_comboBox_4_currentIndexChanged(const QString &arg1);

    void on_VQLAMI_OPEN_clicked();

    void on_VQLAMI_CLOSE_clicked();
    void DataParsing_LAMI(uint8_t temp[]);
    void on_comboBox_5_currentIndexChanged(const QString &arg1);

    void on_VISION_OPEN_clicked();

    void on_VISION_CLOSE_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();


    void on_pushButton_8_clicked();

    void on_pushButton_12_clicked();
	void on_pushButton_13_clicked();
    void update_bt_sig();
    void TOP_STAGE();

    void RAMI_SEND(uint8_t return_sel);
    void TOWER_SEND();
    void STATUS_SEND();


    void VIEW_SETTING(bool SEL);

    void OPEN_CAM_TOP();
    void OPEN_CAM_BOTTOM();
    Mat RotateImage(Mat img, double angle, double x, double y);
    void STAGE_CHECK();
    void SAVE_IMAGE();
    void tcp_send();
    void on_LED3_OFF_clicked();
    void RX_DATA_LED3();
    void DataParsing_LED3(uint8_t temp[]);

    void tcp_send_start(QString lot);
    void tcp_send_end(QString lot);
public:
    Ui::MainWindow *ui;
    QSerialPort* port = new QSerialPort();
    QByteArray Read_temp;

    QSerialPort* port2 = new QSerialPort();
    QByteArray Read_temp2;

    QSerialPort* RAMI_EQ = new QSerialPort();
    QByteArray Read_temp3;

    QSerialPort* VISION_MOTOR = new QSerialPort();
    QByteArray Read_temp4;

    QSerialPort* port3 = new QSerialPort();
    QByteArray Read_temp5;



    QTimer *timer;
    QTimer *r_timer;
    QTimer *result_timer;
    QTimer *test_timer;
    QTimer *RAMI_TIMER;
    QTimer *POINT1_TM;
    QTimer *POINT2_TM;
    QTimer *TOP_CHECK_TM;
    QTimer *OPEN_DELAY;
    QTimer *RECONNECTION;
    QTimer *key_input_timeout_main;
    VideoCapture cap;
    VideoCapture cap_r;
    VideoCapture cap_T;

    Mat cap_temp;
    Mat cap_T_temp;

    QImage qt_image,qt_image_T, qt_image_r;
    QImage Origin_Image, Origin_Image_T,Origin_Image_r;

    Mat frame_T;

    PLAY *play;

    Setting_Form *sf;
    Mat frame, Binary_img;
    Mat frame_temp;
    Mat OTSU;
    Mat Gauss;
    Mat Contour;
    Mat Canny_img;
    Mat Last_img;
    int Reference_NonZero;
    Mat Reference_img;
    Mat Reference_gray_img;


    Mat frame_r, Binary_img_r,frame_r_r;
    Mat frame_temp_r;
    Mat OTSU_r;
    Mat Gauss_r;
    Mat Contour_r;
    Mat Canny_img_r;
    Mat Last_img_r;
    int Reference_NonZero_r;
    Mat Reference_img_r;
    Mat Reference_gray_img_r;

    int BOTTOM_SIG;
    int BOTTOM_SIG_r;
    int BOTTOM_ST_SIG;
    int GGAM_CNT;
    QString OK;
    QString ERROR;

    float ERROR_LEN;

    int TOP_VIEW_IMAGE_CH;

    int STAGE_NUM_TEMP;

    QString TOP_STAGE_MSG;


    bool BOTTOM_SEND_EN;
    bool VIEW_MODE_CHANGE;
    bool VIEW_MODE_CHANGE_TEMP;
    int BOTTOM_STAGE_FLAG;
    int BOTTOM_STAGE_FLAG_r;

    int POINT_STATE;
    double P_POS_X, P_POS_Y;

    QString ALL_DATA;

    int LOT_COUNT;

    bool SKIP_EN;
    int real_lot_cnt;


    uint16_t TOP_POINT1_Y, TOP_POINT2_Y;


    bool Goal_Point1_check,Goal_Point2_check;

    double dd;
    int top_check_cnt;
    float POINT1_X_TEMP,POINT1_Y_TEMP,POINT2_X_TEMP,POINT2_Y_TEMP,POINT3_X_TEMP,POINT3_Y_TEMP,POINT4_X_TEMP,POINT4_Y_TEMP;

    float POINT1_X_TEMP_r,POINT1_Y_TEMP_r,POINT2_X_TEMP_r,POINT2_Y_TEMP_r,POINT3_X_TEMP_r,POINT3_Y_TEMP_r,POINT4_X_TEMP_r,POINT4_Y_TEMP_r;

    QString PUMP_STATUS_Client;
    QString LED_TOWER_Client;
    QString BOTTOM_ERR_Client;
    QString BOTTOM_X_TEMP;

    QString PUMP_STATUS_Client_r;
    QString LED_TOWER_Client_r;
    QString BOTTOM_ERR_Client_r;
    QString BOTTOM_X_TEMP_r;

    QString PRESS_STATUS_Client;
    bool interlock_flag;
    bool tcp_send_en;
    bool first_re_data;
    uint8_t TOP_CHECK_CNT;
    QString Key_input_main;
    bool bottom_r_result;
    bool tilt_check;
    int bottom_l_r_result;

	int typeOfdevice;
	int typeOfdevice_r;
	
signals:
    void LOT_END(void);
private :

    //QScrollBar *sb;
    int aa,bb,cc;
        ClientStuff *client;

        float P_X1 ;
        float varP_X1 ;
        float varM_X1 ;
        float K_X1;
        float Kalman_X1 ;
		
        float P_X2 ;
        float varP_X2 ;
        float varM_X2 ;
        float K_X2;
        float Kalman_X2 ;
		
        float P_X3 ;
        float varP_X3 ;
        float varM_X3 ;
        float K_X3;
        float Kalman_X3 ;

		float P_X4 ;
        float varP_X4 ;
        float varM_X4 ;
        float K_X4;
        float Kalman_X4 ;


        float P_Y1 ;
        float varP_Y1 ;
        float varM_Y1 ;
        float K_Y1;
        float Kalman_Y1 ;
		
        float P_Y2 ;
        float varP_Y2 ;
        float varM_Y2 ;
        float K_Y2;
        float Kalman_Y2 ;
		
        float P_Y3 ;
        float varP_Y3 ;
        float varM_Y3 ;
        float K_Y3;
        float Kalman_Y3 ;

		float P_Y4 ;
        float varP_Y4 ;
        float varM_Y4 ;
        float K_Y4;
        float Kalman_Y4 ;

        float P_X1_r;
        float varP_X1_r;
        float varM_X1_r;
        float K_X1_r;
        float Kalman_X1_r;
		
        float P_X2_r;
        float varP_X2_r;
        float varM_X2_r;
        float K_X2_r;
        float Kalman_X2_r;
		
        float P_X3_r;
        float varP_X3_r;
        float varM_X3_r;
        float K_X3_r;
        float Kalman_X3_r;

		float P_X4_r;
        float varP_X4_r;
        float varM_X4_r;
        float K_X4_r;
        float Kalman_X4_r;


        float P_Y1_r;
        float varP_Y1_r;
        float varM_Y1_r;
        float K_Y1_r;
        float Kalman_Y1_r;
		
        float P_Y2_r;
        float varP_Y2_r;
        float varM_Y2_r;
        float K_Y2_r;
        float Kalman_Y2_r;
		
        float P_Y3_r;
        float varP_Y3_r;
        float varM_Y3_r;
        float K_Y3_r;
        float Kalman_Y3_r;

		float P_Y4_r;
        float varP_Y4_r;
        float varM_Y4_r;
        float K_Y4_r;
        float Kalman_Y4_r;


        double stage_angle_l_stage1;
        double stage_angle_l_stage2;
        double stage_angle_l_stage3;
        double stage_angle_l_stage4;

        double stage_angle_r_stage1;
        double stage_angle_r_stage2;
        double stage_angle_r_stage3;
        double stage_angle_r_stage4;

private slots:
    void on_LIGHT_BT_clicked();
    void on_SNAP_BT_clicked();
    void on_pushButton_9_clicked();
    void on_PLAY_BT_clicked();
    void on_pushButton_10_clicked();
    void LOT_NUM_UPDATE();
    void on_SAVE_BT_clicked();
    void on_pushButton_11_clicked();
    void on_SET_BT_clicked();
    //void on_spinBox_valueChanged(int arg1);
    void POINT1_CHECK();
    void POINT2_CHECK();
    void TOP_CHECK();
    //void on_LOTCOUNT_M_clicked();
    //void on_LOTCOUNT_P_clicked();
    void on_checkBox_2_clicked();
    void OPEN_DELAY_FUNC();
    void on_pushButton_connect_clicked();
    void update_window_r();
    void on_pushButton_disconnect_clicked();

    void on_pushButton_send_clicked();
    void keyin_timeout_main();
    void BOTTOM_STAGE_VISION_PRO_R();
    void server_reconnection();
    void on_LED3_OPEN_clicked();

    void on_LED3_CLOSE_clicked();

    void on_LED_3_VALUE_valueChanged(int arg1);

    void on_comboBox_6_currentIndexChanged(const QString &arg1);


    void on_angle_tune_l_stage1_valueChanged(double arg1);

    void on_angle_tune_l_stage2_valueChanged(double arg1);

    void on_angle_tune_l_stage3_valueChanged(double arg1);

    void on_angle_tune_l_stage4_valueChanged(double arg1);

    void on_angle_tune_r_1_valueChanged(double arg1);

    void on_angle_tune_r_2_valueChanged(double arg1);

    void on_angle_tune_r_3_valueChanged(double arg1);

    void on_angle_tune_r_4_valueChanged(double arg1);

public slots:
    void setStatus(bool newStatus);
    void receivedSomething(QString msg);
    void gotError(QAbstractSocket::SocketError err);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
