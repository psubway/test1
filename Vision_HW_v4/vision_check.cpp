#include "vision_check.h"
#include "ui_vision_check.h"
#include <QMessageBox>


extern MainWindow *Main;
extern PLAY *p;
struct V_AreaCmp {
    V_AreaCmp(const vector<float>& _areas) : areas(&_areas) {}
    bool operator()(int a, int b) const { return (*areas)[a] > (*areas)[b]; }
    const vector<float>* areas;
};

Vision_check::Vision_check(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Vision_check)
{
    ui->setupUi(this);

    Vision_timer = new QTimer(this);
    connect(Vision_timer, SIGNAL(timeout()), this, SLOT(View_Update()));

    Vision_timer->start(20);

    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setVisible(false);

    VISION_STEP=0;
    mvr1_cnt=0;
    mvr2_cnt=0;
    stage_cnt=1;
    //Main->timer->stop();


    QDate *date = new QDate();
    QDate curDate = date->currentDate();   // 시스템에서 현재 날짜 가져오기
    date_string = curDate.toString(Qt::ISODate);


    ui->stage_label->setText("STAGE"+QString::number(stage_cnt));
}

Vision_check::~Vision_check()
{
    qDebug()<<"IWILLBEBACK";
    Main->tilt_check=false;
    Vision_timer->stop();
    Main->timer->start(WIN_UPDATE_TIME);
    Main->r_timer->start(WIN_UPDATE_TIME);
    Main->VIEW_MODE_CHANGE=Main->VIEW_MODE_CHANGE_TEMP;
    Main->VIEW_SETTING(Main->VIEW_MODE_CHANGE);

    delete ui;


}

void Vision_check::View_Update()
{
    Mat VISION_CHECK_MAT;
    Mat VISION_B_IMG;
    Mat VISION_LAST;
    Main->cap_T >> VISION_CHECK_MAT;
    if(VISION_CHECK_MAT.empty()) {qDebug()<<"FRAME ERROR";
        return;  }
    cvtColor(VISION_CHECK_MAT, VISION_CHECK_MAT, CV_BGR2RGB);
    flip(VISION_CHECK_MAT,VISION_CHECK_MAT,0);
    cvtColor(VISION_CHECK_MAT, VISION_B_IMG, CV_BGR2GRAY);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat Blur;
    Mat Thresh;
    Mat VISION_CANNY;

    if(Main->Goal_Point1_check==true || Main->Goal_Point2_check==true)
    {
        GaussianBlur(VISION_B_IMG,Blur,Point(31,31),0);
        threshold(Blur,Thresh,0,255,THRESH_OTSU|THRESH_BINARY);
        Canny(Thresh, VISION_CANNY, 150, 250 ,3);
        cv::Mat result(VISION_CANNY.size(), CV_8U, cv::Scalar(0));
        findContours( VISION_CANNY, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        if(contours.size()>0)
        {
            vector<Moments> mu(contours.size() );
            vector<int> sortIdx(contours.size());
            vector<float> areas(contours.size());
            for( int n = 0; n < (int)contours.size(); n++ ) {
                sortIdx[n] = n;
                areas[n] = contourArea(contours[n], false);
            }
            std::sort( sortIdx.begin(), sortIdx.end(), V_AreaCmp(areas));

            for(unsigned int i = 0; i < contours.size(); i++ )

            { mu[i] = moments( contours[sortIdx[i]], false ); }

            vector<Point2f> mc( contours.size() );
            for( size_t i = 0; i < contours.size(); i++ )
            {
                mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                                 static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
            }




            Mat nonZeroCoordinates;
            float POINT1_X = 0;
            float POINT1_Y = 0;
            float POINT2_X = 9999;
            float POINT2_Y = 0;

            Mat drawing(VISION_CANNY.size(), CV_8U, cv::Scalar(0));

            //for(int i=0; i< contours.size(); i++)
            int moment_sel=0;

            if(Main->Goal_Point1_check==true)
            {
                for(int i=0; i<contours.size(); i++)
                {
                    if((mc[i].x>490 && mc[i].x<690))
                    {
                        moment_sel=i;
                        break;
                    }
                }


            }
            else if(Main->Goal_Point2_check==true)
            {
                for(int i=0; i<contours.size(); i++)
                {
                    if((mc[i].x>550 && mc[i].x<800))
                    {
                        moment_sel=i;
                        break;
                    }
                }

            }
            drawContours( drawing, contours,sortIdx[moment_sel],cv::Scalar(255),1);

            findNonZero(drawing, nonZeroCoordinates);

            for (int i = 0; i < nonZeroCoordinates.total(); i++ ) {
                if(POINT1_X<nonZeroCoordinates.at<Point>(i).x){
                    POINT1_X=nonZeroCoordinates.at<Point>(i).x;
                    POINT1_Y=nonZeroCoordinates.at<Point>(i).y;
                }
            }
            cvtColor(drawing,VISION_LAST,CV_GRAY2BGR);

            // TOP_CHECK_VIEW=QImage(static_cast<const unsigned char*>(VISION_CANNY.data), VISION_CANNY.cols, VISION_CANNY.rows, QImage::Format_Grayscale8);

            TOP_CHECK_VIEW=QImage(static_cast<const unsigned char*>(VISION_LAST.data), VISION_LAST.cols, VISION_LAST.rows, QImage::Format_RGB888);
            ui->Vision_View->setPixmap(QPixmap::fromImage(TOP_CHECK_VIEW));
            ui->Vision_View->resize(ui->Vision_View->pixmap()->size());



            if(Main->Goal_Point1_check==true)
            {
                findNonZero(drawing, nonZeroCoordinates);

                for (int i = 0; i < nonZeroCoordinates.total(); i++ ) {
                    if(POINT1_X<nonZeroCoordinates.at<Point>(i).x){
                        POINT1_X=nonZeroCoordinates.at<Point>(i).x;
                        POINT1_Y=nonZeroCoordinates.at<Point>(i).y;
                    }

                }
                circle(VISION_LAST,Point(POINT1_X,POINT1_Y),3,Scalar(255,255,255));
                POINT1_Y_VALUE=POINT1_Y;
                // mvr1_cnt++;
                // if(mvr1_cnt>10)
                // {
                Main->Goal_Point1_check=false;
                on_pushButton_2_clicked();
                //     mvr1_cnt=0;
                // }
                // qDebug()<<"Zzz_Z_Z_";
                //imwrite("C:\\Users\\Synopex ESD\\Pictures\\test1.jpg",VISION_CHECK_MAT);
            }
            else if(Main->Goal_Point2_check==true)
            {
                circle(VISION_LAST,Point(POINT1_X,POINT1_Y),3,Scalar(255,255,255));
                for (int i = 0; i < nonZeroCoordinates.total(); i++ ) {
                    if(POINT2_X>nonZeroCoordinates.at<Point>(i).x){
                        POINT2_X=nonZeroCoordinates.at<Point>(i).x;
                        POINT2_Y=nonZeroCoordinates.at<Point>(i).y;
                    }

                }
                POINT2_Y_VALUE=POINT2_Y;
                //  POINT2_Y_VALUE=V_MOVING_AVR2(mc[moment_sel].y);
                //   mvr2_cnt++;
                //   if(mvr2_cnt>10)
                //   {
                double tilt_value =((POINT1_Y_VALUE-POINT2_Y_VALUE)*13.1578)/2;
                QString tilt_string = QString::number(tilt_value);
                ui->VISION_TEXT->setText(tilt_string+"um");
               // imwrite("C:\\Users\\Synopex ESD\\Pictures\\test2.jpg",VISION_CHECK_MAT);


                if(tilt_value > 500 || tilt_value < -500 )
                {
                    QMessageBox::information(this,tr("경고"),"허용범위초과(±500um");
                }
                else if(ui->f_r->isChecked() ||ui->s_r->isChecked() || ui->t_r->isChecked())
                {
                        QString time = QDateTime::currentDateTime().toString(Qt::TextDate);
                        QStringList time_split = time.split(' ');
                        QString now_time = time_split[3];
                        QString save_time="";
                        save_time = date_string +" "+ now_time;
                        QString stage_number_string="";
                        if(ui->f_r->isChecked())
                        {
                            if(ui->stage_l->isChecked())
                            {
                                stage_number_string = "L_STAGE"+QString::number(stage_cnt);
                            }
                            else if(ui->stage_r->isChecked())
                            {
                                stage_number_string = "R_STAGE"+QString::number(stage_cnt);
                            }
                            tilt_save(p->Key_input_temp,"초물",save_time,stage_number_string,tilt_string);
                        }
                        else if(ui->s_r->isChecked())
                        {
                            if(ui->stage_l->isChecked())
                            {
                                stage_number_string = "L_STAGE"+QString::number(stage_cnt);
                            }
                            else if(ui->stage_r->isChecked())
                            {
                                stage_number_string = "R_STAGE"+QString::number(stage_cnt);
                            }
                            tilt_save(p->Key_input_temp,"중물",save_time,stage_number_string,tilt_string);
                        }
                        else if(ui->t_r->isChecked())
                        {
                            if(ui->stage_l->isChecked())
                            {
                                stage_number_string = "L_STAGE"+QString::number(stage_cnt);
                            }
                            else if(ui->stage_r->isChecked())
                            {
                                stage_number_string = "R_STAGE"+QString::number(stage_cnt);
                            }
                            tilt_save(p->Key_input_temp,"종물",save_time,stage_number_string,tilt_string);
                        }

                    if(stage_cnt<4) stage_cnt++;
                    else if(stage_cnt==4) stage_cnt=1;
                    ui->stage_label->setText("STAGE"+QString::number(stage_cnt));
                }
                Main->Goal_Point2_check=false;
                on_pushButton_3_clicked();
                //      mvr2_cnt=0;
                //  }
            }

        }
        else
        {
            TOP_CHECK_VIEW=QImage(static_cast<const unsigned char*>(VISION_CHECK_MAT.data), VISION_CHECK_MAT.cols, VISION_CHECK_MAT.rows, QImage::Format_RGB888);
            ui->Vision_View->setPixmap(QPixmap::fromImage(TOP_CHECK_VIEW));
            ui->Vision_View->resize(ui->Vision_View->pixmap()->size());
        }

    }
    else
    {
        TOP_CHECK_VIEW=QImage(static_cast<const unsigned char*>(VISION_CHECK_MAT.data), VISION_CHECK_MAT.cols, VISION_CHECK_MAT.rows, QImage::Format_RGB888);
        ui->Vision_View->setPixmap(QPixmap::fromImage(TOP_CHECK_VIEW));
        ui->Vision_View->resize(ui->Vision_View->pixmap()->size());
    }
}

void Vision_check::on_pushButton_clicked()
{
    Main->Goal_Point1_check=false;
    Main->Goal_Point2_check=false;
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=STAGE_WRITE;
    temp[2]=VISION_CHECK_POINT1_MOVE;
    Main->VISION_MOTOR->write(temp,4);
}

void Vision_check::on_pushButton_2_clicked()
{
    Main->Goal_Point1_check=false;
    Main->Goal_Point2_check=false;
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=STAGE_WRITE;
    temp[2]=VISION_CHECK_POINT2_MOVE;
    Main->VISION_MOTOR->write(temp,4);
}

void Vision_check::on_pushButton_3_clicked()
{
    Main->Goal_Point1_check=false;
    Main->Goal_Point2_check=false;
    char temp[4]={0,};
    temp[0]='<';
    temp[3]='>';
    temp[1]=STAGE_WRITE;
    temp[2]=GOAL_STAGE_POINT_WAIT;
    Main->VISION_MOTOR->write(temp,4);
}

void Vision_check::on_pushButton_4_clicked()
{
    on_pushButton_clicked();
}
static float vision_data_temp2[10];
static float vision_data_temp1[10];
float Vision_check::V_MOVING_AVR1(float input)
{

    static float average = 0;
    unsigned char i;
    average = average + (input - vision_data_temp1[0])/10;
    for (i = 0; i < 10 - 1; i++)
        vision_data_temp1[i] = vision_data_temp1[i+1];
    vision_data_temp1[i] = input;
    return average;
    //return input;
}

float Vision_check::V_MOVING_AVR2(float input)
{

    static float average = 0;
    unsigned char i;
    average = average + (input - vision_data_temp2[0])/10;
    for (i = 0; i < 10- 1; i++)
        vision_data_temp2[i] = vision_data_temp2[i+1];
    vision_data_temp2[i] = input;
    return average;

    //return input;
}
void Vision_check::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Z && (ui->f_r->isChecked()|| ui->s_r->isChecked() || ui->t_r->isChecked() || ui->i_r->isChecked()))
    {
        ui->VISION_TEXT->clear();
        on_pushButton_clicked();
    }
    else if(event->key() == Qt::Key_Z && ui->f_r->isChecked() && ui->s_r->isChecked() && ui->t_r->isChecked() && ui->i_r->isChecked())
    {
        QMessageBox::information(this,tr("경고"),"초,중,종물 중 하나를 선택하세요");
    }
    else if(event->key() == Qt::Key_Up )
    {
        on_stage_up_clicked();
    }
    else if(event->key() == Qt::Key_Down )
    {
       on_stage_down_clicked();
    }
    else if(event->key() == Qt::Key_1 )
    {
       ui->f_r->setChecked(true);
    }
    else if(event->key() == Qt::Key_2 )
    {
       ui->s_r->setChecked(true);
    }
    else if(event->key() == Qt::Key_3 )
    {
       ui->t_r->setChecked(true);
    }
    else if(event->key() == Qt::Key_4 )
    {
       ui->i_r->setChecked(true);
    }
    else if(event->key() == Qt::Key_Q)
    {
       ui->stage_l->setChecked(true);
    }
    else if(event->key() == Qt::Key_W )
    {
       ui->stage_r->setChecked(true);
    }
    else if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
    // qDebug("keyPressEvent(%x)", event->key());
}

void Vision_check::on_stage_down_clicked()
{
    if(stage_cnt>=2)
    {
        stage_cnt--;
        ui->stage_label->setText("STAGE"+QString::number(stage_cnt));
    }


}

void Vision_check::on_stage_up_clicked()
{
    if(stage_cnt<4)
    {
        stage_cnt++;
        ui->stage_label->setText("STAGE"+QString::number(stage_cnt));
    }

}
void Vision_check::tilt_save(QString LOT_NUM,QString MODE,QString DATE, QString STAGE, QString VALUE)
{

    QSqlDatabase db;


    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("C:\\Tilt_Check\\Tilt.db");
    if(!db.open())
    {
        qDebug()<<"problem opening database";
        return;
    }
    QSqlQuery qry;

    QString query = "CREATE TABLE LAMI_RESULT ("
                    "LOT_NUM TEXT,"
                    "DATE TEXT,"
                    "STAGE TEXT,"
                    "VALUE TEXT);";

    if(!qry.exec(query))
    {
        ////qDebug()<<"don't creating databse";
    }

    QSqlQuery search_qry("select * from LAMI_RESULT");
    QSqlRecord rec = search_qry.record();
    qDebug()<<rec.count();

    qry.prepare("INSERT INTO LAMI_RESULT ("
                "LOT_NUM,"
                "MODE,"
                "DATE,"
                "STAGE,"
                "VALUE)"
                "VALUES(?,?,?,?,?);");

    qry.addBindValue(LOT_NUM);
    qry.addBindValue(MODE);
    qry.addBindValue(DATE);
    qry.addBindValue(STAGE);
    qry.addBindValue(VALUE);

    if(!qry.exec())
    {
        ////qDebug()<< " adding value to fw_db";
    }

    db.close();
}
