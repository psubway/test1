#include "stage_change_thread.h"


extern MainWindow *Main;
extern PLAY *p;

STAGE_CHANGE_Thread *vc_t;

STAGE_CHANGE_Thread::STAGE_CHANGE_Thread()
{
    vc_t=this;
}
void STAGE_CHANGE_Thread::run()
{
    if(Main->STAGE_NUM_TEMP==3)

   Main->time_loop(1000);
   Main->on_LED2_ON_clicked();
   Main->on_LED1_ON_clicked();
   Main->on_LED3_ON_clicked();


   if(Main->TOP_CHECK_CNT<4)
   {
       if(Main->VIEW_MODE_CHANGE==false){
          Main->BOTTOM_STAGE_FLAG=OFF;
          Main->VIEW_MODE_CHANGE=true;
          Main->VIEW_SETTING(Main->VIEW_MODE_CHANGE);
       }
   }
   else
   {
       Main->BOTTOM_SEND_EN=true;
   }
}
