#pragma once
#ifndef DUAL_MODE_DEBLOCK_H
#define DUAL_MODE_DEBLOCK_H
#include "stdafx.h"
#include "io.h"
#include "algorithm"
#include "convert_data_brutal.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 
#include <opencv2/opencv.hpp> 
#include <opencv2/core/core.hpp>



using namespace std;
using namespace cv;


class Dual_Mode_Deblock_Learner 
{
private:
	float c1 = 2.0;
	float c2 = 5.0;
	float c3 = 8.0;
	int T1 = 2;
	int T2 = 6;
	int QP = 17;
	float v[10];
	float v_update[10];
	float a3_k[3];
	float d;
	
	Mat single_channel = Mat(256,256,CV_32F);
	Mat updated_single_channel_x = Mat(256, 256, CV_32F);
	Mat updated_single_channel_y = Mat(256, 256, CV_32F);

	float Find_Min_a3_k(void);
	float Max(void);
	float Min(void);
	void Update_a3_k(void);
	int Compute_F(void);
	float Compute_Pm(int m);
	float Compute_bk(int k);
	void Update_v(void);
	float Compute_Updated_a3_1(void);
	void Update_d(void);
	void Update_v4_v5(void);
	void Process_single_v(void);
	void Process_Single_Channel_Along_X(void);
	void Process_Single_Channel_Along_Y(void);


	





public:

	Dual_Mode_Deblock_Learner(Mat channel)
	{
		for (int i = 0; i < 256; i++) 
		{
			for (int j = 0; j < 256; j++) 
			{
				single_channel.at<float>(i, j) = channel.at<float>(i, j);
				updated_single_channel_x.at<float>(i, j) = channel.at<float>(i, j);
			}
		}
	}
	
	Mat Process_and_Output(void);











};







#endif
