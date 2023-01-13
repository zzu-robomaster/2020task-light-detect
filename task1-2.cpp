#include <iostream>
#include <ctype.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;


Rect recogrect;
Mat hsv,mask,check1,mask2;


//first attemt to adjust the picture, 
//it turned out that adjusting the camera works better
Mat hsvpre1(Mat &src,double a,double b){//a-对比度  b-亮度
    Mat hsv1;
    src.convertTo(hsv1, src.type(),a,b);
    return hsv1;
}

//Check if is there any interest points in the picture.
//Subtract the B & R Channels of the picture, use different
//convolution kernels to process the picture
int check(Mat src2) {
    Mat m0,m;
    Mat1b srcb, srcg, srcr;
    vector<Mat> channels;
    int flag=0;
    split(src2, channels);
    channels.at(0).copyTo(srcb);
    channels.at(1).copyTo(srcg);
    channels.at(2).copyTo(srcr);
    //srcb = Mat::zeros(srcg.size(), srcg.type());
    //srcg = Mat::zeros(srcg.size(), srcg.type());
    //channels.at(0) = srcb;
    //channels.at(1) = srcg;
    subtract(srcb,srcr,srcb);//通道相减
    //srcr.copyTo(channels.at(2));
    //merge(channels, src2);//另外两通道置零并合在一起 方便后续跟踪（已弃用）
    src2.copyTo(mask2);
    
    //cvtColor(src2, src2, COLOR_BGR2GRAY);
    
    threshold(srcb, hsv, 110, 255, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3));
    //morphologyEx(hsv, mask2, MORPH_CLOSE, element);
    dilate(hsv,mask2,element);
    erode(mask2, mask2, element2);//形态学处理 不直接open是为了调不同卷积核
    //hsv.copyTo(mask2);
    imshow("m0", mask2);
    //threshold(mask, mask, 100, 255, THRESH_BINARY);//???
    // cvtColor(src2, src2, COLOR_BGR2HSV);
    // src2.copyTo(hsv);
    // //m0 = mask(Rect(200,100,800,500));//720p大小1280*720
    // //imshow("m0", m0);
    // inRange(src2, Scalar(0, 0, 20), Scalar(5, 256, 245), mask);//HSV
    // inRange(src2, Scalar(160, 0, 46), Scalar(180, 256, 245), mask2);
    // bitwise_or(mask, mask2, mask);
    m = Mat::zeros(mask2.size(), mask2.type());
    if (memcmp(mask2.data, m.data, mask2.total() * mask2.elemSize())) {//如果目标为空则不跟踪
        flag = -1;
    }
    return flag;
}

Rect recongnize(Mat& src3) {
    Mat dst;
    //threshold(src3, dst, 100, 255, THRESH_BINARY);
    //imshow("dst", dst);
    vector<vector<Point>> cont;
    vector<Vec4i> hier;
    vector<float> ang;
    vector<Point2f> center;
    vector<Size2f> size;
    Rect recogrect;
    
    //src3.convertTo(src3, CV_8UC1);
    findContours(src3,cont,hier,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point(0,0));//提取最外层轮廓
    for(int i=0; i<hier.size(); ++i){
        RotatedRect rec=minAreaRect(cont[i]);//对每个轮廓求旋转矩形 提取角度 中心点和大小
        ang.push_back(float());
        center.push_back(Point2f());
        size.push_back(Size2f());
        ang[i]=rec.angle;
        center[i]=rec.center;
        size[i]=rec.size;
    }
    
    float ang1,ang2;
    Point2f cen1, cen2;
    int con1=0,con2=0;
    for(int i=0; i<hier.size(); ++i){//每对矩形对长宽比 角度差和中心点距离判定
        if((3 < size[i].width / size[i].height && size[i].width / size[i].height < 7) || (0.14 < size[i].width/size[i].height && size[i].width / size[i].height < 0.3)){
            for(int j=0; j<hier.size(); ++j){
            ang1=ang[i];
            ang2=ang[j];
            cen1 = center[i];
            cen2 = center[j];
            if(abs(ang1-ang2)<=15 && abs(cen1.x-cen2.x)>=50){con1=i;con2=j;};
        }
        }
    }
    ang.clear();
    center.clear();
    size.clear();
    dst = Mat::zeros(mask2.size(), mask2.type());
    drawContours(dst, cont, con1, 255);//画出两个矩形的点集 用一个大旋转矩形框住
    drawContours(dst, cont, con2, 255);

    if (con1 != 0 && con2 != 0) {
        recogrect = boundingRect(dst);
    }

    return recogrect;
}

//Histogram callback tracking
RotatedRect tracking(Mat& src1) {//直方图回调跟踪法
    Mat hist, backproj;
    Mat hueimage(src1.rows, src1.cols, CV_8UC1);
    Rect trackingrect;
    int histsize = 8;
    float hueranges[] = { 0,180 };
    const float* histrages = hueranges;
    int trackingflag = check(src1);

    cvtColor(mask, mask, COLOR_BGR2HSV);
    if (trackingflag) {
        //imshow("mask", mask);
        int channels[] = { 0,0 };
        //hueimage.create(mask.size(), 3);
        mixChannels(&mask, 1, &hueimage, 1, channels,1);//将mask的0号矩阵复制到hueimage的0号矩阵
        //normalize(hist, hist, 0, 255, NORM_MINMAX);
        // imshow("check1", check1);//temp
        if (trackingflag < 0)
        {
            recogrect = recongnize(mask2);//temp
            trackingflag = 1;
        }
        Mat roi(hueimage, recogrect), maskroi(mask2, recogrect);
        calcHist(&roi, 1, 0, maskroi, hist, 1, &histsize, &histrages);
        normalize(hist, hist, 0, 255, NORM_MINMAX);
        trackingrect = recogrect;
        
        
        calcBackProject(&hueimage, 1, 0, hist, backproj, &histrages);
        backproj &= mask2;//operator error
        //imshow("back", backproj);
        
        RotatedRect rotatedtrackingrect = CamShift(backproj, trackingrect, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
        //imshow("mask", mask2);

        /*Point2f* vertices = new cv::Point2f[4];
        rotatedtrackingrect.points(vertices);

        for (size_t i = 0; i < 4; i++)
        {
            cv::line(mask2, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 4, 8, 0);
        }*/
        if (trackingrect.area() <= -1) {
            int cols = backproj.cols, rows = backproj.rows;
            int offset = MIN(rows, cols) + 1;
            trackingrect = Rect(trackingrect.x - offset, trackingrect.y - offset, trackingrect.x + offset, trackingrect.y + offset) & Rect(0, 0, cols, rows);
        }
        return rotatedtrackingrect;
    }
}

// Mat_<Vec3f> frame1;
int main(){
     Mat3b frame1;
     VideoCapture cap;
     RotatedRect rec;
     cap.set(CAP_PROP_CONVERT_RGB, true);
     cap.open("2-2.avi");
     if (!cap.isOpened()) {
         cerr << "unable open cam" << endl;
         return -1;
     }
     for(;;){
         cap.read(frame1);
         if(frame1.empty()){
             break;
         }
         //imshow("demo",frame1);
         //frame1 = hsvpre1(frame1, 0.8, 20);//a b

         
         rec = tracking(frame1);
         Point2f* vertices = new cv::Point2f[4];

         rec.points(vertices);
         for (size_t i = 0; i < 4; i++)
         {
             cv::line(frame1, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 4, 8, 0);
         }
         imshow("pre", frame1);
         //imshow("mask", mask2);
         char c = (char)waitKey(10);//1115
         if(c == 27){
             break;
         }
     }


    // Mat3f pic;
    // pic=imread("1107.jpg");
    // imshow("demo",pic);
    // imshow("pre",hsvpre1(pic));
    // waitKey(0);
    return 0;
}