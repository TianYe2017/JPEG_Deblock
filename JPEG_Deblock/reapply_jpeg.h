#pragma once
#ifndef REAPPLY_JPEG_H
#define REAPPLY_JPEG_H
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


class Reapply_Jpeg_Learner 
{
private:
	int N;
	Mat channel = Mat(256, 256, CV_32F);
	Mat channel_out = Mat(256, 256, CV_32F);



public:
	Reapply_Jpeg_Learner(Mat single_channel, int num) 
	{   
		int quality;
		if (num == 1)
		{
			quality = 8;
		}
		else if (num == 2)
		{
			quality = 12;
		}
		else if (num == 3)
		{
			quality = 16;
		}
		else if (num == 4)
		{
			quality = 24;
		}
		else
		{
			quality = 34;
		}
		N = quality;
		for (int i = 0; i < 256; i++) 
		{
			for (int j = 0; j < 256; j++) 
			{
				channel.at<float>(i, j) = single_channel.at<float>(i, j);
			}
		}	
	}

	Mat Process_and_Output(int x_shift_left, int x_shift_right,int y_shift_top,int y_shift_bottom);






};





#endif
