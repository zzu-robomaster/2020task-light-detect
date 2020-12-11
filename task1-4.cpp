#include <iostream>
#include <ctype.h>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>

#include "task1-4.h"
//#include "pre.h"
using namespace std;
using namespace cv;


vector<vector<float> > led_v_h;
//const int maxn = 100 + 5;
//float led_v_h[maxn][maxn];
vector<vector<Point> > area_v;
int armor_index = 0;
//vector<vector<int> > armor_led_link;
Mat hsv, mask, check1, mask2;

ArmorParam param;
int color_ = param.color;

//void pre(Mat src) {
//    //Mat m0, m;
//    Mat1b srcb, srcg, srcr;
//    vector<Mat> channels;
//    split(src, channels);
//    channels.at(0).copyTo(srcb);
//    channels.at(2).copyTo(srcr);
//    if (color_ == 0) {
//        subtract(srcb, srcr, srcb);//通道相减
//    }
//    else {
//        subtract(srcr, srcb, srcb);
//    }
//
//    //src.copyTo(mask);
//    threshold(srcb, hsv, 125, 255, THRESH_BINARY);
//    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//    Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));
//    //morphologyEx(hsv, mask2, MORPH_CLOSE, element);
//    dilate(hsv, mask2, element);
//    //erode(mask2, mask2, element2);//形态学处理 不直接open是为了调不同卷积核
//    //hsv.copyTo(mask2);
//    imshow("m0", mask2);
//}

void pre_2(Mat _src)
{
    vector<Mat> bgr;
    Mat _binary, dst;
    elment_1 = getStructuringElement(MORPH_RECT, Size(5, 5));
    split(_src, bgr);
    cvtColor(_src, _binary, COLOR_RGB2GRAY);
    threshold(_binary, _binary, thresh_1, 255, THRESH_BINARY);
    //imshow("_binary", _binary);
    Mat result_image;
    if (team == 1)
    {
        subtract(bgr[2], bgr[1], result_image);
    }
    else
    {
        subtract(bgr[0], bgr[2], result_image);
    }
    threshold(result_image, result_image, thresh_2, 255, THRESH_BINARY);
    dilate(result_image, result_image, elment_1);
    //imshow("result_image", result_image);
    dst = _binary & result_image;
    dilate(dst, mask2, elment_1);
    cv::imshow("dst", dst);
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
        if (rec.size.area() > 5) {
            ang.push_back(float());
            center.push_back(Point2f());
            size.push_back(Size2f());
            ang[i] = rec.angle;
            center[i] = rec.center;
            size[i] = rec.size;
        }
    }

    float ang1, ang2;
    Point2f cen1, cen2;
    int con1 = 0, con2 = 0/*,num = 0*/;
    vector<Point> v1,v2,cont_area;
    
    for (int i = 0; i < hier.size(); ++i) {//每对led对长宽比和周长 角度差和中心点距离判定
        armor_index = 0;
        //if ((2<size[i].width && size[i].width< 50) || (2<size[i].height && size[i].height < 50)) {//降低要求！orz
        //if (15<=2*(size[i].width + size[i].height)<=4000) {//周长判定

        float ledw, ledh;
        if (size[i].width > size[i].height) {
            ledw = size[i].width;
            ledh = size[i].height;
        }
        else {
            ledh = size[i].width;
            ledw = size[i].height;
        }

        float ledratio = ledw / ledh;
        float ledc = 2 * (ledw + ledh);
        if ((min_ledc < ledratio && ledratio < max_ledc) || peri_led_min <= ledc <= peri_led_max) {
        //if (1) {//(0.2 < size[i].width / size[i].height< 0.7) && 15 <= 2 * (size[i].width + size[i].height) <= 400;
            //cout << "筛选灯条通过 ";
            //cout << "ledc:" << ledc << endl;
            //cout << "ledratio:" << ledratio << endl;
            for (int j = 0; j < hier.size(); ++j) {
                if (j == i)continue;
                //cout << "led_id:" << i << " and " << j << endl;
                ang1 = ang[i];
                ang2 = ang[j];
                cen1 = center[i];
                cen2 = center[j];
                //cout << "ang: " << abs(ang1 - ang2);
                if (abs(ang1 - ang2) <= ang_led  ) {
                    //cout << "角度通过 ";
                    //cout << "cen: " << abs(cen1.x - cen2.x);
                    if (1 && abs(cen1.x - cen2.x) >= centre_led ) {
                        //cout << "中心距离通过 ";
                        //cout << "hei_com: " << size[i].height / size[j].height << endl;

                        float ledh_2;
                            if (size[j].width > size[j].height) {
                                ledh_2 = size[j].height;
                        }else ledh_2 = size[j].width;

                        if (1 && min_height_com < ledh / ledh_2 && ledh / ledh_2 < max_height_com) {
                            //cout << "长宽比通过 ";
                            con1 = i; con2 = j;
                            v1 = cont[con1];
                            v2 = cont[con2];
                            cont_area = v1;
                            cont_area.insert(cont_area.end(), v2.begin(), v2.end());
                            area_v.push_back(cont_area);//求点集的旋转矩形

                            vector<float> led_h;
                            led_h.push_back(ledh);
                            led_h.push_back(ledh_2);
                            led_v_h.push_back(led_h);

                            v1.clear();
                            v2.clear();
                            cont_area.clear();
                            led_h.clear();
                            //cout << "初次匹配通过 ";
                            armor_index++;
                        }
                    }
                }
             }
        }
        //cout << endl;
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

//1129 TODO---------装甲板筛选函数优化 添加与灯条比较条件
void detect(Mat frame0){
    vector<RotatedRect> armor_v;
    vector<float> armor_pri;
    vector<vector<int>> armor_num;

    for (size_t i0 = 0; i0 <= armor_index; i0++) {
        RotatedRect recogrect;
        vector<Point> area0;
        area0 = area_v[i0];
        recogrect = minAreaRect(area0);


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
            Mat roi = mask2(Range(recy, recy + rech*0.6f), Range(recx, recx + recw*0.6f));
            //imshow("roi", roi);
            int average_intensity = static_cast<int>(mean(roi).val[0]);//<50
            //cout << "inten:" << average_intensity << endl;
            //if (recw < 4.0f * rech && recw > 1.2f * rech && 2 * (recw + rech) < 400) {
            if (recw < max_height_com_mor * rech  ) {          
                //cout << "小于最大长宽比 ";
                if (1 && recw > min_height_com_mor * rech)
                {
                    //cout << "大于最小长宽比 ";
                    if (1&& 2 * (recw + rech) < peri_moro) {
                        //cout << "符合周长" << endl;
                        float ratio = recw / rech;
                        float peri = 2 * (recw + rech);
                        //cout <<"ratio:"<< ratio << endl;
                        //cout <<"peri:"<< peri << endl;
                        if (average_intensity*(recw+rech)/mid_intensity < mid_intensity) {
                            //cout << "符合中间亮度" << endl;

                            float delta1 = abs(rech - led_v_h[i0][0]);
                            float delta2 = abs(rech - led_v_h[i0][1]);
                            //cout << "delta:" <<delta<< endl;
                            if (delta1 > armor_led_delta_h_min && delta2 > armor_led_delta_h_min && delta1<armor_led_delta_h_max && delta2 < armor_led_delta_h_max) {
                                cout <<"index:"<<i0<< " delta:" << delta1 <<" "<<delta2<< endl;//装甲板与灯条的宽差在一定范围内
                                //cout << "the difference between led_h & armor_h is acceptable" << endl;
                                armor_v.push_back(recogrect);
                                armor_pri.push_back(ratio * peri);
                                
                            }
                            
                        }
                    }
                }
            }
        }
        
        
        area0.clear();
    }

    float max = 0.0f;
    int ind_max = -1;
    for (int i1 = 0; i1 < armor_v.size(); ++i1) {
        if (armor_pri[i1] > max)max = armor_pri[i1]; ind_max = i1;
        Point2f* vertices = new cv::Point2f[4];
        armor_v[i1].points(vertices);
        for (size_t i = 0; i < 4; i++)
        {
            cv::line(frame0, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 1, 8, 0);
        }
    }
    if (ind_max >= 0) {
        cout << "best armor: " << ind_max << endl;
        Point2f* vertices = new cv::Point2f[4];
        armor_v[ind_max].points(vertices);
        for (size_t i = 0; i < 4; i++)
        {
            cv::line(frame0, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 255, 0), 1, 8, 0);
        }
    }
    max = 0.0f;
    ind_max = -1;
    area_v.clear();
    led_v_h.clear();
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
    cap.open("test1206.mp4");
    //cap.open(0);
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
        pre_2(frame1);
        if (check() > 0) {
            //cout << "进入detect" << endl;
            detect(frame1);
        }
        
        finish = clock();
        double time = static_cast<double>(finish - start) / CLOCKS_PER_SEC;
        /*cout <<"time"<< time << endl;*/
        //cout << 1 / time << endl;
        cv::imshow("frame1", frame1);
        //imshow("mask", mask2);
        char c = (char)waitKey(1);//1115
        if (c == 27) {
            break;
        }
        //getchar();
    }
    return 0;
}