#include <iostream>
#include <ctype.h>
#include <imgproc.hpp>
#include <highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;


int main() {

    Mat3b frame1;
    VideoCapture cap;

    cap.set(CAP_PROP_CONVERT_RGB, true);
    //cap.open("2-2.avi");
    cap.open(1);
    if (!cap.isOpened()) {
        cerr << "unable open cam" << endl;
        return -1;
    }
    for (;;) {

        cap.read(frame1);
        if (frame1.empty()) {
            break;
        }

        imshow("frame1", frame1);
        //imshow("mask", mask2);
        char c = (char)waitKey(10);//1115
        if (c == 27) {
            break;
        }
    }
    return 0;
}