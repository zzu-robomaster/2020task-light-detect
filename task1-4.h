#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
//��ֵ������
#define thresh_1 120//pre_2
#define thresh_2 60 
#define blue 1
#define red 0
#define team red
//��������
#define peri_led_max 100
#define peri_led_min 15
#define min_ledc 0.4
#define max_ledc 1
#define ang_led 100
#define centre_led 5
#define min_height_com 0.3f
#define max_height_com 2.5f
//װ�װ�����
#define mid_intensity 100
#define peri_moro 300
#define max_height_com_mor 3.3f
#define min_height_com_mor 1.5f
#define armor_led_delta_h_min 2.5f//װ�װ�������Ŀ����һ����Χ��
#define armor_led_delta_h_max 7.5f
Mat elment_1;

struct ArmorParam
{
    int color = 0;       // ����������ɫ��0����ɫ��1�Ǻ�ɫ
};

