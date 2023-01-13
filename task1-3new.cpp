#include <iostream>
#include <ctype.h>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>

#include "task1-3.h"
using namespace std;
using namespace cv;


RotatedRect recogrect;
Mat hsv, mask, check1, mask2;

ArmorParam param;
int color_ = param.color;

int check(Mat src2) {
    Mat m0, m;
    Mat1b srcb, srcg, srcr;
    vector<Mat> channels;
    int flag = 0;
    src2.copyTo(mask);

    split(src2, channels);
    channels.at(0).copyTo(srcb);
    channels.at(2).copyTo(srcr);
    if (color_ == 0) {
        subtract(srcb, srcr, m0);//ͨ�����
    }
    else {
        subtract(srcr, srcb, m0);
    }

    threshold(m0, m, 110, 255, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));
    //morphologyEx(hsv, mask2, MORPH_CLOSE, element);
    dilate(m, m, element);
    erode(m, mask2, element2);//��̬ѧ���� ��ֱ��open��Ϊ�˵���ͬ�����
    //hsv.copyTo(mask2);
    imshow("check", m);


    vector<LED_Stick> LED_Stick_v;  // �������п��ܵĵ�������

    Mat dst;
    vector<vector<Point> > cont;
    vector<Vec4i> hier;
    findContours(mask2, cont, hier, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));//��ȡ���������
    for (int i = 0; i < hier.size(); ++i) {
        RotatedRect rec = minAreaRect(cont[i]);//��ÿ����������ת����
    }
}