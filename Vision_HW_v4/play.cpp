#include "play.h"
#include "ui_play.h"

bool ALL_ENABLE;
int LOT_COUNT_MAX;
extern int LOT_COUNT;
extern MainWindow *Main;

PLAY *p;

PLAY::PLAY(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PLAY)
{
    ui->setupUi(this);
    key_input_timeout = new QTimer(this);
    connect(key_input_timeout, SIGNAL(timeout()),SLOT(keyin_timeout()));
    p=this;

    ALL_ENABLE=false;

    QDate *date = new QDate();
    QDate curDate = date->currentDate();   // 시스템에서 현재 날짜 가져오기
    QString date_string = curDate.toString(Qt::ISODate);

    QStringList temp = date_string.split('-');
    DATE_TEMP = temp[0]+temp[1]+temp[2];
    NO_BT=true;
}


PLAY::~PLAY()
{
    delete ui;
}

void PLAY::keyin_timeout()
{
    Key_input="";
    key_input_timeout->stop();
}
void PLAY::OK()
{
    Main->LOT_COUNT=0;
    NO_BT=false;
    LOT_TEMP = ui->LOT_LINE->text();
    LOGO_DIR = LOT_TEMP;
  //  QMessageBox::information(this,"CHECK", "인증 완료");

    ALL_ENABLE=true;



    QString logPath = "C:\\RG3_VISION_DATA\\"+LOT_TEMP;
    QDir dir(logPath);
    dir.mkpath(logPath);

    for(int i=0; i< 360; i++)
    {
        QString logPath = "C:\\RG3_VISION_DATA\\"+LOT_TEMP+"\\"+QString::number(i+1);
        QDir dir(logPath);
        dir.mkpath(logPath);
    }
    QString ALL_DIR_String = logPath+"\\All_Data";
    QDir ALLDIR(ALL_DIR_String);
    ALLDIR.mkpath(ALL_DIR_String);



    QString ini_temp = logPath+"\\"+LOGO_DIR+".ini";

    QSettings* settings = new QSettings(ini_temp, QSettings::IniFormat);

    bool dstFilexist=QFile::exists(ini_temp);

    if(dstFilexist==true)
    {
        settings->setValue("LOT_COUNT","0");
        Main->VIEW_MODE_CHANGE=false;
        Main->VIEW_SETTING(Main->VIEW_MODE_CHANGE);

        bool dstFilexist_all=QFile::exists(ALL_DIR_String+"\\All_Data.csv");


        if(dstFilexist_all==false){
            QFile file(ALL_DIR_String+"\\All_Data.csv");
            if (file.open(QFile::WriteOnly|QFile::Truncate))
            {
                QTextStream stream(&file);
                stream <<"Lot Number,"<<"Lot Count,"<<"X err,"<<"Y err"<<"\n";
                file.close();
            }
        }

    }
    settings->sync();


    if(NO_BT==true)
    {
        bool dstFilexist_all=QFile::exists(ALL_DIR_String+"\\All_Data.csv");


        if(dstFilexist_all==false){
            QFile file(ALL_DIR_String+"\\All_Data.csv");
            if (file.open(QFile::WriteOnly|QFile::Truncate))
            {
                QTextStream stream(&file);
                stream <<"Lot Number,"<<"Lot Count,"<<"X err,"<<"Y err"<<"\n";
                file.close();
            }
        }
    }
    emit CHECK_END();
    close();
}
void PLAY::LOT_END_SIG()
{
    //LOT_NUM++;
}

void PLAY::keyPressEvent(QKeyEvent *event)
{

    if(key_input_timeout->isActive()==false)
    {
        key_input_timeout->start(200);
    }

#if 1
     Key_input="SU202000001001";
     qDebug()<<Key_input;
     ui->LOT_LINE->setText(Key_input);
     key_input_timeout->stop();
     Key_input_temp=Key_input;
     Key_input="";
      Main->tcp_send_start(Key_input_temp);
     Main-> RAMI_SEND(RAMI_STATUS);
     OK();
#else

    if((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)  ||(event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) ||event->key()==Qt::Key_Space||
            event->key()==Qt::Key_Slash|| event->key()==Qt::Key_ParenLeft || event->key()==Qt::Key_ParenRight )
    {
        switch(event->key())
        {
        case Qt::Key_0: Key_input+='0'; break;
        case Qt::Key_1: Key_input+='1'; break;
        case Qt::Key_2: Key_input+='2'; break;
        case Qt::Key_3: Key_input+='3'; break;
        case Qt::Key_4: Key_input+='4'; break;
        case Qt::Key_5: Key_input+='5'; break;
        case Qt::Key_6: Key_input+='6'; break;
        case Qt::Key_7: Key_input+='7'; break;
        case Qt::Key_8: Key_input+='8'; break;
        case Qt::Key_9: Key_input+='9'; break;
        case Qt::Key_A: Key_input+='A'; break;
        case Qt::Key_B: Key_input+='B'; break;
        case Qt::Key_C: Key_input+='C'; break;
        case Qt::Key_D: Key_input+='D'; break;
        case Qt::Key_E: Key_input+='E'; break;
        case Qt::Key_F: Key_input+='F'; break;
        case Qt::Key_G: Key_input+='G'; break;
        case Qt::Key_H: Key_input+='H'; break;
        case Qt::Key_I: Key_input+='I'; break;
        case Qt::Key_J: Key_input+='J'; break;
        case Qt::Key_K: Key_input+='K'; break;
        case Qt::Key_L: Key_input+='L'; break;
        case Qt::Key_M: Key_input+='M'; break;
        case Qt::Key_N: Key_input+='N'; break;
        case Qt::Key_O: Key_input+='O'; break;
        case Qt::Key_P: Key_input+='P'; break;
        case Qt::Key_Q: Key_input+='Q'; break;
        case Qt::Key_R: Key_input+='R'; break;
        case Qt::Key_S: Key_input+='S'; break;
        case Qt::Key_T: Key_input+='T'; break;
        case Qt::Key_U: Key_input+='U'; break;
        case Qt::Key_V: Key_input+='V'; break;
        case Qt::Key_W: Key_input+='W'; break;
        case Qt::Key_X: Key_input+='X'; break;
        case Qt::Key_Y: Key_input+='Y'; break;
        case Qt::Key_Z: Key_input+='Z'; break;
        case Qt::Key_Space: Key_input+=' '; break;
        case Qt::Key_Slash: Key_input+='/'; break;
        case Qt::Key_ParenLeft : Key_input+='('; break;
        case Qt::Key_ParenRight : Key_input+=')'; break;
        }

        // jk1 SU202011241001-SUAB241
        //if(Key_input.length()==1)
        if(Key_input.length()==14)
        {
           if(Key_input.at(0)=='S' &&Key_input.at(1)=='U'){
               // Key_input="SU202000001001";
                qDebug()<<Key_input;
                ui->LOT_LINE->setText(Key_input);
                key_input_timeout->stop();
                Key_input_temp=Key_input;
                Key_input="";
                 Main->tcp_send_start(Key_input_temp);
                Main-> RAMI_SEND(RAMI_STATUS);
                OK();
            }
           else {
                QMessageBox::information(this,tr("JOB ID 확인"),tr("JOB ID를 확인하세요"));
           }
        }
    }
    else if(event->key()==Qt::Key_Return)
    {

    }
#endif
    qDebug("keyPressEvent(%x)", event->key());
}
