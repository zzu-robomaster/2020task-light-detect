#pragma once

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define thresh_1 125
#define thresh_2 40 

#define blue 1
#define red 0

#define team red

Mat elment_1;

//int pre_1(cv::Mat _src)
//{
//	Mat gray, result_image, binary_1, binary_2;
//	vector<Mat> bgr;
//	cvtColor(_src, gray, COLOR_RGB2GRAY);
//	split(_src, bgr);
//	if (team == 1)
//	{
//		subtract(bgr[2], bgr[1], result_image);
//	}
//	else
//	{
//		subtract(bgr[0], bgr[2], result_image);
//	}
//	threshold(gray, binary_1, thresh_1, 255, THRESH_BINARY);
//	threshold(gray, binary_2, thresh_2, 255, THRESH_BINARY);
//
//	return 0;
//};

int pre_2(Mat _src)
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
	dilate(dst, dst, elment_1);
	imshow("dst", dst);
	return 0;
}