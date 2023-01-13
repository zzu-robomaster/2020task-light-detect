#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
//二值化数据
#define thresh_1 120//pre_2
#define thresh_2 60 
#define blue 1
#define red 0
#define team red
//灯条数据
#define peri_led_max 100
#define peri_led_min 15
#define min_ledc 0.4
#define max_ledc 1
#define ang_led 100
#define centre_led 5
#define min_height_com 0.3f
#define max_height_com 2.5f
//装甲板数据
#define mid_intensity 100
#define peri_moro 300
#define max_height_com_mor 3.3f
#define min_height_com_mor 1.5f
#define armor_led_delta_h_min 2.5f//装甲板与灯条的宽差在一定范围内
#define armor_led_delta_h_max 7.5f
Mat elment_1;

struct ArmorParam
{
    int color = 0;       // 待检测灯条颜色，0是蓝色，1是红色
};

