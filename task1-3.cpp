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


vector<vector<Point> > area_v;
Mat hsv, mask, check1, mask2;

ArmorParam param;
int color_ = param.color;

void pre(Mat src) {
    //Mat m0, m;
    Mat1b srcb, srcg, srcr;
    vector<Mat> channels;
    split(src, channels);
    channels.at(0).copyTo(srcb);
    channels.at(2).copyTo(srcr);
    if (color_ == 0) {
        subtract(srcb, srcr, srcb);//通道相减
    }
    else {
        subtract(srcr, srcb, srcb);
    }

    //src.copyTo(mask);
    threshold(srcb, hsv, 110, 255, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));
    //morphologyEx(hsv, mask2, MORPH_CLOSE, element);
    dilate(hsv, mask2, element);
    erode(mask2, mask2, element2);//形态学处理 不直接open是为了调不同卷积核
    //hsv.copyTo(mask2);
    imshow("m0", mask2);
}

int check() {
    int flag = 0;
    //Mat dst;
    vector<vector<Point> > cont;
    vector<Vec4i> hier;
    vector<float> ang;
    vector<Point2f> center;
    vector<Size2f> size;
    findContours(mask2, cont, hier, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));//提取最外层轮廓
    for (int i = 0; i < hier.size(); ++i) {
        RotatedRect rec = minAreaRect(cont[i]);//对每个轮廓求旋转矩形 提取角度 中心点和大小
        ang.push_back(float());
        center.push_back(Point2f());
        size.push_back(Size2f());
        ang[i] = rec.angle;
        center[i] = rec.center;
        size[i] = rec.size;
    }

    float ang1, ang2;
    Point2f cen1, cen2;
    int con1 = 0, con2 = 0/*,num = 0*/;
    vector<Point> v1,v2,cont_area;
    for (int i = 0; i < hier.size(); ++i) {//每对矩形对长宽比和周长 角度差和中心点距离判定
        //if ((2<size[i].width && size[i].width< 50) || (2<size[i].height && size[i].height < 50)) {//降低要求！orz
        //if (15<=2*(size[i].width + size[i].height)<=4000) {//周长判定
        if ((1 < size[i].width / size[i].height && size[i].width / size[i].height < 7) || (0.14 < size[i].width / size[i].height && size[i].width / size[i].height < 1) || 15 <= 2 * (size[i].width + size[i].height) <= 4000) {
            //cout << "2" << endl;
            for (int j = 0; j < hier.size(); ++j) {
                ang1 = ang[i];
                ang2 = ang[j];
                cen1 = center[i];
                cen2 = center[j];
                if (abs(ang1 - ang2) <= 15 && abs(cen1.x - cen2.x) >= 5) { 
                    con1 = i; con2 = j; 
                    v1 = cont[con1];
                    v2 = cont[con2];
                    cont_area = v1;
                    cont_area.insert(cont_area.end(), v2.begin(), v2.end());
                    area_v.push_back(cont_area);//求点集的旋转矩形--------BUG------------1128
                    v1.clear();
                    v2.clear();
                    cont_area.clear();
                    //num++;
                };
                
                //cout << i << j << endl;
            }
        }
    }

    ang.clear();
    center.clear();
    size.clear();

    //dst = Mat::zeros(mask2.size(), mask2.type());
    //drawContours(dst, cont, con1, 255);//画出两个矩形的点集 用一个大旋转矩形框住
    //drawContours(dst, cont, con2, 255);

    if (con1 == 0 && con2 == 0) {
        flag = -1;
    }else {
        flag = 1;
    }
    return flag;
}

//1127 TODO---------装甲板筛选函数
void detect(Mat frame0){
    RotatedRect recogrect;

    for (size_t i0 = 0; i0 < area_v.size(); i0++) {
        vector<Point> area0;
        area0 = area_v[i0];
        recogrect = fitEllipse(area0);


        float recw, rech;
        if (recogrect.size.width > recogrect.size.height) {
            recw = recogrect.size.width;
            rech = recogrect.size.height;
        }
        else {
            rech = recogrect.size.width;
            recw = recogrect.size.height;
        }
        float recx = recogrect.center.x - recw / 3;
        float recy = recogrect.center.y - rech / 3;
        if (recw < 1 || rech < 1 || recx < 1 || recy < 1
            || recw + recx > frame0.cols || rech + recy > frame0.rows) {
            cout << "rect out of range" << endl;
        }
        else {
            Mat roi = frame0(Range(recy, recy + rech), Range(recx, recx + recw));
            int average_intensity = static_cast<int>(mean(roi).val[0]);//<50

            if (recw < 4.0f * rech && rech < 1.2f * recw) {
                if (average_intensity < 50) {
                    Point2f* vertices = new cv::Point2f[4];
                    recogrect.points(vertices);
                    for (size_t i = 0; i < 4; i++)
                    {
                        cv::line(frame0, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 4, 8, 0);
                    }
                }
            }
        }
        
        
        area0.clear();
    }
    area_v.clear();
}

//void draw(RotatedRect rec,Mat frame) {
//    Point2f* vertices = new cv::Point2f[4];
//    rec.points(vertices);
//    for (size_t i = 0; i < 4; i++)
//    {
//        cv::line(frame, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 4, 8, 0);
//    }
//}

int main() {
    clock_t start,finish;
    Mat3b frame1;
    VideoCapture cap;
    RotatedRect rec;
    cap.set(CAP_PROP_CONVERT_RGB, true);
    cap.open("2-2.avi");
    if (!cap.isOpened()) {
        cerr << "unable open cam" << endl;
        return -1;
    }
    for (;;) {
        start = clock();
        cap.read(frame1);
        if (frame1.empty()) {
            break;
        }
        pre(frame1);
        if (check() > 0) {
            detect(frame1);
            
        }
        
        finish = clock();
        double time = static_cast<double>(finish - start) / CLOCKS_PER_SEC;
        cout << time << endl;

        imshow("frame1", frame1);
        //imshow("mask", mask2);
        char c = (char)waitKey(10);//1115
        if (c == 27) {
            break;
        }
    }
    return 0;
}