#include "stdafx.h"
#include "hw2_p3_2.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 


using namespace std;
using namespace cv;


float Find_PSNR_OpenCV_256(Mat m1, Mat m2) 
{   
	m1.convertTo(m1, CV_32F);
	m2.convertTo(m2, CV_32F);
	float MSE = 0.0;
	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j < 256; j++) 
		{
			MSE += pow(((int)m1.at<float>(i, j) - (int)m2.at<float>(i, j)), 2);
		}
	}
	MSE = MSE / 65536;
	float output = 10.0 * log10(255.0*255.0 / MSE);
	return output;
}


void HW2_Problem_three_part_two(void) 
{   

	string path = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.raw";
	vector<unsigned char> temp = ReadBinaryFile(path);
	Mat img = Mat(256, 256, CV_32F);
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			img.at<float>(i, j) = (float)U8_to_0_255(temp[256 * i + j]);
		}
	}
	vector<int> compression_parameter;


	compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_parameter.push_back(1);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock1.jpg", img, compression_parameter);

	compression_parameter.clear();
	compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_parameter.push_back(10);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock10.jpg", img, compression_parameter);

	compression_parameter.clear();
	compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_parameter.push_back(20);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock20.jpg", img, compression_parameter);

	compression_parameter.clear();
	compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_parameter.push_back(40);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock40.jpg", img, compression_parameter);

	compression_parameter.clear();
	compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_parameter.push_back(60);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock60.jpg", img, compression_parameter);

	compression_parameter.clear();
	compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_parameter.push_back(100);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock100.jpg", img, compression_parameter);
	//-------------------------------------------------------------------------------------------------//
	Mat img1 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock1.jpg",0);
	cout << "N=1 PSNR: " << Find_PSNR_OpenCV_256(img, img1) << endl;

	Mat img10 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock10.jpg",0);
	cout << "N=10 PSNR: " << Find_PSNR_OpenCV_256(img, img10) << endl;

	Mat img20 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock20.jpg",0);
	cout << "N=20 PSNR: " << Find_PSNR_OpenCV_256(img, img20) << endl;

	Mat img40 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock40.jpg",0);
	cout << "N=40 PSNR: " << Find_PSNR_OpenCV_256(img, img40) << endl;

	Mat img60 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock60.jpg",0);
	cout << "N=60 PSNR: " << Find_PSNR_OpenCV_256(img, img60) << endl;

	Mat img100 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_2OUT\\clock100.jpg",0);
	cout << "N=100 PSNR: " << Find_PSNR_OpenCV_256(img, img100) << endl;
	
}