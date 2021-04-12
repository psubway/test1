# this is a sample code.

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *Main;

struct AreaCmp {
    AreaCmp(const vector<float>& _areas) : areas(&_areas) {}
    bool operator()(int a, int b) const { return (*areas)[a] > (*areas)[b]; }
    const vector<float>* areas;
};

extern bool ALL_ENABLE;
extern int LOT_COUNT_MAX;
extern SAVE_Thread *save_t;
extern STAGE_CHANGE_Thread *vc_t;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Main=this;

    VIEW_MODE_CHANGE = false;

    /* camera image data processing part */
    timer = new QTimer(this);
    r_timer = new QTimer(this);

    result_timer = new QTimer(this);

	/* vision check part */
    test_timer= new QTimer(this);
    RAMI_TIMER = new QTimer(this);
    POINT1_TM = new QTimer(this);
    POINT2_TM = new QTimer(this);
    TOP_CHECK_TM = new QTimer(this);
    OPEN_DELAY = new QTimer(this);
    RECONNECTION = new QTimer(this);
    key_input_timeout_main = new QTimer(this);

    /* camera image timer connection */    
    connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    connect(r_timer, SIGNAL(timeout()), this, SLOT(update_window_r()));
    connect(result_timer, SIGNAL(timeout()), this, SLOT(result_process()));
    
	/* vision check timer connection */
    connect(key_input_timeout_main, SIGNAL(timeout()),SLOT(keyin_timeout_main()));
    connect(RECONNECTION, SIGNAL(timeout()),this,SLOT(server_reconnection()));
    //connect(test_timer, SIGNAL(timeout()), this, SLOT(update_bt_sig()));

    connect(POINT1_TM,SIGNAL(timeout()),this,SLOT(POINT1_CHECK()));
    connect(OPEN_DELAY,SIGNAL(timeout()),this,SLOT(OPEN_DELAY_FUNC()));
    connect(POINT2_TM,SIGNAL(timeout()),this,SLOT(POINT2_CHECK()));
    connect(TOP_CHECK_TM,SIGNAL(timeout()),this,SLOT(TOP_CHECK()));

    /* define local variables */
    TOP_CHECK_CNT=4;

    TOP_VIEW_IMAGE_CH=OFF;

    for (QSerialPortInfo portn : QSerialPortInfo::availablePorts())
    {
        qDebug() << portn.portName();
        ui->comboBox->addItem(portn.portName());
    }

    port->setPortName("COM51");
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    connect(port,SIGNAL(readyRead()), this, SLOT(RX_DATA_LED1()));

    for (QSerialPortInfo portn : QSerialPortInfo::availablePorts())
    {
        qDebug() << portn.portName();
        ui->comboBox_2->addItem(portn.portName());
    }

    port2->setPortName("COM52");
    port2->setBaudRate(QSerialPort::Baud115200);
    port2->setDataBits(QSerialPort::Data8);
    port2->setParity(QSerialPort::NoParity);
    port2->setStopBits(QSerialPort::OneStop);
    port2->setFlowControl(QSerialPort::NoFlowControl);
    connect(port2,SIGNAL(readyRead()), this, SLOT(RX_DATA_LED2()));

    for (QSerialPortInfo portn : QSerialPortInfo::availablePorts())
    {
        qDebug() << portn.portName();
        ui->comboBox_4->addItem(portn.portName());
    }

    port3->setPortName("COM53");
    port3->setBaudRate(QSerialPort::Baud115200);
    port3->setDataBits(QSerialPort::Data8);
    port3->setParity(QSerialPort::NoParity);
    port3->setStopBits(QSerialPort::OneStop);
    port3->setFlowControl(QSerialPort::NoFlowControl);
    connect(port3,SIGNAL(readyRead()), this, SLOT(RX_DATA_LED3()));

    for (QSerialPortInfo portn : QSerialPortInfo::availablePorts())
    {
        qDebug() << portn.portName();
        ui->comboBox_6->addItem(portn.portName());
    }

    RAMI_EQ->setPortName("COM54");
    RAMI_EQ->setBaudRate(QSerialPort::Baud115200);
    RAMI_EQ->setDataBits(QSerialPort::Data8);
    RAMI_EQ->setParity(QSerialPort::NoParity);
    RAMI_EQ->setStopBits(QSerialPort::OneStop);
    RAMI_EQ->setFlowControl(QSerialPort::NoFlowControl);
    connect(RAMI_EQ,SIGNAL(readyRead()), this, SLOT(RX_DATA_RAMI()));


    for (QSerialPortInfo portn : QSerialPortInfo::availablePorts())
    {
        qDebug() << portn.portName();
        ui->comboBox_5->addItem(portn.portName());
    }

    VISION_MOTOR->setPortName("COM55");
    VISION_MOTOR->setBaudRate(QSerialPort::Baud115200);
    VISION_MOTOR->setDataBits(QSerialPort::Data8);
    VISION_MOTOR->setParity(QSerialPort::NoParity);
    VISION_MOTOR->setStopBits(QSerialPort::OneStop);
    VISION_MOTOR->setFlowControl(QSerialPort::NoFlowControl);
    connect(VISION_MOTOR,SIGNAL(readyRead()), this, SLOT(RX_DATA_VISION_MOTOR()));

    on_LED3_OPEN_clicked();
    on_LED2_OPEN_clicked();
    on_LED1_OPEN_clicked();
    on_VQLAMI_OPEN_clicked();
    on_VISION_OPEN_clicked();

    QIcon icon1,icon2,icon3,icon4,icon5;
    icon1.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\PLAY_BT.png")), QIcon::Normal, QIcon::On);
    //icon.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\PLAY_BT_A.png")), QIcon::Active, QIcon::On);
    ui->PLAY_BT->setIcon(icon1);
    ui->PLAY_BT->setIconSize(QSize(161, 100));

    icon2.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\LIGHT_BT.png")), QIcon::Normal, QIcon::On);
    //icon.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\PLAY_BT_A.png")), QIcon::Active, QIcon::On);
    ui->LIGHT_BT->setIcon(icon2);
    ui->LIGHT_BT->setIconSize(QSize(161, 100));

    icon3.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\SNAP_SHOT_BT.png")), QIcon::Normal, QIcon::On);
    //icon.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\PLAY_BT_A.png")), QIcon::Active, QIcon::On);
    ui->SNAP_BT->setIcon(icon3);
    ui->SNAP_BT->setIconSize(QSize(161, 100));

    icon4.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\SET_BT.png")), QIcon::Normal, QIcon::On);
    //icon.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\PLAY_BT_A.png")), QIcon::Active, QIcon::On);
    ui->SET_BT->setIcon(icon4);
    ui->SET_BT->setIconSize(QSize(161, 100));

    icon5.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\SAVE_BT.jpg")), QIcon::Normal, QIcon::On);
    //icon.addPixmap(QPixmap(QString::fromUtf8("C:\\Users\\Synopex ESD\\Pictures\\PLAY_BT_A.png")), QIcon::Active, QIcon::On);
    ui->SAVE_BT->setIcon(icon5);
    ui->SAVE_BT->setIconSize(QSize(161, 100));

    QDate *date = new QDate();
    QDate curDate = date->currentDate();   // 시스템에서 현재 날짜 가져오기
    QString date_string = curDate.toString(Qt::ISODate);

    QString logPath = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\ORIGINAL\\TOP";
    QDir dir(logPath);
    dir.mkpath(logPath);

    QString logPath3 = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\ORIGINAL\\BOTTOM";
    QDir dir3(logPath3);
    dir3.mkpath(logPath3);


    QString logPath2 = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\VISION\\TOP";
    QDir dir2(logPath2);
    dir.mkpath(logPath2);

    QString logPath4 = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\VISION\\BOTTOM";
    QDir dir4(logPath4);
    dir4.mkpath(logPath4);

    QPixmap LOGO("C:\\Users\\Synopex ESD\\Pictures\\LOGO.jpg");
    ui->LOGO_LABEL->setPixmap(LOGO);

    BOTTOM_STAGE_FLAG=STOP;
    BOTTOM_SEND_EN=false;

    BOTTOM_STAGE_FLAG_r=STOP;
    BOTTOM_SEND_EN=false;

    TOP_VIEW_IMAGE_CH=true;

    play = new PLAY(this);
    connect(play,SIGNAL(CHECK_END()),SLOT(LOT_NUM_UPDATE()));


    LOT_COUNT=0;

    ui->PAGE_CHANGE->setCurrentWidget(ui->BOTTOM_STATE);


    ui->TOP_VISION->setVisible(false);
    ui->BOTTOM_VIDEO->setVisible(false);
    ui->BOTTOM_VIDEO_r->setVisible(false);

    Goal_Point1_check=false;
    Goal_Point2_check=false;

    ui->STAGE_TEXT->setVisible(false);
    POINT_STATE=0;

    POINT1_X_TEMP=0;
    POINT2_X_TEMP=0;
    POINT3_X_TEMP=0;
    POINT4_X_TEMP=0;

    POINT1_Y_TEMP=0;
    POINT2_Y_TEMP=0;
    POINT3_Y_TEMP=0;
    POINT4_Y_TEMP=0;

    POINT1_X_TEMP_r=0;
    POINT2_X_TEMP_r=0;
    POINT3_X_TEMP_r=0;
    POINT4_X_TEMP_r=0;

    POINT1_Y_TEMP_r=0;
    POINT2_Y_TEMP_r=0;
    POINT3_Y_TEMP_r=0;
    POINT4_Y_TEMP_r=0;

    P_X1 = 1.0;
    varP_X1 = pow(0.1, 2);
    varM_X1 = pow(0.5, 2);
    K_X1 = 1.0;
    Kalman_X1 = 500.0;

    P_X2 = 1.0;
    varP_X2 = pow(0.1, 2);
    varM_X2 = pow(0.5, 2);
    K_X2 = 1.0;
    Kalman_X2 = 800.0;

    P_X3 = 1.0;
    varP_X3 = pow(0.1, 2);
    varM_X3 = pow(0.5, 2);
    K_X3 = 1.0;
    Kalman_X3 = 800.0;

    P_X4 = 1.0;
    varP_X4 = pow(0.1, 2);
    varM_X4 = pow(0.5, 2);
    K_X4 = 1.0;
    Kalman_X4 = 800.0;

    P_Y1 = 1.0;
    varP_Y1 = pow(0.1, 2);
    varM_Y1 = pow(0.5, 2);
    K_Y1 = 1.0;
    Kalman_Y1 = 500.0;

    P_Y2 = 1.0;
    varP_Y2 = pow(0.1, 2);
    varM_Y2 = pow(0.5, 2);
    K_Y2 = 1.0;
    Kalman_Y2 = 800.0;

    P_Y3 = 1.0;
    varP_Y3 = pow(0.1, 2);
    varM_Y3 = pow(0.5, 2);
    K_Y3 = 1.0;
    Kalman_Y3 = 800.0;

    P_Y4 = 1.0;
    varP_Y4 = pow(0.1, 2);
    varM_Y4 = pow(0.5, 2);
    K_Y4 = 1.0;
    Kalman_Y4 = 800.0;

    P_X1_r = 1.0;
    varP_X1_r = pow(0.1, 2);
    varM_X1_r= pow(0.5, 2);
    K_X1_r= 1.0;
    Kalman_X1_r= 500.0;

    P_X2_r= 1.0;
    varP_X2_r= pow(0.1, 2);
    varM_X2_r= pow(0.5, 2);
    K_X2_r= 1.0;
    Kalman_X2_r= 800.0;

    P_X3_r= 1.0;
    varP_X3_r= pow(0.1, 2);
    varM_X3_r= pow(0.5, 2);
    K_X3_r= 1.0;
    Kalman_X3_r= 800.0;

    P_X4_r= 1.0;
    varP_X4_r= pow(0.1, 2);
    varM_X4_r= pow(0.5, 2);
    K_X4_r= 1.0;
    Kalman_X4_r= 800.0;

    P_Y1_r= 1.0;
    varP_Y1_r= pow(0.1, 2);
    varM_Y1_r= pow(0.5, 2);
    K_Y1_r= 1.0;
    Kalman_Y1_r= 500.0;

    P_Y2_r= 1.0;
    varP_Y2_r= pow(0.1, 2);
    varM_Y2_r= pow(0.5, 2);
    K_Y2_r= 1.0;
    Kalman_Y2_r= 800.0;

    P_Y3_r= 1.0;
    varP_Y3_r= pow(0.1, 2);
    varM_Y3_r= pow(0.5, 2);
    K_Y3_r= 1.0;
    Kalman_Y3_r= 800.0;

    P_Y4_r= 1.0;
    varP_Y4_r= pow(0.1, 2);
    varM_Y4_r= pow(0.5, 2);
    K_Y4_r= 1.0;
    Kalman_Y4_r= 800.0;


    ui->pushButton_disconnect->setVisible(false);

    client = new ClientStuff("10.0.6.217", 6974);

    setStatus(client->getStatus());

    connect(client, &ClientStuff::hasReadSome, this, &MainWindow::receivedSomething);
    connect(client, &ClientStuff::statusChanged, this, &MainWindow::setStatus);
    // FIXME change this connection to the new syntax
    connect(client->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(gotError(QAbstractSocket::SocketError)));

    bottom_l_r_result=0;
    tilt_check=false;
    tcp_send_en=false;
    first_re_data=false;
    bottom_r_result=false;
    on_PLAY_BT_clicked();

    stage_angle_l_stage1=-0.2;
    stage_angle_l_stage2=-0.2;
    stage_angle_l_stage3=-0.79;
    stage_angle_l_stage4=-0.15;

    stage_angle_r_stage1=0.65;
    stage_angle_r_stage2=0.96;
    stage_angle_r_stage3=0.67;
    stage_angle_r_stage4=0.60;



    ui->angle_tune_l_stage1->setValue(stage_angle_l_stage1);
    ui->angle_tune_l_stage2->setValue(stage_angle_l_stage2);
    ui->angle_tune_l_stage3->setValue(stage_angle_l_stage3);
    ui->angle_tune_l_stage4->setValue(stage_angle_l_stage4);

    ui->angle_tune_r_1->setValue(stage_angle_r_stage1);
    ui->angle_tune_r_2->setValue(stage_angle_r_stage2);
    ui->angle_tune_r_3->setValue(stage_angle_r_stage3);
    ui->angle_tune_r_4->setValue(stage_angle_r_stage4);

#if 1
    ui->angle_tune_r_1->setVisible(false);
    ui->angle_tune_r_2->setVisible(false);
    ui->angle_tune_r_3->setVisible(false);
    ui->angle_tune_r_4->setVisible(false);

    ui->angle_tune_l_stage1->setVisible(false);
    ui->angle_tune_l_stage2->setVisible(false);
    ui->angle_tune_l_stage3->setVisible(false);
    ui->angle_tune_l_stage4->setVisible(false);

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
#endif
    client->connect2host();

}

MainWindow::~MainWindow()
{

    LED_TOWER_Client="TOWER_OFF";
    tcp_send();
    time_loop(500);
    qDebug()<<"end tcp send";
    delete ui;
    delete client;
}

void MainWindow::on_pushButton_12_clicked()
{
    MainWindow::close();
}
void MainWindow::on_CAM_OPEN_clicked()
{
    cap.open("http://192.168.9.4:9996/?action=stream");
    cap_T.open("http://192.168.9.5:9991/?action=stream");
    cap_r.open("http://192.168.9.7:9997/?action=stream");
    if(!cap.isOpened() || !cap_T.isOpened() || !cap_r.isOpened())
        // || !cap_T.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    }
    else
    {
        cout << "camera is open" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        if(timer->isActive()==false)
        {
            timer->start(WIN_UPDATE_TIME);
            r_timer->start(WIN_UPDATE_TIME);
        }
    }

}

void MainWindow::OPEN_CAM_BOTTOM()
{
    //cap_T.release();
    //cap.open("http://192.168.9.4:9996/?action=stream");
    cap.open("http://169.254.26.178:9996/?action=stream");
    cap_r.open("http://169.254.26.178:9996/?action=stream");

    if(!cap.isOpened())// || !cap_r.isOpened())
        // || !cap_T.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open2" << endl;
    }
    else
    {
        cout << "camera is open bottom2" << endl;
 #if 1
        //r_thread->start();
        //l_thread->start();
        if(timer->isActive()==false)
        {
            timer->start(WIN_UPDATE_TIME);
        }

        if(r_timer->isActive()==false)
        {
            //r_timer->start(WIN_UPDATE_TIME);
        }


#endif
        if(result_timer->isActive()==false)
        {
            result_timer->start(2000);
        }

    }

}
void MainWindow::OPEN_CAM_TOP()
{
    cap_T.open("http://192.168.9.5:9991/?action=stream");
    cap.release();
    cap_r.release();
    if(!cap_T.isOpened())
        // || !cap_T.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    }
    else
    {
        cout << "camera is open top" << endl;
        if(timer->isActive()==false)
        {
            timer->start(WIN_UPDATE_TIME);
            r_timer->start(WIN_UPDATE_TIME);
            result_timer->start(LAMI_UPDATE_TIME);
        }
    }

}
void MainWindow::on_CAM_CLOSE_clicked()
{
    cap.release();
    cap_T.release();
    cap_r.release();
    cout << "camera is closed" << endl;
}

void MainWindow::result_process()
{
	if(BOTTOM_STAGE_FLAG_r== BOTTOM_OK && BOTTOM_STAGE_FLAG == BOTTOM_OK && bottom_l_r_result!=BOTTOM_OK)
	{
		BOTTOM_SIG=BOTTOM_VISION_OK;
		bottom_l_r_result=BOTTOM_OK;
		RAMI_SEND(OFF);
		QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\PASS.jpg");
		ui->PASSNG->setPixmap(im);

		qDebug()<<"LAMI SEND OK";
	}
	else if((BOTTOM_STAGE_FLAG_r== BOTTOM_NG || BOTTOM_STAGE_FLAG == BOTTOM_NG) && bottom_l_r_result!=BOTTOM_NG)
	{
		BOTTOM_SIG=BOTTOM_VISION_NG;
		bottom_l_r_result=BOTTOM_NG;
		RAMI_SEND(OFF);
		QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\NG.jpg");
		ui->PASSNG->setPixmap(im);
		qDebug()<<"LAMI SEND NG";
	}
}

void MainWindow::update_window()
{

    if(ALL_ENABLE==true)
    {
        if(VIEW_MODE_CHANGE==false)
        {
            BOTTOM_STAGE_VISION_PRO();          
        }
        else
        {
            if(tilt_check==false)
            {
	            TOP_STAGE();
	        }
        }
    }
}
void MainWindow::update_window_r()
{
    if(ALL_ENABLE==true)
    {
        if(VIEW_MODE_CHANGE==false)
        {
            BOTTOM_STAGE_VISION_PRO_R();
        }
    }
}
void MainWindow::on_LED2_OPEN_clicked()
{
    if(!port2->open(QIODevice::ReadWrite))
    {

        QMessageBox OPENERR;
        OPENERR.setText("OPEN ERROR LED2");
        OPENERR.exec();
    }
    else
    {
        qDebug()<<"OPEN S";
        ui->LED2_OPEN->setEnabled(false);
        ui->LED2_CLOSE->setEnabled(true);
        char temp[4]={0,};
        temp[0]='<';
        temp[3]='>';
        temp[1]=LED_CON_CHECK;
        port2->write(temp,4);

    }
}

void MainWindow::on_LED1_CLOSE_clicked()
{
    port->close();
    ui->LED1_OPEN->setEnabled(true);
    ui->LED1_CLOSE->setEnabled(false);
    on_LED1_OFF_clicked();
    ui->LED1_ST->setText("DISCONNECT");


}

void MainWindow::on_LED2_CLOSE_clicked()
{
    port2->close();
    ui->LED2_OPEN->setEnabled(true);
    ui->LED2_CLOSE->setEnabled(false);
    ui->LED2_ST->setText("DISCONNECT");
    on_LED2_OFF_clicked();
}

void MainWindow::on_LED_1_VALUE_valueChanged(int arg1)
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=arg1;
    port->write(temp,4);
}

void MainWindow::on_LED_2_VALUE_valueChanged(int arg1)
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=arg1;
    port2->write(temp,4);
}

void MainWindow::on_LED1_ON_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    //  temp[1]=LED_VALUE_SET;
    temp[1]='H';
    temp[2]='I';
    // temp[2]=255;
    port->write(temp,4);
    ui->LED_1_VALUE->setValue(255);
}

void MainWindow::on_LED1_OFF_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=0;
    port->write(temp,4);
    ui->LED_1_VALUE->setValue(0);
}

void MainWindow::on_LED2_ON_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=255u;
    port2->write(temp,4);
    ui->LED_2_VALUE->setValue(255);
}

void MainWindow::on_LED2_OFF_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=0;
    port2->write(temp,4);
    ui->LED_2_VALUE->setValue(0);
}

void MainWindow::on_LED3_ON_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    //  temp[1]=LED_VALUE_SET;
    temp[1]='H';
    temp[2]='I';
    // temp[2]=255;
    port3->write(temp,4);
    ui->LED_3_VALUE->setValue(255);
}

void MainWindow::on_LED3_OFF_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=0;
    port3->write(temp,4);
    ui->LED_3_VALUE->setValue(0);
}

void MainWindow::on_LED_3_VALUE_valueChanged(int arg1)
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=LED_VALUE_SET;
    temp[2]=arg1;
    port3->write(temp,4);
}


uint8_t TEMP_LED1[4]={0,};
uint8_t TEMP_WPOS_LED1;
int read_cnt;

void MainWindow::RX_DATA_LED1()
{
    Read_temp = port->readAll();
    read_cnt = Read_temp.length();
    for(int i=0; i<read_cnt; i++)
    {
        TEMP_LED1[TEMP_WPOS_LED1]=Read_temp[i];
        TEMP_WPOS_LED1++;
    }
    if(TEMP_WPOS_LED1==4){

        if(TEMP_LED1[0]=='<' && TEMP_LED1[TEMP_WPOS_LED1-1]=='>')
        {

            DataParsing_LED1(TEMP_LED1);
            TEMP_WPOS_LED1=0;
            read_cnt=0;
        }
    }

}

uint8_t TEMP_LED2[4]={0,};
uint8_t TEMP_WPOS_LED2;
int read_cnt2;
void MainWindow::RX_DATA_LED2()
{
    Read_temp2 = port2->readAll();
    read_cnt2 = Read_temp2.length();
    for(int i=0; i<read_cnt2; i++)
    {
        TEMP_LED2[TEMP_WPOS_LED2]=Read_temp2[i];
        TEMP_WPOS_LED2++;
    }
    if(TEMP_WPOS_LED2==4){

        if(TEMP_LED2[0]=='<' && TEMP_LED2[TEMP_WPOS_LED2-1]=='>')
        {

            DataParsing_LED2(TEMP_LED2);
            TEMP_WPOS_LED2=0;
            read_cnt2=0;
        }
    }

}


uint8_t TEMP_LED3[4]={0,};
uint8_t TEMP_WPOS_LED3;
int read_cnt5;
void MainWindow::RX_DATA_LED3()
{
    Read_temp3 = port3->readAll();
    read_cnt5 = Read_temp3.length();
    for(int i=0; i<read_cnt5; i++)
    {
        TEMP_LED3[TEMP_WPOS_LED3]=Read_temp3[i];
        TEMP_WPOS_LED3++;
    }
    if(TEMP_WPOS_LED3==4){

        if(TEMP_LED3[0]=='<' && TEMP_LED3[TEMP_WPOS_LED3-1]=='>')
        {

            DataParsing_LED3(TEMP_LED3);
            TEMP_WPOS_LED3=0;
            read_cnt5=0;
        }
    }

}

void MainWindow::DataParsing_LED1(uint8_t temp[])
{
    if(*(temp+1)==LED_CON_CHECK)
    {

        if(*(temp+2)==ON)
        {
            ui->LED1_ST->setText("CONNECTED");
        }
    }
}

void MainWindow::DataParsing_LED2(uint8_t temp[])
{
    if(*(temp+1)==LED_CON_CHECK)
    {

        if(*(temp+2)==ON)
        {
            ui->LED2_ST->setText("CONNECTED");
        }
    }
}

void MainWindow::DataParsing_LED3(uint8_t temp[])
{
    if(*(temp+1)==LED_CON_CHECK)
    {

        if(*(temp+2)==ON)
        {
            ui->LED3_ST->setText("CONNECTED");
        }
    }
}

void MainWindow::time_loop(long int time)
{
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::on_pushButton_clicked()
{
    ui->PAGE_CHANGE->setCurrentWidget(ui->BOTTOM_STATE);
}

void MainWindow::on_VIEW_CHANGE_BOTTOM_clicked()
{
    ui->PAGE_CHANGE->setCurrentWidget(ui->TOP_STAGE);
}
// QTime ex_nowTime;
void MainWindow::BOTTOM_STAGE_VISION_PRO()
{
    int recogImageCnt;

    cap >> frame;

    if(frame.empty())
    {
        qDebug()<<"FRAME ERROR BOTTOM";

        timer->stop();
        on_CAM_CLOSE_clicked();
        OPEN_DELAY->start(300);

        return;
    }

    Rect rect(520, 0, bottom_image_size_x, bottom_image_size_y);
    frame=frame(rect);
    flip(frame,frame,0);

    if(STAGE_NUM_TEMP==1)
        frame= RotateImage(frame,stage_angle_l_stage1,bottom_image_size_x/2,bottom_image_size_y/2);
    else if(STAGE_NUM_TEMP==2)
        frame= RotateImage(frame,stage_angle_l_stage2,bottom_image_size_x/2,bottom_image_size_y/2);
    else if(STAGE_NUM_TEMP==3)
        frame= RotateImage(frame,stage_angle_l_stage3,bottom_image_size_x/2,bottom_image_size_y/2);
    else if(STAGE_NUM_TEMP==4)
        frame= RotateImage(frame,stage_angle_l_stage4,bottom_image_size_x/2,bottom_image_size_y/2);

    cvtColor(frame, Binary_img, CV_BGR2GRAY);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat Blur;
    Mat Thresh;
    //Mat Erode,Dilate;
    //Mat mask(3,3,CV_8U,Scalar(1));

    blur(Binary_img,Blur,Point(9,9));

    threshold(Blur,Canny_img,200,255,THRESH_BINARY|THRESH_OTSU);
    Canny(Canny_img,Canny_img,0,0,3);
    findContours( Canny_img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    recogImageCnt = 0;

    if(0)//contours.size()>0)
    {
        //vector<Moments> mu(contours.size() );
        vector<int> sortIdx(contours.size());
        vector<float> areas(contours.size());

        qDebug()<<"s---------------------------";
        for( int n = 0; n < (int)contours.size(); n++ )
        {
            sortIdx[n] = n;
            areas[n] = contourArea(contours[n], false);
            //areas[n] = arcLength(contours[n], false);   // false:openLine, true:closedLine
            //qDebug()<<areas[n];

            if(areas[n] > 1000)
            {
                recogImageCnt++;
            }
        }

        switch(recogImageCnt)
        {
            case 1:
                typeOfdevice = DEVICE_TYPE_DIRECTION;
                break;

            case 3:
                typeOfdevice = DEVICE_TYPE_POWER;
                break;

            default:
                typeOfdevice = DEVICE_TYPE_NONE;
                break;

        }

        qDebug("e---------------------------%d, %d",typeOfdevice, recogImageCnt);

        goto sError;

        std::sort( sortIdx.begin(), sortIdx.end(), AreaCmp(areas));
        ui->PASSNG->setVisible(true);

        if(typeOfdevice != DEVICE_TYPE_NONE)
        {
            Mat nonZeroCoordinates;
            float POINT1_X = 9999;
            float POINT1_Y = 0;
            float POINT2_X = 0;
            float POINT2_Y = 0;
            float POINT3_X = 0;
            float POINT3_Y = 9999;
            float POINT4_X = 0;
            float POINT4_Y = 0;

            Mat drawing(Canny_img.size(), CV_8U, cv::Scalar(0));

            drawContours( drawing, contours,sortIdx[0],cv::Scalar(255),1);

            findNonZero(drawing, nonZeroCoordinates);

            for (uint i = 0; i < nonZeroCoordinates.total(); i++ )
            {
                if(POINT1_X>nonZeroCoordinates.at<Point>(i).x)
                {
                    POINT1_X=nonZeroCoordinates.at<Point>(i).x;
                    POINT1_Y=nonZeroCoordinates.at<Point>(i).y;
                }

                if(POINT3_Y>nonZeroCoordinates.at<Point>(i).y)
                {
                    POINT3_Y=nonZeroCoordinates.at<Point>(i).y;
                    POINT3_X=nonZeroCoordinates.at<Point>(i).x;
                }

                if(POINT2_Y<nonZeroCoordinates.at<Point>(i).y)
                {
                    POINT2_Y=nonZeroCoordinates.at<Point>(i).y;
                    POINT2_X=nonZeroCoordinates.at<Point>(i).x;
                }

                if(POINT4_X<nonZeroCoordinates.at<Point>(i).x)
                {
                    POINT4_X=nonZeroCoordinates.at<Point>(i).x;
                    POINT4_Y=nonZeroCoordinates.at<Point>(i).y;
                }
            }

            if(POINT2_Y != POINT2_Y_TEMP && POINT3_Y !=POINT3_Y_TEMP && POINT1_Y!=POINT1_Y_TEMP)
            {
                POINT1_X_TEMP=POINT1_X;
                POINT2_X_TEMP=POINT2_X;
                POINT3_X_TEMP=POINT3_X;
                POINT4_X_TEMP=POINT4_X;

                POINT1_Y_TEMP=POINT1_Y;
                POINT2_Y_TEMP=POINT2_Y;
                POINT3_Y_TEMP=POINT3_Y;
                POINT4_Y_TEMP=POINT4_Y;
            }

            P_X1 = P_X1 + varP_X1;
            K_X1 = P_X1 / (P_X1 + varM_X1);
            Kalman_X1 = K_X1 * POINT1_X_TEMP + (1 - K_X1) * Kalman_X1;
            P_X1 = (1 - K_X1) * P_X1;

            P_X2 = P_X2 + varP_X2;
            K_X2 = P_X2 / (P_X2 + varM_X2);
            Kalman_X2 = K_X2 * POINT2_X_TEMP + (1 - K_X2) * Kalman_X2;
            P_X2 = (1 - K_X2) * P_X2;

            P_X3 = P_X3 + varP_X3;
            K_X3 = P_X3 / (P_X3 + varM_X3);
            Kalman_X3 = K_X3 * POINT3_X_TEMP + (1 - K_X3) * Kalman_X3;
            P_X3 = (1 - K_X3) * P_X3;

            P_X4 = P_X4 + varP_X4;
            K_X4 = P_X4 / (P_X4 + varM_X4);
            Kalman_X4 = K_X4 * POINT4_X_TEMP + (1 - K_X4) * Kalman_X4;
            P_X4 = (1 - K_X4) * P_X4;


            P_Y1 = P_Y1 + varP_Y1;
            K_Y1 = P_Y1 / (P_Y1 + varM_Y1);
            Kalman_Y1 = K_Y1 * POINT1_Y_TEMP + (1 - K_Y1) * Kalman_Y1;
            P_Y1 = (1 - K_Y1) * P_Y1;

            P_Y2 = P_Y2 + varP_Y2;
            K_Y2 = P_Y2 / (P_Y2 + varM_Y2);
            Kalman_Y2 = K_Y2 * POINT2_Y_TEMP + (1 - K_Y2) * Kalman_Y2;
            P_Y2 = (1 - K_Y2) * P_Y2;

            P_Y3 = P_Y3 + varP_Y3;
            K_Y3 = P_Y3 / (P_Y3 + varM_Y3);
            Kalman_Y3 = K_Y3 * POINT3_Y_TEMP + (1 - K_Y3) * Kalman_Y3;
            P_Y3 = (1 - K_Y3) * P_Y3;

            P_Y4 = P_Y4 + varP_Y4;
            K_Y4 = P_Y4 / (P_Y4 + varM_Y4);
            Kalman_Y4 = K_Y4 * POINT4_Y_TEMP + (1 - K_Y4) * Kalman_Y4;
            P_Y4 = (1 - K_Y4) * P_Y4;

            POINT1_X_TEMP=Kalman_X1;
            POINT2_X_TEMP=Kalman_X2;
            POINT3_X_TEMP=Kalman_X3;
            POINT4_X_TEMP=Kalman_X4;

            POINT1_Y_TEMP=Kalman_Y1;
            POINT2_Y_TEMP=Kalman_Y2;
            POINT3_Y_TEMP=Kalman_Y3;
            POINT4_Y_TEMP=Kalman_Y4;

            cvtColor(drawing,drawing,cv::COLOR_GRAY2BGR);

            char temp_y[50]={0,};
            float err_temp=0;
            ui->STAGE_TEXT->setText("X="+QString::number(contourArea(contours[sortIdx[0]])));

            if(typeOfdevice == DEVICE_TYPE_DIRECTION)
            {
                err_temp = (Kalman_X2-Kalman_X3);

                if(fabs(err_temp)<2)
                {
                    if(fabs(err_temp)>=1)
                    {
                        sprintf(temp_y,"GOOD");
                        BOTTOM_ERR_Client="G";
                        BOTTOM_X_TEMP="GOOD, err +- 75um ~ 150um";

                    }
                    else if(fabs(err_temp)<1)
                    {
                        sprintf(temp_y,"EXCELLENT");
                        BOTTOM_ERR_Client="G";
                        BOTTOM_X_TEMP="EXCELLENT, err +- 25um";
                    }

                    line(drawing,Point(POINT1_X_TEMP,POINT1_Y_TEMP),Point(POINT2_X_TEMP,POINT2_Y_TEMP),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(POINT1_X_TEMP,POINT1_Y_TEMP),Point(POINT3_X_TEMP,POINT3_Y_TEMP),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(POINT2_X_TEMP,POINT2_Y_TEMP),Point(POINT2_X_TEMP,POINT1_Y_TEMP),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(POINT3_X_TEMP,POINT3_Y_TEMP),Point(POINT3_X_TEMP,POINT1_Y_TEMP),Scalar(106,189,73),5,0,0);

                    if(BOTTOM_STAGE_FLAG!=BOTTOM_OK && interlock_flag==false)
                    {
                        BOTTOM_STAGE_FLAG=BOTTOM_OK;
                        qDebug()<<"BOTTOM OK";
                    }
                }
                else
                {
                    sprintf(temp_y,"");
                    //sprintf(temp_y,"D(X4:%.0lf)-(X1:%.0lf)=(%.0lf)",POINT4_X_TEMP, POINT1_X_TEMP, (POINT4_X_TEMP-POINT1_X_TEMP));
                    sprintf(temp_y,"D(Y2:%.0lf)-(Y3:%.0lf)=(%.0lf)",POINT2_Y_TEMP, POINT3_Y_TEMP, (POINT2_Y_TEMP-POINT3_Y_TEMP));
                    BOTTOM_ERR_Client="B";
                    BOTTOM_X_TEMP="BAD";

                    line(drawing,Point(POINT1_X_TEMP,POINT1_Y_TEMP),Point(POINT2_X_TEMP,POINT2_Y_TEMP),Scalar(255,0,0),5,0,0);
                    line(drawing,Point(POINT1_X_TEMP,POINT1_Y_TEMP),Point(POINT3_X_TEMP,POINT3_Y_TEMP),Scalar(255,0,0),5,0,0);
                    line(drawing,Point(POINT2_X_TEMP,POINT2_Y_TEMP),Point(POINT2_X_TEMP,POINT1_Y_TEMP),Scalar(255,0,0),5,0,0);
                    line(drawing,Point(POINT3_X_TEMP,POINT3_Y_TEMP),Point(POINT3_X_TEMP,POINT1_Y_TEMP),Scalar(255,0,0),5,0,0);

                    if(BOTTOM_STAGE_FLAG!=BOTTOM_NG)
                    {
                        BOTTOM_STAGE_FLAG=BOTTOM_NG;
                    }
                }

                line(drawing,Point(0,POINT1_Y_TEMP),Point(POINT1_X_TEMP,POINT1_Y_TEMP),Scalar(197,10,2),2);
                line(drawing,Point(POINT1_X_TEMP,POINT1_Y_TEMP),Point(bottom_image_size_x,POINT1_Y_TEMP),Scalar(197,10,2),2,0,0);

                circle(drawing,Point(POINT1_X_TEMP,POINT1_Y_TEMP),3,Scalar(255,255,255));
                //  circle(drawing,Point(mc[0].x,mc[0].y),3,Scalar(255,255,255));
                circle(drawing,Point(POINT2_X_TEMP,POINT2_Y_TEMP),3,Scalar(255,255,255));
                circle(drawing,Point(POINT3_X_TEMP,POINT3_Y_TEMP),3,Scalar(255,255,255));
                ///////////////////////////////////////////////////////////////////////////////////////////

            }
            else if(typeOfdevice == DEVICE_TYPE_POWER)
            {
                //////////////////////////////////////////////
                err_temp = (Kalman_Y2-Kalman_Y3) - DISTING_RS4;

                if(fabs(err_temp)<2)
                {
                    if(fabs(err_temp) >= 1)
                    {
                        sprintf(temp_y,"GOOD %.2f",err_temp);
                        //sprintf(temp_y,"GOOD");
                        BOTTOM_ERR_Client="G";
                        BOTTOM_X_TEMP="GOOD, err +- 75um ~ 150um";

                    }
                    else // if(fabs(err_temp)<1)
                    {
                        sprintf(temp_y,"EXCELLENT %.2f",err_temp);
                        //sprintf(temp_y,"EXCELLENT");
                        BOTTOM_ERR_Client="G";
                        BOTTOM_X_TEMP="EXCELLENT, err +- 25um";
                    }

                    if(BOTTOM_STAGE_FLAG!=BOTTOM_OK && interlock_flag==false)
                    {
                        BOTTOM_STAGE_FLAG=BOTTOM_OK;
                        qDebug()<<"BOTTOM OK";
                    }

                    line(drawing,Point(0,POINT2_Y_TEMP),Point(POINT2_X_TEMP,POINT2_Y_TEMP),Scalar(106,189,73),2);
                    line(drawing,Point(POINT2_X_TEMP,POINT2_Y_TEMP),Point(bottom_image_size_x,POINT2_Y_TEMP),Scalar(106,189,73),2,0,0);

                    line(drawing,Point(0,POINT3_Y_TEMP),Point(POINT3_X_TEMP,POINT3_Y_TEMP),Scalar(106,189,73),2);
                    line(drawing,Point(POINT3_X_TEMP,POINT3_Y_TEMP),Point(bottom_image_size_x,POINT3_Y_TEMP),Scalar(106,189,73),2,0,0);
                }
                else
                {
                    //sprintf(temp_y,"");
                    //sprintf(temp_y,"(Y2:%.0lf)-(Y3:%.0lf)=%.0lf",Kalman_Y2, Kalman_Y3,fabs(err_temp));
                    //sprintf(temp_y,"P(X4:%.0lf)-(X1:%.0lf)=(%.0lf)",POINT4_X_TEMP, POINT1_X_TEMP, (POINT4_X_TEMP-POINT1_X_TEMP));
                    sprintf(temp_y,"P(Y2:%.0lf)-(Y3:%.0lf)=(%.0lf)",POINT2_Y_TEMP, POINT3_Y_TEMP, (POINT2_Y_TEMP-POINT3_Y_TEMP));
					BOTTOM_ERR_Client="B";
                    BOTTOM_X_TEMP="BAD";

                    if(BOTTOM_STAGE_FLAG!=BOTTOM_NG)
                    {
                        BOTTOM_STAGE_FLAG=BOTTOM_NG;
                    }

                    line(drawing,Point(0,POINT2_Y_TEMP),Point(POINT2_X_TEMP,POINT2_Y_TEMP),Scalar(197,10,2),2);
                    line(drawing,Point(POINT2_X_TEMP,POINT2_Y_TEMP),Point(bottom_image_size_x,POINT2_Y_TEMP),Scalar(197,10,2),2,0,0);

                    line(drawing,Point(0,POINT3_Y_TEMP),Point(POINT3_X_TEMP,POINT3_Y_TEMP),Scalar(197,10,2),2);
                    line(drawing,Point(POINT3_X_TEMP,POINT3_Y_TEMP),Point(bottom_image_size_x,POINT3_Y_TEMP),Scalar(197,10,2),2,0,0);
                }



                circle(drawing,Point(POINT3_X_TEMP,POINT3_Y_TEMP),3,Scalar(255,255,255));
                circle(drawing,Point(POINT2_X_TEMP,POINT2_Y_TEMP),3,Scalar(255,255,255));
            }

            Last_img=drawing;

            putText(Last_img,temp_y,Point(20,100),1,2,Scalar(255,255,255));

            line(Last_img,Point(0,bottom_image_size_y/2),Point(bottom_image_size_x,bottom_image_size_y/2),Scalar(255,0,0),2);
            qt_image = QImage(static_cast<const unsigned char*>(Last_img.data), Last_img.cols, Last_img.rows,Last_img.step, QImage::Format_RGB888);
            //  qt_image = QImage(static_cast<const unsigned char*>(Canny_img.data), Canny_img.cols, Canny_img.rows, QImage::Format_Grayscale8);
            ui->BOTTOM_VIDEO->setPixmap(QPixmap::fromImage(qt_image));
            ui->BOTTOM_VIDEO->resize(ui->BOTTOM_VIDEO->pixmap()->size());

        }
        else
        {
            line(frame,Point(0,bottom_image_size_y/2),Point(bottom_image_size_x,bottom_image_size_y/2),Scalar(255,0,0),2);
            qt_image = QImage(static_cast<const unsigned char*>(frame.data), frame.cols, frame.rows,frame.step, QImage::Format_RGB888);
            //  qt_image = QImage(static_cast<const unsigned char*>(Canny_img.data), Canny_img.cols, Canny_img.rows, QImage::Format_Grayscale8);
            ui->BOTTOM_VIDEO->setPixmap(QPixmap::fromImage(qt_image));
            ui->BOTTOM_VIDEO->resize(ui->BOTTOM_VIDEO->pixmap()->size());
        }

    }
    else
    {
        //BOTTOM_POS_CHECK=true;
        // line(frame,Point(0,360),Point(1280,360),Scalar(255,0,0),2);
        qt_image = QImage(static_cast<const unsigned char*>(frame.data), frame.cols, frame.rows,frame.step, QImage::Format_RGB888);
        //  qt_image = QImage(static_cast<const unsigned char*>(Canny_img.data), Canny_img.cols, Canny_img.rows, QImage::Format_Grayscale8);
        ui->BOTTOM_VIDEO->setPixmap(QPixmap::fromImage(qt_image));
        ui->BOTTOM_VIDEO->resize(ui->BOTTOM_VIDEO->pixmap()->size());
    }
    //line(frame,Point(0,TEST_Y),Point(640,TEST_Y),Scalar(255,0,0),1);
    // Origin_Image = QImage(static_cast<const unsigned char*>(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    // ui->Origin_View->setPixmap(QPixmap::fromImage(Origin_Image));
    // ui->Origin_View->resize(ui->Origin_View->pixmap()->size());
    // ui->Cen->setText("X_POS="+QString::number(contourArea(contours[0],false)));

    if(tilt_check==true) timer->stop();

    return;

sError:
    return;
}

void MainWindow::BOTTOM_STAGE_VISION_PRO_R()
{
    cap_r >> frame_r;

    if(frame_r.empty()) 
    {
	    qDebug()<<"FRAME ERROR BOTTOM";

        timer->stop();
        on_CAM_CLOSE_clicked();
        OPEN_DELAY->start(300);

        return;
    }

    Rect rect(520, 0, bottom_image_size_x, bottom_image_size_y);
    frame_r=frame_r(rect);

    if(STAGE_NUM_TEMP==1)
        frame_r= RotateImage(frame_r,stage_angle_r_stage1,bottom_image_size_x/2,bottom_image_size_y/2);
    else if(STAGE_NUM_TEMP==2)
        frame_r= RotateImage(frame_r,stage_angle_r_stage2,bottom_image_size_x/2,bottom_image_size_y/2);
    else if(STAGE_NUM_TEMP==3)
        frame_r= RotateImage(frame_r,stage_angle_r_stage3,bottom_image_size_x/2,bottom_image_size_y/2);
    else if(STAGE_NUM_TEMP==4)
        frame_r= RotateImage(frame_r,stage_angle_r_stage4,bottom_image_size_x/2,bottom_image_size_y/2);
        
    flip(frame_r,frame_r,0);
    cvtColor(frame_r, Binary_img_r, CV_BGR2GRAY);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat Blur;
    Mat Thresh;

    blur(Binary_img_r,Blur,Point(9,9));

    threshold(Blur,Canny_img_r,200,255,THRESH_BINARY|THRESH_OTSU);
    Canny(Canny_img_r,Canny_img_r,0,0,3);

    findContours( Canny_img_r, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    if(contours.size()>0)
    {
        if(contours.size() == 1)    // image 1
        {
            typeOfdevice_r = DEVICE_TYPE_DIRECTION;
        }
        else if(contours.size() == 3)    // image 3
        {
            typeOfdevice_r = DEVICE_TYPE_POWER;
        }
        else
        {
            typeOfdevice_r = DEVICE_TYPE_NONE;
        }
        //vector<Moments> mu(contours.size());
        vector<int> sortIdx(contours.size());
        vector<float> areas(contours.size());
        for( int n = 0; n < (int)contours.size(); n++)
        {
            sortIdx[n] = n;
            areas[n] = contourArea(contours[n], false);
        }
        std::sort( sortIdx.begin(), sortIdx.end(), AreaCmp(areas));
        ui->PASSNG_r->setVisible(true);

        if(contourArea(contours[sortIdx[typeOfdevice_r]])>100 && typeOfdevice_r != DEVICE_TYPE_NONE)
        {
            Mat nonZeroCoordinates;
            float POINT1_X = 9999;
            float POINT1_Y = 0;
            float POINT2_X = 0;
            float POINT2_Y = 0;
            float POINT3_X = 0;
            float POINT3_Y = 9999;
            float POINT4_X = 0;
            float POINT4_Y = 0;

            Mat drawing(Canny_img_r.size(), CV_8U, cv::Scalar(0));

            drawContours( drawing, contours,sortIdx[typeOfdevice_r],cv::Scalar(255),1);

            findNonZero(drawing, nonZeroCoordinates);

            for (uint i = 0; i < nonZeroCoordinates.total(); i++ )
            {
                if(POINT1_X>nonZeroCoordinates.at<Point>(i).x)
                {
                    POINT1_X=nonZeroCoordinates.at<Point>(i).x;
                    POINT1_Y=nonZeroCoordinates.at<Point>(i).y;
                }
                if(POINT3_Y>nonZeroCoordinates.at<Point>(i).y)
                {
                    POINT3_Y=nonZeroCoordinates.at<Point>(i).y;
                    POINT3_X=nonZeroCoordinates.at<Point>(i).x;
                }
                if(POINT2_Y<nonZeroCoordinates.at<Point>(i).y)
                {
                    POINT2_Y=nonZeroCoordinates.at<Point>(i).y;
                    POINT2_X=nonZeroCoordinates.at<Point>(i).x;
                }

                if(POINT4_X<nonZeroCoordinates.at<Point>(i).x)
                {
                    POINT4_X=nonZeroCoordinates.at<Point>(i).x;
                    POINT4_Y=nonZeroCoordinates.at<Point>(i).y;
                }
            }

            if(POINT2_Y != POINT2_Y_TEMP_r && POINT3_Y !=POINT3_Y_TEMP_r && POINT1_Y!=POINT1_Y_TEMP_r)
            {
                POINT1_X_TEMP_r=POINT1_X;
                POINT2_X_TEMP_r=POINT2_X;
                POINT3_X_TEMP_r=POINT3_X;
                POINT4_X_TEMP_r=POINT4_X;

                POINT1_Y_TEMP_r=POINT1_Y;
                POINT2_Y_TEMP_r=POINT2_Y;
                POINT3_Y_TEMP_r=POINT3_Y;
                POINT4_Y_TEMP_r=POINT4_Y;
            }

            P_X1_r = P_X1_r + varP_X1_r;
            K_X1_r = P_X1_r / (P_X1_r + varM_X1_r);
            Kalman_X1_r = K_X1_r * POINT1_X_TEMP_r + (1 - K_X1_r) * Kalman_X1_r;
            P_X1_r = (1 - K_X1_r) * P_X1_r;

            P_X2_r = P_X2_r + varP_X2_r;
            K_X2_r = P_X2_r / (P_X2_r + varM_X2_r);
            Kalman_X2_r = K_X2_r * POINT2_X_TEMP_r + (1 - K_X2_r) * Kalman_X2_r;
            P_X2_r = (1 - K_X2_r) * P_X2_r;

            P_X3_r = P_X3_r + varP_X3_r;
            K_X3_r = P_X3_r / (P_X3_r + varM_X3_r);
            Kalman_X3_r = K_X3_r * POINT3_X_TEMP_r + (1 - K_X3_r) * Kalman_X3_r;
            P_X3_r = (1 - K_X3_r) * P_X3_r;

            P_X4_r = P_X4_r + varP_X4_r;
            K_X4_r = P_X4_r / (P_X4_r + varM_X4_r);
            Kalman_X4_r = K_X4_r * POINT4_X_TEMP_r + (1 - K_X4_r) * Kalman_X4_r;
            P_X4_r = (1 - K_X4_r) * P_X4_r;


            P_Y1_r = P_Y1_r + varP_Y1_r;
            K_Y1_r = P_Y1_r / (P_Y1_r + varM_Y1_r);
            Kalman_Y1_r = K_Y1_r * POINT1_Y_TEMP_r + (1 - K_Y1_r) * Kalman_Y1_r;
            P_Y1_r = (1 - K_Y1_r) * P_Y1_r;

            P_Y2_r = P_Y2_r + varP_Y2_r;
            K_Y2_r = P_Y2_r / (P_Y2_r + varM_Y2_r);
            Kalman_Y2_r = K_Y2_r * POINT2_Y_TEMP_r + (1 - K_Y2_r) * Kalman_Y2_r;
            P_Y2_r = (1 - K_Y2_r) * P_Y2_r;

            P_Y3_r = P_Y3_r + varP_Y3_r;
            K_Y3_r = P_Y3_r / (P_Y3_r + varM_Y3_r);
            Kalman_Y3_r = K_Y3_r * POINT3_Y_TEMP_r + (1 - K_Y3_r) * Kalman_Y3_r;
            P_Y3_r = (1 - K_Y3_r) * P_Y3_r;

            P_Y4_r = P_Y4_r + varP_Y4_r;
            K_Y4_r = P_Y4_r / (P_Y4_r + varM_Y4_r);
            Kalman_Y4_r = K_Y4_r * POINT4_Y_TEMP_r + (1 - K_Y4_r) * Kalman_Y4_r;
            P_Y4_r = (1 - K_Y4_r) * P_Y4_r;

            POINT1_X_TEMP_r=Kalman_X1_r;
            POINT2_X_TEMP_r=Kalman_X2_r;
            POINT3_X_TEMP_r=Kalman_X3_r;
            POINT4_X_TEMP_r=Kalman_X4_r;

            POINT1_Y_TEMP_r=Kalman_Y1_r;
            POINT2_Y_TEMP_r=Kalman_Y2_r;
            POINT3_Y_TEMP_r=Kalman_Y3_r;
            POINT4_Y_TEMP_r=Kalman_Y4_r;

            cvtColor(drawing,drawing,CV_GRAY2BGR);

            char temp_y[50]={0,};
            float err_temp=0;

            if(typeOfdevice_r == DEVICE_TYPE_DIRECTION)
            {
                err_temp = (Kalman_X2_r-Kalman_X3_r);

                if(fabs(err_temp)<2)
                {
                    if(fabs(err_temp)>=1)
                    {
                        sprintf(temp_y,"GOOD");
                        BOTTOM_ERR_Client_r="G";
                        BOTTOM_X_TEMP_r="GOOD, err +- 75um ~ 150um";

                    }
                    else if(fabs(err_temp)<1)
                    {
                        sprintf(temp_y,"EXCELLENT");
                        BOTTOM_ERR_Client_r="G";
                        BOTTOM_X_TEMP_r="EXCELLENT, err +- 25um";
                    }

                    line(drawing,Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Point(POINT2_X_TEMP_r,POINT1_Y_TEMP_r),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Point(POINT3_X_TEMP_r,POINT1_Y_TEMP_r),Scalar(106,189,73),5,0,0);

                    if(BOTTOM_STAGE_FLAG_r!=BOTTOM_OK && interlock_flag==false)
                    {
                        BOTTOM_SIG_r=BOTTOM_VISION_OK;
                        BOTTOM_STAGE_FLAG_r=BOTTOM_OK;
                        //RAMI_SEND(OFF);
                       // QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\PASS.jpg");
                       // ui->PASSNG_r->setPixmap(im);
                       // bottom_r_result=true;
                        qDebug()<<"BOTTOM OK";
                    }
                }
                else
                {
                    //sprintf(temp_y,"");
                     //sprintf(temp_y,"diff(%.3lf)",err_temp);
                    //sprintf(temp_y,"(X4:%.0lf)-(X1:%.0lf)=(%.0lf)",POINT4_X_TEMP_r, POINT1_X_TEMP_r, (POINT4_X_TEMP_r-POINT1_X_TEMP_r));
                    sprintf(temp_y,"P(Y2:%.0lf)-(Y3:%.0lf)=(%.0lf)",POINT2_Y_TEMP_r, POINT3_Y_TEMP_r, (POINT2_Y_TEMP_r-POINT3_Y_TEMP_r));
                    BOTTOM_ERR_Client_r="B";
                    BOTTOM_X_TEMP_r="BAD";

                    line(drawing,Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Scalar(255,0,0),5,0,0);
                    line(drawing,Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Scalar(255,0,0),5,0,0);
                    line(drawing,Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Point(POINT2_X_TEMP_r,POINT1_Y_TEMP_r),Scalar(255,0,0),5,0,0);
                    line(drawing,Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Point(POINT3_X_TEMP_r,POINT1_Y_TEMP_r),Scalar(255,0,0),5,0,0);

                    if(BOTTOM_STAGE_FLAG_r!=BOTTOM_NG )
                    {
                        //BOTTOM_SIG_r=BOTTOM_VISION_NG;
                        BOTTOM_STAGE_FLAG_r=BOTTOM_NG;
                        //RAMI_SEND(OFF);
                        qDebug()<<"BOTTOM_R NG";
                       // QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\NG.jpg");
                       // ui->PASSNG_r->setPixmap(im);
                        bottom_r_result=false;
                    }
                }
                line(drawing,Point(0,POINT1_Y_TEMP_r),Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),Scalar(197,10,2),2);
                line(drawing,Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),Point(bottom_image_size_x,POINT1_Y_TEMP_r),Scalar(197,10,2),2,0,0);

                circle(drawing,Point(POINT1_X_TEMP_r,POINT1_Y_TEMP_r),3,Scalar(255,255,255));
                //  circle(drawing,Point(mc[0].x,mc[0].y),3,Scalar(255,255,255));
                circle(drawing,Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),3,Scalar(255,255,255));
                circle(drawing,Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),3,Scalar(255,255,255));
            }
            else if(typeOfdevice_r == DEVICE_TYPE_POWER)
            {
                err_temp = (Kalman_Y2_r-Kalman_Y3_r) - DISTING_RS4_r;

                //qDebug()<<err_temp;
                //err_temp=POINT2_X_TEMP-POINT3_X_TEMP;
                if(fabs(err_temp)< 2)
                {
                    if(fabs(err_temp) >= 1)
                    {
                        sprintf(temp_y,"GOOD %.2f",err_temp);
                        //sprintf(temp_y,"GOOD");
                        BOTTOM_ERR_Client_r="G";
                        BOTTOM_X_TEMP_r="GOOD, err +- 75um ~ 150um";

                    }
                    else //if(fabs(err_temp)<1)
                    {
                        sprintf(temp_y,"EXCELLENT %.2f",err_temp);
                        //sprintf(temp_y,"EXCELLENT");
                        BOTTOM_ERR_Client_r="G";
                        BOTTOM_X_TEMP_r="EXCELLENT, err +- 25um";
                    }

                    if(BOTTOM_STAGE_FLAG_r!=BOTTOM_OK && interlock_flag==false)
                    {
                        //bottom_check_cnt_r=0;
                        BOTTOM_SIG_r=BOTTOM_VISION_OK;
                        BOTTOM_STAGE_FLAG_r=BOTTOM_OK;
                        //RAMI_SEND(OFF);
                       // QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\PASS.jpg");
                       // ui->PASSNG_r->setPixmap(im);
                       // bottom_r_result=true;
                        qDebug()<<"BOTTOM OK";
                    }

                    line(drawing,Point(0,POINT2_Y_TEMP_r),Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Scalar(106,189,73),5);
                    line(drawing,Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Point(bottom_image_size_x,POINT2_Y_TEMP_r),Scalar(106,189,73),5,0,0);
                    line(drawing,Point(0,POINT3_Y_TEMP_r),Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Scalar(106,189,73),5);
                    line(drawing,Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Point(bottom_image_size_x,POINT3_Y_TEMP_r),Scalar(106,189,73),5,0,0);
                }
                else
                {
                    //sprintf(temp_y,"");
                   // sprintf(temp_y,"(X4:%.0lf)-(X1:%.0lf)=(%.0lf)",POINT4_X_TEMP_r, POINT1_X_TEMP_r, (POINT4_X_TEMP_r-POINT1_X_TEMP_r));
                    sprintf(temp_y,"(Y2:%.0lf)-(Y3:%.0lf)=(%.0lf)",POINT2_Y_TEMP_r, POINT3_Y_TEMP_r, (POINT2_Y_TEMP_r-POINT3_Y_TEMP_r));
                    BOTTOM_ERR_Client_r="B";
                    BOTTOM_X_TEMP_r="BAD";

                    if(BOTTOM_STAGE_FLAG_r!=BOTTOM_NG )
                    {

                        //BOTTOM_SIG_r=BOTTOM_VISION_NG;
                        BOTTOM_STAGE_FLAG_r=BOTTOM_NG;
                        //RAMI_SEND(OFF);
                        qDebug()<<"BOTTOM_R NG";
                       // QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\NG.jpg");
                       // ui->PASSNG_r->setPixmap(im);
                        bottom_r_result=false;
                    }

                    line(drawing,Point(0,POINT2_Y_TEMP_r),Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Scalar(197,10,2),2);
                    line(drawing,Point(POINT2_X_TEMP_r,POINT2_Y_TEMP_r),Point(bottom_image_size_x,POINT2_Y_TEMP_r),Scalar(197,10,2),2,0,0);
                    line(drawing,Point(0,POINT3_Y_TEMP_r),Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Scalar(197,10,2),2);
                    line(drawing,Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),Point(bottom_image_size_x,POINT3_Y_TEMP_r),Scalar(197,10,2),2,0,0);
                }


                circle(drawing,Point(POINT3_X_TEMP_r,POINT3_Y_TEMP_r),3,Scalar(255,255,255));
                circle(drawing,Point(POINT4_X_TEMP_r,POINT4_Y_TEMP_r),3,Scalar(255,255,255));
            }

            Last_img_r=drawing;
            putText(Last_img_r,temp_y,Point(20,100),1,2,Scalar(255,255,255));

            line(Last_img_r,Point(0,bottom_image_size_y/2),Point(bottom_image_size_x,bottom_image_size_y/2),Scalar(255,0,0),2);
            qt_image_r = QImage(static_cast<const unsigned char*>(Last_img_r.data), Last_img_r.cols, Last_img_r.rows,Last_img_r.step, QImage::Format_RGB888);
            //  qt_image = QImage(static_cast<const unsigned char*>(Canny_img.data), Canny_img.cols, Canny_img.rows, QImage::Format_Grayscale8);
            ui->BOTTOM_VIDEO_r->setPixmap(QPixmap::fromImage(qt_image_r));

         //   ui->BOTTOM_VIDEO_r->resize(ui->BOTTOM_VIDEO_r->pixmap()->size());

        }
        else
        {
            line(frame_r,Point(0,bottom_image_size_y/2),Point(bottom_image_size_x,bottom_image_size_y/2),Scalar(255,0,0),2);
            qt_image_r = QImage(static_cast<const unsigned char*>(frame_r.data), frame_r.cols, frame_r.rows,frame_r.step, QImage::Format_RGB888);
            //  qt_image = QImage(static_cast<const unsigned char*>(Canny_img.data), Canny_img.cols, Canny_img.rows, QImage::Format_Grayscale8);
            ui->BOTTOM_VIDEO_r->setPixmap(QPixmap::fromImage(qt_image_r));
         //  ui->BOTTOM_VIDEO_r->resize(ui->BOTTOM_VIDEO_r->pixmap()->size());
        }
    }
    else
    {
        //BOTTOM_POS_CHECK=true;
        // line(frame,Point(0,360),Point(1280,360),Scalar(255,0,0),2);
        qt_image_r = QImage(static_cast<const unsigned char*>(frame_r.data), frame_r.cols, frame_r.rows,frame_r.step, QImage::Format_RGB888);
        //  qt_image = QImage(static_cast<const unsigned char*>(Canny_img.data), Canny_img.cols, Canny_img.rows, QImage::Format_Grayscale8);
        ui->BOTTOM_VIDEO_r->setPixmap(QPixmap::fromImage(qt_image_r));
       // ui->BOTTOM_VIDEO_r->resize(ui->BOTTOM_VIDEO_r->pixmap()->size());
    }
    //line(frame,Point(0,TEST_Y),Point(640,TEST_Y),Scalar(255,0,0),1);
    // Origin_Image = QImage(static_cast<const unsigned char*>(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    // ui->Origin_View->setPixmap(QPixmap::fromImage(Origin_Image));
    // ui->Origin_View->resize(ui->Origin_View->pixmap()->size());
    // ui->Cen->setText("X_POS="+QString::number(contourArea(contours[0],false)));

      if(tilt_check==true) r_timer->stop();

      return;

sError:
      return;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
   // port->setPortName(arg1);
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
   // port2->setPortName(arg1);
}

void MainWindow::on_LED1_OPEN_clicked()
{
    if(!port->open(QIODevice::ReadWrite))
    {

        QMessageBox OPENERR;
        OPENERR.setText("OPEN ERROR LED1");
        OPENERR.exec();
    }
    else
    {
        qDebug()<<"OPEN S";
        ui->LED1_OPEN->setEnabled(false);
        ui->LED1_CLOSE->setEnabled(true);
        char temp[4]={0,};
        temp[0]='<';
        temp[3]='>';
        temp[1]=LED_CON_CHECK;
        port->write(temp,4);
    }
}
bool MainWindow::ContourAreaSortPredicate(const std::vector<cv::Point> c1, const std::vector<cv::Point> c2)
{
    return cv::contourArea(c1) > cv::contourArea(c2);
}


void MainWindow::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
  //  RAMI_EQ->setPortName(arg1);
}

void MainWindow::on_VQLAMI_OPEN_clicked()
{
    if(!RAMI_EQ->open(QIODevice::ReadWrite))
    {

        QMessageBox OPENERR;
        OPENERR.setText("OPEN ERROR VQLAMI");
        OPENERR.exec();
    }
    else
    {
        qDebug()<<"OPEN 3";
        ui->VQLAMI_OPEN->setEnabled(false);
        ui->VQLAMI_CLOSE->setEnabled(true);
    }
}

void MainWindow::on_VQLAMI_CLOSE_clicked()
{
    RAMI_EQ->close();
    ui->VQLAMI_OPEN->setEnabled(true);
    ui->VQLAMI_CLOSE->setEnabled(false);
}
uint8_t TEMP_DATA_LAMI[20]={0,};
uint8_t TEMP_WPOS_RAMI;
int read_cnt3;
void MainWindow::RX_DATA_RAMI()
{
    Read_temp3 = RAMI_EQ->readAll();
    read_cnt3 = Read_temp3.length();
    for(int i=0; i<read_cnt3; i++)
    {
        TEMP_DATA_LAMI[TEMP_WPOS_RAMI]=Read_temp3[i];
        TEMP_WPOS_RAMI++;

    }
    if(TEMP_WPOS_RAMI==20){

        if(TEMP_DATA_LAMI[0]=='<' && TEMP_DATA_LAMI[TEMP_WPOS_RAMI-1]=='>')
        {
            DataParsing_LAMI(TEMP_DATA_LAMI);
            TEMP_WPOS_RAMI=0;
            read_cnt3=0;
        }
    }
    else if(TEMP_WPOS_RAMI>20)
    {
        TEMP_WPOS_RAMI=0;
        read_cnt3=0;
    }
}
void MainWindow::DataParsing_LAMI(uint8_t temp[])
{
    if(*(temp+1)==RAMI_STATUS)
    {
        //qDebug()<<"STATUS LAMI";
        if(*(temp+2)!=STAGE_NUM_TEMP)
        {
            STAGE_CHECK();
            ui->VQLAMI_RX->setText(QString::number(*(temp+2)));
            STAGE_NUM_TEMP=*(temp+2);
            ui->STAGE_NUMBER->setText("STAGE "+QString::number(STAGE_NUM_TEMP));
            BOTTOM_STAGE_FLAG=0;

            if(BOTTOM_STAGE_FLAG_r==BOTTOM_OK)
            BOTTOM_STAGE_FLAG_r=BOTTOM_NG;
            if(BOTTOM_STAGE_FLAG==BOTTOM_OK)
            BOTTOM_STAGE_FLAG=BOTTOM_NG;
        }
        qDebug()<<"STAGE NUM"+QString::number(STAGE_NUM_TEMP);

        if(*(temp+3)==TOWER_AUTO)
        {
            LED_TOWER_Client="TOWER_AUTO";
            qDebug()<<"TOWER AUTO";
        }
        else if(*(temp+3)==TOWER_WAIT)
        {
            LED_TOWER_Client="TOWER_WAIT";
            qDebug()<<"TOWER WAIT";
        }
        else if(*(temp+3)==TOWER_ERROR)
        {
            LED_TOWER_Client="TOWER_ERROR";
            qDebug()<<"TOWER ERROR";
        }

        if(*(temp+4)==OFF)
        {
            PUMP_STATUS_Client="PUMP OFF";
            qDebug()<<"PUMP OFF";
        }
        else if(*(temp+4)==ON)
        {
            PUMP_STATUS_Client="PUMP ON";
            qDebug()<<"PUMP ON";
        }

        if(*(temp+5)==ON)
        {
            //if(LOT_COUNT>0)
            //{
            //  save_t->run();
            //}
            qDebug()<<"BT_CLICK";

        }
        else if(*(temp+5)==OFF)
        {
            qDebug()<<"BT_RELEASE";
        }
        if(*(temp+6)==ON)
        {
            ui->VQLAMI_DOOR->setText("ON");
            qDebug()<<"DOOR EMG ON";
        }
        else if(*(temp+6)==OFF)
        {
            ui->VQLAMI_DOOR->setText("OFF");
            qDebug()<<"DOOR EMG OFF";
        }


        if(*(temp+7)==ON)
        {
            ui->VQLAMI_TURN->setText("ON");
            on_LED2_OFF_clicked();
            on_LED1_OFF_clicked();
            on_LED3_OFF_clicked();
            qDebug()<<"STAGE TURN";
        }
        if(*(temp+8)==ON)
        {
            qDebug()<<"PRESS_END_ON_SIG";
            PRESS_STATUS_Client="PRESS_UP";
        }
        if(*(temp+8)==OFF)
        {
            qDebug()<<"PRESS_END_OFF_SIG";
            PRESS_STATUS_Client="PRESS_NONE";
        }
        if(*(temp+9)==OFF)
        {
            ui->PASSNG->setVisible(true);
            if(LOT_COUNT<4)
                VIEW_MODE_CHANGE=true;
            else
                VIEW_MODE_CHANGE=false;
            VIEW_SETTING(VIEW_MODE_CHANGE);
            qDebug()<<"INTERLOCK OFF";
            interlock_flag=false;
        }
        else if(*(temp+9)==ON)
        {
            ui->PASSNG->setVisible(false);
            interlock_flag=true;
            if(VIEW_MODE_CHANGE==true){
                VIEW_MODE_CHANGE=false;
                VIEW_SETTING(VIEW_MODE_CHANGE);
            }
            qDebug()<<"INTERLOCK ON";
        }
        if(*(temp+10)==OFF)
        {
            // qDebug()<<"Communication ON";
            // if(RAMI_TIMER->isActive()==false)
            //    RAMI_TIMER->start(200);
        }
        else if(*(temp+10)==ON)
        {
            // qDebug()<<"Communication OFF";
            //  if(RAMI_TIMER->isActive()==true)
            //RAMI_TIMER->stop();
        }

        if(tcp_send_en==true)
            tcp_send();
    }
}

void MainWindow::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
  //  VISION_MOTOR->setPortName(arg1);
}

void MainWindow::on_VISION_OPEN_clicked()
{
    if(!VISION_MOTOR->open(QIODevice::ReadWrite))
    {

        QMessageBox OPENERR;
        OPENERR.setText("OPEN ERROR");
        OPENERR.exec();
    }
    else
    {
        qDebug()<<"OPEN 4";
        ui->VISION_OPEN->setEnabled(false);
        ui->VISION_CLOSE->setEnabled(true);
    }
}

void MainWindow::on_VISION_CLOSE_clicked()
{
    VISION_MOTOR->close();
    ui->VISION_OPEN->setEnabled(true);
    ui->VISION_CLOSE->setEnabled(false);
}
uint8_t TEMP_DATA_VISION[4]={0,};
uint8_t TEMP_WPOS_VISION;
int read_cnt4;
void MainWindow::RX_DATA_VISION_MOTOR()
{
    Read_temp4 = VISION_MOTOR->readAll();
    read_cnt4 = Read_temp4.length();
    for(int i=0; i<read_cnt4; i++)
    {
        TEMP_DATA_VISION[TEMP_WPOS_VISION]=Read_temp4[i];
        TEMP_WPOS_VISION++;
        qDebug()<<Read_temp4[i];
    }

    if(TEMP_WPOS_VISION==4){

        if(TEMP_DATA_VISION[0]=='<' && TEMP_DATA_VISION[TEMP_WPOS_VISION-1]=='>')
        {

            DataParsing_VISION_MOTOR(TEMP_DATA_VISION);
            TEMP_WPOS_VISION=0;
            read_cnt4=0;

        }
    }
    else if(TEMP_WPOS_VISION>4)
    {
        TEMP_WPOS_VISION=0;
        read_cnt4=0;
    }
}
void MainWindow::DataParsing_VISION_MOTOR(uint8_t temp[])
{
    if(*(temp+1)==0x01)
    {
        ui->VISION_RX->setText(QString::number(*(temp+2)));
    }
    else if(*(temp+1)==GOAL_STAGE_POINT1)
    {
        TOP_CHECK_TM->start(1500);
    }
    else if(*(temp+1)==GOAL_STAGE_POINT2)
    {
        TOP_CHECK_TM->start(1500);
    }
    else if(*(temp+1)==GOAL_STAGE_POINT_WAIT)
    {
        TOP_VIEW_IMAGE_CH=OFF;
        VIEW_MODE_CHANGE=false;
        VIEW_SETTING(VIEW_MODE_CHANGE);
    }
    else if(*(temp+1)==BUTTON_STATE)
    {
        if(*(temp+2)==BUTTON_ON)
        {
            POINT_STATE=0;
            ui->BUTTON->setText("BUTTON ON");
            on_pushButton_4_clicked();
        }
        else if(*(temp+2)==BUTTON_OFF)
        {
            ui->BUTTON->setText("BUTTON OFF");
        }
    }
    else if(*(temp+1)==VISION_ENABLE)
    {
        if(VIEW_MODE_CHANGE==true)
        {
            char temp[4]={0,};
            temp[0]='<';
            temp[3]='>';
            temp[1]=STAGE_WRITE;
            temp[2]=STAGE_POINT1_MOVE;
            VISION_MOTOR->write(temp,4);
        }
    }
    else if(*(temp+1)==VISION_CHECK_POINT1)
    {
        POINT1_TM->start(2000);

    }
    else if(*(temp+1)==VISION_CHECK_POINT2)
    {
        POINT2_TM->start(2000);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=STAGE_WRITE;
    temp[2]=STAGE_POINT1_MOVE;
    VISION_MOTOR->write(temp,4);
    TOP_VIEW_IMAGE_CH=OFF;
    ui->VISION_RX->setText("GO MOVE1~~~~");

}

void MainWindow::on_pushButton_5_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=STAGE_WRITE;
    temp[2]=STAGE_POINT2_MOVE;
    VISION_MOTOR->write(temp,4);
    TOP_VIEW_IMAGE_CH=OFF;
}

void MainWindow::on_pushButton_6_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=VISION_ORIGIN;
    temp[2]=0;
    VISION_MOTOR->write(temp,4);
    TOP_VIEW_IMAGE_CH=OFF;

}

void MainWindow::on_pushButton_7_clicked()
{


}

void MainWindow::on_pushButton_8_clicked()
{
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=STAGE_WRITE;
    temp[2]=STAGE_WAIT_MOVE;
    VISION_MOTOR->write(temp,4);
    TOP_VIEW_IMAGE_CH=OFF;
    //on_CAM_CLOSE_clicked();
    //
    if(ui->checkBox->isChecked())
    {
        time_loop(4500);
        update_bt_sig();
    }
}

void MainWindow::update_bt_sig()
{
    //on_CAM_OPEN_clicked();
    on_pushButton_4_clicked();

}

void MainWindow::on_LIGHT_BT_clicked()
{
    LED_CONTROL *LC = new LED_CONTROL();

    LC->setModal(true);
    // LC->setAttribute(Qt::WA_DeleteOnClose);

    LC->show();
}
void MainWindow::TOP_STAGE()
{
    QScrollBar *sb = ui->STAGE_TEXT->verticalScrollBar();
    //QTime _nowTime = QTime::currentTime();
    //qDebug()<<_nowTime;
    cap_T >> frame_T;
    if(frame_T.empty())
    {
	    qDebug()<<"FRAME ERROR TOP";
        timer->stop();
        on_CAM_CLOSE_clicked();
        OPEN_DELAY->start(300);
        frame_T=cap_T_temp;

        return;
    }
    else 
    {
        cap_T_temp=frame_T;
    }
    // flip(frame_T,frame_T,0);

    if(TOP_VIEW_IMAGE_CH==ON)
    {


        cvtColor(frame_T,frame_T,CV_BGR2RGB);
        ui->STAGE_TEXT->clear();

        Mat frame_T_Binary;
        cvtColor(frame_T, frame_T_Binary, CV_RGB2GRAY);
        GaussianBlur(frame_T_Binary, frame_T_Binary, Size(31,31),0);
        threshold(frame_T_Binary,frame_T_Binary,245,255,THRESH_BINARY|THRESH_OTSU);
        frame_T_Binary=~frame_T_Binary;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        Mat Canny_T;
        Canny(frame_T_Binary, Canny_T, 245, 255,3);
        Mat result(Canny_T.size(), CV_8U, cv::Scalar(0));
        findContours( Canny_T, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        sb->setValue(sb->maximum());
        vector<int> sortIdx(contours.size());
        vector<float> areas(contours.size());
        for( int n = 0; n < (int)contours.size(); n++ ) {
            sortIdx[n] = n;
            areas[n] = contourArea(contours[n], false);
        }
        std::sort( sortIdx.begin(), sortIdx.end(), AreaCmp(areas));

        if(((contours.size()>0 && contourArea(contours[sortIdx[0]],false)>20000)))
        {


            cv::drawContours(result, contours,sortIdx[0],cv::Scalar(255),2);    // 두께를 2로
            vector<Moments> mu(contours.size() );

            for(unsigned int i = 0; i < contours.size(); i++ )
            { mu[i] = moments( contours[i], false ); }
            vector<Point2f> mc( contours.size() );
            for( size_t i = 0; i < contours.size(); i++ )
            {
                mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                                 static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
            }
            Mat drawing(Canny_T.size(), CV_8U, cv::Scalar(0));


            Mat Contours_color;
            cvtColor(drawing,Contours_color,CV_GRAY2BGR);
            drawContours( Contours_color, contours,sortIdx[0],cv::Scalar(0,0,255),1);
            circle( Contours_color, mc[sortIdx[0]], 4, cv::Scalar(0,255,0),3, 1, 0 );
            if(POINT_STATE==GOAL_STAGE_POINT1)
            {
                TOP_POINT1_Y=mc[sortIdx[0]].y;
                char temp[4]={0,};
                temp[0]='<';
                temp[3]='>';
                temp[1]=STAGE_WRITE;
                temp[2]=STAGE_POINT2_MOVE;
                VISION_MOTOR->write(temp,4);
                TOP_VIEW_IMAGE_CH=OFF;
            }
            else if(POINT_STATE==GOAL_STAGE_POINT2)
            {
                TOP_POINT2_Y=mc[sortIdx[0]].y;

                if(fabs(TOP_POINT1_Y-TOP_POINT2_Y)<=10)
                {
                    QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\S_GREEN.jpg");
                    switch(STAGE_NUM_TEMP)
                    {
                    case 1:
                        ui->S1_LED->setPixmap(im);
                        break;
                    case 2:
                        ui->S2_LED->setPixmap(im);
                        break;
                    case 3:
                        ui->S3_LED->setPixmap(im);
                        break;
                    case 4:
                        ui->S4_LED->setPixmap(im);
                        break;

                    }

                    char temp[4]={0,};
                    temp[0]='<';
                    temp[3]='>';
                    temp[1]=STAGE_WRITE;
                    temp[2]=STAGE_WAIT_MOVE;
                    VISION_MOTOR->write(temp,4);
                    TOP_VIEW_IMAGE_CH=OFF;
                    POINT_STATE=0;

                    if(TOP_CHECK_CNT<4) TOP_CHECK_CNT++;
                }
            }
            //char str_temp[20]={0,};
            //  sprintf(str_temp,"%d",fabs(TOP_POINT1_Y-TOP_POINT2_Y));
            //  putText(Contours_color,str_temp,Point(50,80),0,2,Scalar(255,255,255));
            qt_image_T = QImage(static_cast<const unsigned char*>(Contours_color.data), Contours_color.cols, Contours_color.rows, QImage::Format_RGB888);
            ui->TOP_VISION->setPixmap(QPixmap::fromImage(qt_image_T));
            ui->TOP_VISION->resize(ui->TOP_VISION->pixmap()->size());

            ui->STAGE_TEXT->setText(QString::number(TOP_POINT1_Y-TOP_POINT2_Y));

            TOP_STAGE_MSG="";
        }
        else
        {
            line(frame_T,Point(0,360),Point(1280,360),Scalar(255,0,0),2);
            qt_image_T = QImage(static_cast<const unsigned char*>(frame_T.data), frame_T.cols, frame_T.rows, QImage::Format_RGB888);
            ui->TOP_VISION->setPixmap(QPixmap::fromImage(qt_image_T));
            ui->TOP_VISION->resize(ui->TOP_VISION->pixmap()->size());
        }
    }
    else
    {
        line(frame_T,Point(0,360),Point(1280,360),Scalar(255,0,0),2);
        qt_image_T = QImage(static_cast<const unsigned char*>(frame_T.data), frame_T.cols, frame_T.rows, QImage::Format_RGB888);

        ui->TOP_VISION->setPixmap(QPixmap::fromImage(qt_image_T));
        ui->TOP_VISION->resize(ui->TOP_VISION->pixmap()->size());
    }
}
void MainWindow::STATUS_SEND()
{

}
void MainWindow::RAMI_SEND(uint8_t return_sel)
{
    char temp[10]={'<',1,1,1,1,1,1,1,1,'>'};
    temp[0]='<';
    temp[9]='>';
    temp[1]=return_sel;
    temp[2]=BOTTOM_SIG;
    temp[3]=BOTTOM_ST_SIG;
    BOTTOM_SIG=BOTTOM_IDLE;
    BOTTOM_ST_SIG=STOP;
    RAMI_EQ->write(temp,10);

    qDebug()<<"RAMI SEND~~";
}
void MainWindow::TOWER_SEND()
{
}
void MainWindow::on_SNAP_BT_clicked()
{

    QString current_time=QTime::currentTime().toString("hh-mm-ss");
    QDate *date = new QDate();
    QDate curDate = date->currentDate();   // 시스템에서 현재 날짜 가져오기
    QString  date_string = curDate.toString(Qt::ISODate);



    std::string cd = date_string.toUtf8().constData();

    qDebug()<< current_time;
    if(VIEW_MODE_CHANGE==true)
    {
        Mat LAST_TEMP = frame_T;
        cvtColor(LAST_TEMP,LAST_TEMP,CV_BGR2RGB);
        QString  Original_Path = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\ORIGINAL\\TOP\\"+current_time+".jpg";
        imwrite(Original_Path.toStdString(), LAST_TEMP);
        QString  Vision_Path = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\VISION\\TOP\\"+current_time+".jpg";
        imwrite(Vision_Path.toStdString(), Last_img);
    }
    else
    {
        Mat LAST_TEMP = frame;
        cvtColor(LAST_TEMP,LAST_TEMP,CV_BGR2RGB);
        QString  Original_Path = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\ORIGINAL\\BOTTOM\\"+current_time+".jpg";
        imwrite(Original_Path.toStdString(), LAST_TEMP);
        QString  Vision_Path = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_SCREENSHOT\\"+date_string+"\\VISION\\BOTTOM\\"+current_time+".jpg";
        imwrite(Vision_Path.toStdString(), Last_img);
    }

    cout << "Save image" << endl;
}

void MainWindow::on_pushButton_13_clicked()
{
	on_pushButton_9_clicked();
}


void MainWindow::on_pushButton_9_clicked()
{
    if(VIEW_MODE_CHANGE)
        VIEW_MODE_CHANGE=false;
    else
        VIEW_MODE_CHANGE=true;
    VIEW_SETTING(VIEW_MODE_CHANGE);

}

void MainWindow::on_PLAY_BT_clicked()
{
    /* if(VIEW_MODE_CHANGE==true)
        VIEW_MODE_CHANGE=false;
    else {
        VIEW_MODE_CHANGE=true;
    }
    VIEW_SETTING(VIEW_MODE_CHANGE);
  */

    play->setModal(true);
    //  play->setAttribute(Qt::WA_DeleteOnClose);
    play->show();

}

void MainWindow::VIEW_SETTING(bool SEL)
{

    if(ui->SET_BT->isEnabled()==false)
    {
        ui->SET_BT->setEnabled(true);
    }

    if(SEL==true)
    {
        ui->TOP_VISION->setVisible(true);
        ui->BOTTOM_VIDEO->setVisible(false);
        ui->BOTTOM_VIDEO_r->setVisible(false);
        OPEN_CAM_TOP();
        ui->PASSNG->setVisible(false);
        ui->PASSNG_r->setVisible(false);
        TOP_STAGE_MSG+="";
        ui->STAGE_TEXT->setText(TOP_STAGE_MSG);
        QScrollBar *sb = ui->STAGE_TEXT->verticalScrollBar();
        sb->setValue(sb->minimum());
        BOTTOM_STAGE_FLAG=0;

    }
    else
    {
        ui->TOP_VISION->setVisible(false);
        ui->BOTTOM_VIDEO->setVisible(true);
         ui->BOTTOM_VIDEO_r->setVisible(true);
        OPEN_CAM_BOTTOM();
        // ui->STAGE_TEXT->setVisible(false);
        ui->PASSNG->setVisible(true);
        ui->PASSNG_r->setVisible(true);
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    // imwrite("C:\\Users\\Synopex ESD\\Pictures\\false_1.jpg",Last_img);
    /*QString DIR_TEMP_LOG = "C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_DATA\\"+play->LOGO_DIR+"\\"+QString::number(LOT_COUNT+1)+"\\"+play->LOGO_DIR+"_"+QString::number(LOT_COUNT+1)+".csv";
    QString DIR_TEMP_ORIGIN_IMG="C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_DATA\\"+play->LOGO_DIR+"\\"+QString::number(LOT_COUNT+1)+"\\Origin.jpg";
    QString DIR_TEMP_VISION_IMG="C:\\Users\\Synopex ESD\\Documents\\RG3_VISION_DATA\\"+play->LOGO_DIR+"\\"+QString::number(LOT_COUNT+1)+"\\Vision.jpg";
    QFile file(DIR_TEMP_LOG);
    if (file.open(QFile::WriteOnly|QFile::Truncate))
    {
    QTextStream stream(&file);
    stream <<"Lot Number,"<<"Lot Count,"<<"X err,"<<"Y err"<<"\n"<<play->LOGO_DIR<<","<<QString::number(LOT_COUNT+1)<<","<<QString::number(BOTTOM_X_TEMP)<<"um,"<<QString::number(BOTTOM_Y_TEMP)<<"um";
    file.close();
    }
    imwrite(DIR_TEMP_ORIGIN_IMG.toStdString(),frame);
    imwrite(DIR_TEMP_VISION_IMG.toStdString(),Last_img);
*/






}
void MainWindow::LOT_NUM_UPDATE()
{

    ui->LOT_CNT_TEXT->setText(" "+QString::number(LOT_COUNT));
    ui->LOT_NUM_LINE->setText(play->LOT_TEMP);


    if(TOP_CHECK_CNT!=0)
    {
        SKIP_EN=false;

        QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\S_GREEN.jpg");
        ui->S1_LED->setPixmap(im);

        QPixmap im1("C:\\Users\\Synopex ESD\\Pictures\\S_GREEN.jpg");
        ui->S2_LED->setPixmap(im);

        QPixmap im2("C:\\Users\\Synopex ESD\\Pictures\\S_GREEN.jpg");
        ui->S3_LED->setPixmap(im);

        QPixmap im3("C:\\Users\\Synopex ESD\\Pictures\\S_GREEN.jpg");
        ui->S4_LED->setPixmap(im);


    }
    else {
        SKIP_EN=true;
        QPixmap im("C:\\Users\\Synopex ESD\\Pictures\\S_YELLOW.jpg");
        ui->S1_LED->setPixmap(im);

        QPixmap im1("C:\\Users\\Synopex ESD\\Pictures\\S_YELLOW.jpg");
        ui->S2_LED->setPixmap(im);

        QPixmap im2("C:\\Users\\Synopex ESD\\Pictures\\S_YELLOW.jpg");
        ui->S3_LED->setPixmap(im);

        QPixmap im3("C:\\Users\\Synopex ESD\\Pictures\\S_YELLOW.jpg");
        ui->S4_LED->setPixmap(im);


    }
    //client->connect2host();
}

void MainWindow::on_SAVE_BT_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("C:\\Tilt_Check"));
}

void MainWindow::SAVE_IMAGE()
{
    QString DIR_TEMP_ORIGIN_IMG="C:\\RG3_VISION_DATA\\"+play->LOGO_DIR+"\\"+QString::number(LOT_COUNT)+"\\Origin.jpg";
    QString DIR_TEMP_VISION_IMG="C:\\RG3_VISION_DATA\\"+play->LOGO_DIR+"\\"+QString::number(LOT_COUNT)+"\\Vision.jpg";

    imwrite(DIR_TEMP_ORIGIN_IMG.toStdString(),frame);
    imwrite(DIR_TEMP_VISION_IMG.toStdString(),Last_img);
}

void MainWindow::on_pushButton_11_clicked()
{
    STAGE_NUM_TEMP++;
    if(STAGE_NUM_TEMP)
        STAGE_NUM_TEMP=1;

    ui->VQLAMI_RX->setText(QString::number(STAGE_NUM_TEMP));
    if(STAGE_NUM_TEMP==3)
    time_loop(1000);
    on_LED2_ON_clicked();
    on_LED1_ON_clicked();
    on_LED3_ON_clicked();
    ui->STAGE_NUMBER->setText("STAGE "+QString::number(STAGE_NUM_TEMP));
    if(BOTTOM_STAGE_FLAG==ON) BOTTOM_STAGE_FLAG=OFF;
    if(TOP_CHECK_CNT<4)
    {
        if(VIEW_MODE_CHANGE==false){
            VIEW_MODE_CHANGE=true;
            VIEW_SETTING(VIEW_MODE_CHANGE);
        }

    }
    if(BOTTOM_SEND_EN==false) BOTTOM_SEND_EN=true;

    qDebug()<<"STAGE_CHANGE";
}


void MainWindow::on_SET_BT_clicked()
{
    tilt_check=true;
    VIEW_MODE_CHANGE_TEMP=VIEW_MODE_CHANGE;
    VIEW_MODE_CHANGE=true;
    VIEW_SETTING(VIEW_MODE_CHANGE);
    //time_loop(1000);
    Vision_check *VC = new Vision_check;
    VC->setModal(true);
    VC->setAttribute(Qt::WA_DeleteOnClose);
    VC->show();

}
Mat MainWindow::RotateImage(Mat img, double angle, double x, double y)
{
    Mat matRotation = getRotationMatrix2D(  Point( x, y ),angle, 1);
    Mat imgRotated;
    warpAffine( img, img, matRotation, img.size(), INTER_LINEAR, 0, Scalar() );

    return img;
}

void MainWindow::STAGE_CHECK()
{
    vc_t->run();
    if(first_re_data==true)
        LOT_COUNT++;
    ui->LOT_CNT_TEXT->setText(" "+QString::number(LOT_COUNT));
    ui->LOT_CNT_TEXT->setText(QString::number(LOT_COUNT));
    QString logPath = "C:\\RG3_VISION_DATA\\"+play->LOT_TEMP;
    QString ini_temp = logPath+"\\"+play->LOGO_DIR+".ini";
    QSettings* settings = new QSettings(ini_temp, QSettings::IniFormat);
    settings->setValue("LOT_COUNT",QString::number(LOT_COUNT));
    settings->sync();

    if(first_re_data==false)
        first_re_data=true;
}


void MainWindow::POINT1_CHECK()
{
    Goal_Point1_check=true;
    Goal_Point2_check=false;
    POINT1_TM->stop();
}

void MainWindow::POINT2_CHECK()
{
    Goal_Point1_check=false;
    Goal_Point2_check=true;
    POINT2_TM->stop();
}

void MainWindow::TOP_CHECK()
{

    TOP_VIEW_IMAGE_CH=ON;
    if(POINT_STATE==0)
    {
        POINT_STATE= GOAL_STAGE_POINT1;
    }

    else if(POINT_STATE==GOAL_STAGE_POINT1)
    {
        POINT_STATE=GOAL_STAGE_POINT2;
    }

    TOP_CHECK_TM->stop();
}


void MainWindow::on_checkBox_2_clicked()
{
    /*if(ui->checkBox_2->isChecked())
    {
        top_check_cnt=3;
    }
    else
    {
        top_check_cnt=99999;
    }*/
}

void MainWindow::OPEN_DELAY_FUNC()
{
    on_pushButton_connect_clicked();
    if(VIEW_MODE_CHANGE==false)
        OPEN_CAM_BOTTOM();
    else
        OPEN_CAM_TOP();

    OPEN_DELAY->stop();
}
void MainWindow::setStatus(bool newStatus)
{
    if(newStatus)
    {
        ui->label_status->setText(
                    tr("<font color=\"green\">CONNECTED</font>"));
        ui->pushButton_connect->setVisible(false);
        ui->pushButton_disconnect->setVisible(true);
    }
    else
    {
        ui->label_status->setText(
                    tr("<font color=\"red\">DISCONNECTED</font>"));
        ui->pushButton_connect->setVisible(true);
        ui->pushButton_disconnect->setVisible(false);
        if(RECONNECTION->isActive()==false)
        {
              RECONNECTION->start(5000);
        }
    }
}
int cnt=0;
void MainWindow::receivedSomething(QString msg)
{
    ui->textEdit_log->append(msg);
    if(msg=="Reply: connection established")
    {
        qDebug()<<"comin";

        RECONNECTION->stop();

        if(tcp_send_en==false) tcp_send_en=true;
        RAMI_SEND(RAMI_STATUS);

    }
}

void MainWindow::gotError(QAbstractSocket::SocketError err)
{
    //qDebug() << "got error";
    QString strError = "unknown";
    switch (err)
    {
    case 0:
        strError = "Connection was refused";
        break;
    case 1:
        strError = "Remote host closed the connection";
        break;
    case 2:
        strError = "Host address was not found";
        break;
    case 5:
        strError = "Connection timed out";
        break;
    default:
        strError = "Unknown error";
    }

    ui->textEdit_log->append(strError);

    if(tcp_send_en==true) tcp_send_en=false;
}

void MainWindow::on_pushButton_connect_clicked()
{
    client->connect2host();
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    client->closeConnection();
}

void MainWindow::on_pushButton_send_clicked()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << ui->sendtext->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}
void MainWindow::server_reconnection()
{
    client->connect2host();
}
void MainWindow::tcp_send()
{
    QString LOT_Number_Client, STAGE_NUM_Client, LOT_CNT_Client;

    LOT_Number_Client=play->LOT_TEMP;
    STAGE_NUM_Client=QString::number(STAGE_NUM_TEMP);
    LOT_CNT_Client = QString::number(LOT_COUNT);

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << "LAMI:"+PUMP_STATUS_Client+":"+LOT_Number_Client+":"+STAGE_NUM_Client+":"+LOT_CNT_Client+":"+LED_TOWER_Client+":"+PRESS_STATUS_Client;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);

    qDebug()<<LED_TOWER_Client;
}

void MainWindow::tcp_send_start(QString lot)
{

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << "LAMI_S:"+lot;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}
void MainWindow::tcp_send_end(QString lot)
{

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << "LAMI_E:"+lot;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    client->tcpSocket->write(arrBlock);
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{

    if(key_input_timeout_main->isActive()==false)
        key_input_timeout_main->start(200);

    if((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)  ||(event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) ||event->key()==Qt::Key_Space||
            event->key()==Qt::Key_Slash|| event->key()==Qt::Key_ParenLeft || event->key()==Qt::Key_ParenRight )
    {
        switch(event->key())
        {
        case Qt::Key_0: Key_input_main+='0'; break;
        case Qt::Key_1: Key_input_main+='1'; break;
        case Qt::Key_2: Key_input_main+='2'; break;
        case Qt::Key_3: Key_input_main+='3'; break;
        case Qt::Key_4: Key_input_main+='4'; break;
        case Qt::Key_5: Key_input_main+='5'; break;
        case Qt::Key_6: Key_input_main+='6'; break;
        case Qt::Key_7: Key_input_main+='7'; break;
        case Qt::Key_8: Key_input_main+='8'; break;
        case Qt::Key_9: Key_input_main+='9'; break;
        case Qt::Key_A: Key_input_main+='A'; break;
        case Qt::Key_B: Key_input_main+='B'; break;
        case Qt::Key_C: Key_input_main+='C'; break;
        case Qt::Key_D: Key_input_main+='D'; break;
        case Qt::Key_E: Key_input_main+='E'; break;
        case Qt::Key_F: Key_input_main+='F'; break;
        case Qt::Key_G: Key_input_main+='G'; break;
        case Qt::Key_H: Key_input_main+='H'; break;
        case Qt::Key_I: Key_input_main+='I'; break;
        case Qt::Key_J: Key_input_main+='J'; break;
        case Qt::Key_K: Key_input_main+='K'; break;
        case Qt::Key_L: Key_input_main+='L'; break;
        case Qt::Key_M: Key_input_main+='M'; break;
        case Qt::Key_N: Key_input_main+='N'; break;
        case Qt::Key_O: Key_input_main+='O'; break;
        case Qt::Key_P: Key_input_main+='P'; break;
        case Qt::Key_Q: Key_input_main+='Q'; break;
        case Qt::Key_R: Key_input_main+='R'; break;
        case Qt::Key_S: Key_input_main+='S'; break;
        case Qt::Key_T: Key_input_main+='T'; break;
        case Qt::Key_U: Key_input_main+='U'; break;
        case Qt::Key_V: Key_input_main+='V'; break;
        case Qt::Key_W: Key_input_main+='W'; break;
        case Qt::Key_X: Key_input_main+='X'; break;
        case Qt::Key_Y: Key_input_main+='Y'; break;
        case Qt::Key_Z: Key_input_main+='Z'; break;
        case Qt::Key_Space: Key_input_main+=' '; break;
        case Qt::Key_Slash: Key_input_main+='/'; break;
        case Qt::Key_ParenLeft : Key_input_main+='('; break;
        case Qt::Key_ParenRight : Key_input_main+=')'; break;
        }

        if(Key_input_main.length()==14)
        {
            if(Key_input_main==play->Key_input_temp)
            {
                tcp_send_end(play->Key_input_temp);
                key_input_timeout_main->stop();
                QString LOGO_TEMP = play->Key_input_temp;
                play->Key_input_temp="";
                Key_input_main="";
                LOT_COUNT=0;
                LOT_COUNT_MAX=0;
                ALL_ENABLE=false;
                emit LOT_END();
                play->setModal(true);
                play->show();
                SKIP_EN=true;
                //on_pushButton_disconnect_clicked();
                QMessageBox::information(this,tr("알람"), LOGO_TEMP +" 작업이 종료 되었습니다.");
            }
        }
    }
    if(event->key()==Qt::Key_X)
    {
        on_SET_BT_clicked();
        qDebug()<<"XCLICK";
    }
   // qDebug()<<play->Key_input;
   // qDebug()<<Key_input_main;
}
void MainWindow::keyin_timeout_main()
{
    Key_input_main="";
    key_input_timeout_main->stop();
}



void MainWindow::on_LED3_OPEN_clicked()
{
    if(!port3->open(QIODevice::ReadWrite))
    {

        QMessageBox OPENERR;
        OPENERR.setText("OPEN ERROR LED3");
        OPENERR.exec();
    }
    else
    {
        qDebug()<<"OPEN S";
        ui->LED3_OPEN->setEnabled(false);
        ui->LED3_CLOSE->setEnabled(true);
        char temp[4]={0,};
        temp[0]='<';
        temp[3]='>';
        temp[1]=LED_CON_CHECK;
        port3->write(temp,4);

    }
}

void MainWindow::on_LED3_CLOSE_clicked()
{
    port3->close();
    ui->LED3_OPEN->setEnabled(true);
    ui->LED3_CLOSE->setEnabled(false);
    on_LED3_OFF_clicked();
    ui->LED3_ST->setText("DISCONNECT");
}




void MainWindow::on_comboBox_6_currentIndexChanged(const QString &arg1)
{

      //  port3->setPortName(arg1);

}



void MainWindow::on_angle_tune_l_stage1_valueChanged(double arg1)
{
    stage_angle_l_stage1=arg1;
}

void MainWindow::on_angle_tune_l_stage2_valueChanged(double arg1)
{
    stage_angle_l_stage2=arg1;
}

void MainWindow::on_angle_tune_l_stage3_valueChanged(double arg1)
{
    stage_angle_l_stage3=arg1;
}

void MainWindow::on_angle_tune_l_stage4_valueChanged(double arg1)
{
    stage_angle_l_stage4=arg1;
}

void MainWindow::on_angle_tune_r_1_valueChanged(double arg1)
{
    stage_angle_r_stage1=arg1;
}

void MainWindow::on_angle_tune_r_2_valueChanged(double arg1)
{
    stage_angle_r_stage2=arg1;
}

void MainWindow::on_angle_tune_r_3_valueChanged(double arg1)
{
   stage_angle_r_stage3=arg1;
}

void MainWindow::on_angle_tune_r_4_valueChanged(double arg1)
{
    stage_angle_r_stage4=arg1;
}
