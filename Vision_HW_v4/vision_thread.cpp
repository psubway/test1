#include "vision_thread.h"

struct AreaCmp {
    AreaCmp(const vector<float>& _areas) : areas(&_areas) {}
    bool operator()(int a, int b) const { return (*areas)[a] > (*areas)[b]; }
    const vector<float>* areas;
};

Vision_Thread::Vision_Thread()
{

}


void Vision_Thread::run(){


    MainWindow * mw = MainWindow::getMainWinPtr();

    while(1){
        Mat frame_T_Binary;
        cvtColor(mw->frame_T, frame_T_Binary, CV_RGB2GRAY);
        blur( frame_T_Binary, frame_T_Binary, Size(5,5) );
        threshold(frame_T_Binary,frame_T_Binary,245,255,THRESH_OTSU);
        frame_T_Binary=~frame_T_Binary;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        Mat Gauss_T;
        //    GaussianBlur(frame_T_Binary, Gauss_T, Size(3, 3), 1.5, 1.5);

        Mat Canny_T;
        Canny(frame_T_Binary, Canny_T, 245, 255,3);

        Mat result(Canny_T.size(), CV_8U, cv::Scalar(0));
        findContours( Canny_T, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        //  sort(contours.begin(), contours.end(), Contou)
        vector<int> sortIdx(contours.size());
        vector<float> areas(contours.size());
        for( int n = 0; n < (int)contours.size(); n++ ) {
            sortIdx[n] = n;
            areas[n] = contourArea(contours[n], false);
        }
        std::sort( sortIdx.begin(), sortIdx.end(), AreaCmp(areas));

        if(contours.size()>0 && contourArea(contours[sortIdx[0]],false)>14000)
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
            drawContours( Contours_color, contours,sortIdx[0],cv::Scalar(0,0,255),4);
            circle( Contours_color, mc[sortIdx[0]], 4, cv::Scalar(0,255,0),3, 1, 0 );
            line(Contours_color,Point(0,mc[sortIdx[0]].y),Point(1280,mc[sortIdx[0]].y),Scalar(255,0,0),2);
            line(Contours_color,Point(mc[sortIdx[0]].x,0),Point(mc[sortIdx[0]].x,768),Scalar(255,0,0),2);

            line(Contours_color,Point(0,240),Point(640,240),Scalar(255,0,0),2);
            line(Contours_color,Point(320,0),Point(320,480),Scalar(255,0,0),2);

            imwrite("C:\\Users\\Synopex ESD\\Pictures\\TOP_STAGE.jpg", Contours_color);

        }
    }
}
