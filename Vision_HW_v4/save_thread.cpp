#include "save_thread.h"

extern MainWindow *Main;
extern PLAY *p;

SAVE_Thread *save_t;

SAVE_Thread::SAVE_Thread()
{
    save_t = this;
}


void SAVE_Thread::run()
{
    QString DIR_TEMP_LOG = "C:\\RG3_VISION_DATA\\"+p->LOGO_DIR+"\\"+QString::number(Main->LOT_COUNT)+"\\"+p->LOGO_DIR+"_"+QString::number(Main->LOT_COUNT)+".csv";

    QFile file(DIR_TEMP_LOG);
    if (file.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream stream(&file);
        stream <<"Lot Number,"<<"Lot Count,"<<"X err,"<<"Y err"<<"\n"<<p->LOGO_DIR<<","<<QString::number(Main->LOT_COUNT)<<","<<Main->BOTTOM_X_TEMP;
        file.close();
    }

   /* QFile file(DIR_TEMP_LOG);
    if (file.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream stream(&file);
        stream <<"Lot Number,"<<"Lot Count,"<<"X err,"<<"Y err"<<"\n"<<play->LOGO_DIR<<","<<QString::number(LOT_COUNT)<<","<<QString::number(BOTTOM_X_TEMP)<<"um,"<<QString::number(BOTTOM_Y_TEMP)<<"um";
        file.close();
    }
    */

    Main->SAVE_IMAGE();

    QString all_D;

    QFile allfile("C:\\RG3_VISION_DATA\\"+p->LOT_TEMP+"\\All_Data\\All_Data.csv");
    if ( !allfile.open(QFile::ReadOnly | QFile::Text) ) {
          qDebug() << "File not exists";
      } else {
          QTextStream in(&allfile);


        all_D=in.readAll();

          all_D+=p->LOGO_DIR+","+QString::number(Main->LOT_COUNT)+","+Main->BOTTOM_X_TEMP+"\n";
          allfile.close();
      }
    if ( !allfile.open(QFile::WriteOnly | QFile::Truncate) ) {
          qDebug() << "File not exists";
      } else {
          QTextStream in(&allfile);
          in<<all_D;
          allfile.close();
      }


}
