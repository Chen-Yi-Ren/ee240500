#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "xbee.h"

using namespace cv;
using namespace std;

Mat src,boundary,color_car;

int main(int, char**)
{
    Xbee* xbee;
    xbee = new Xbee("/dev/ttyUSB0");
    VideoCapture cap(4); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("line",1);
    //namedWindow("src",1);
//namedWindow("src with Gblur",1);

    Mat color,subcolor;


    for(;;)
    {
        cap >> src; // get a new frame from camera
        //imshow("src", src );

        GaussianBlur(src, color, Size(9, 9), 2, 2);
        subcolor=color(cv::Range(color.rows/4,color.rows),cv::Range(0,color.cols));
        boundary=subcolor.clone();


        //find line by color
        cvtColor(boundary,boundary,CV_RGB2HSV);
        inRange(boundary, Scalar(0,0,180), Scalar(255,255,255), boundary);
        //imshow("line",boundary);
        int count_white_left = 0;
        int count_white_right = 0;
        for( int y = 0; y < boundary.rows; y++ ) {
            for( int x = 0; x < boundary.cols/2; x++ ) {
                if ( boundary.at<uchar>(y,x) != 0 ) {
                    // change this to to 'src.atuchar>(y,x) == 255'
                    // if your img has only 1 channel
                    if ( boundary.at<cv::Vec3b>(y,x) == cv::Vec3b(255,255,255) ) {
                        count_white_left++;
                    }
                }
                if ( boundary.at<uchar>(y,x+boundary.cols/2) != 0 ) {
                    // change this to to 'src.atuchar>(y,x) == 255'
                    // if your img has only 1 channel
                    if ( boundary.at<cv::Vec3b>(y,x+boundary.cols/2) == cv::Vec3b(255,255,255) ) {
                        count_white_right++;
                    }
                }
            }
        }
        cout <<count_white_left << endl;
        cout << count_white_right << endl;
        if(count_white_left>count_white_right+2000&& count_white_left>30000){
            // go right
            xbee->terminate();
            xbee->setDir('3');
            xbee->start();

            cout << "right\n";
        }
        else if(count_white_left+2000<count_white_right && count_white_right>30000){
//             Left
            xbee->terminate();
            xbee->setDir('1');
            xbee->start();
            cout << "left\n";
        }
        else{
            xbee->terminate();
            xbee->setDir('0');
            xbee->start();
            cout << "forward\n";
        }
       // imshow("src with Gblur", color);

        if(waitKey(100) >= 0) break;
        src.release();
        //        line.release();
        //        img.release();
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

