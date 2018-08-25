#include "stdafx.h"
#include "reapply_jpeg.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream>

using namespace std;
using namespace cv;

Mat Reapply_Jpeg_Learner :: Process_and_Output(int x_shift_left, int x_shift_right, int y_shift_top, int y_shift_bottom)
{   
	vector<Mat> cluster;
	for (int y_shift = y_shift_top; y_shift <= y_shift_bottom; y_shift++)
	{
		for (int x_shift = x_shift_left; x_shift <= x_shift_right; x_shift++) 
		{
			//cout << "hit" << endl;
			//border extend and shift
			Mat channel_extended_y;
			if (y_shift < 0) 
			{
				copyMakeBorder(channel, channel_extended_y, 0, -y_shift, 0, 0, BORDER_REPLICATE);
			}
			else
			{
				copyMakeBorder(channel, channel_extended_y, y_shift, 0, 0, 0, BORDER_REPLICATE);
			}
			Mat channel_extended_xy;
			if (x_shift < 0) 
			{
				copyMakeBorder(channel_extended_y, channel_extended_xy, 0, 0, 0, -x_shift, BORDER_REPLICATE);
			}
			else 
			{
				copyMakeBorder(channel_extended_y, channel_extended_xy, 0, 0, x_shift, 0, BORDER_REPLICATE);
			}
			channel_extended_xy.convertTo(channel_extended_xy, CV_32F);

			//encode
			vector<unsigned char> temp;
			vector<int> compression_parameter;
			compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
			compression_parameter.push_back(N);
			imencode(".jpg", channel_extended_xy, temp, compression_parameter);

			//decode
			Mat decoded_img = imdecode(temp, 0);
			decoded_img.convertTo(decoded_img,CV_32F);

			//shift back
			Mat result = Mat(256, 256, CV_32F);
			int start_point_y;
			if (y_shift <= 0) 
			{
				start_point_y = 0;
			}
			else 
			{
				start_point_y = y_shift;
			}
			int start_point_x;
			if (x_shift <= 0)
			{
				start_point_x = 0;
			}
			else
			{
				start_point_x = x_shift;
			}
			for (int y = 0; y < 256; y++) 
			{
				for (int x = 0; x < 256; x++) 
				{
					result.at<float>(y, x) = decoded_img.at<float>(y + start_point_y, x + start_point_x);
				}
			}
			cluster.push_back(result);
		}
	}

	//average
	float total = cluster.size();
	for (int y = 0; y < 256; y++) 
	{
		for (int x = 0; x < 256; x++)
		{
			float temp = 0.0;
			for (int i = 0; i < cluster.size(); i++)
			{
				temp += cluster[i].at<float>(y, x);
			}
			temp = temp / total;
			channel_out.at<float>(y, x) = temp;
		}	
	}

	////border extend
	//Mat channel_extended;
	//copyMakeBorder(channel, channel_extended, 2, 2, 2, 2, BORDER_REPLICATE);
	//channel_extended.convertTo(channel_extended, CV_32F);
	//cout << channel_extended.rows << "  " << channel_extended.cols << endl;

	////shift and encode
	//vector<unsigned char> cluster[25];
	//int index = 0;
	//for (int y = 0; y <= 4; y++)
	//{
	//	for (int x = 0; x <= 4; x++)
	//	{
	//		Mat sub = Mat(256, 256, CV_32F);
	//		for (int i = 0; i < 256; i++)
	//		{
	//			for (int j = 0; j < 256; j++)
	//			{
	//				sub.at<float>(i, j) = channel_extended.at<float>(y + i, x + j);
	//			}
	//		}
	//		vector<int> compression_parameter;
	//		compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	//		compression_parameter.push_back(N);
	//		imencode(".jpg", sub, cluster[index], compression_parameter);
	//		index++;
	//	}
	//}

	//// decode and average
	//Mat decoded_cluster[25];
	//for (int i = 0; i < 25; i++) 
	//{   
	//	decoded_cluster[i] = imdecode(cluster[i],0);
	//	decoded_cluster[i].convertTo(decoded_cluster[i],CV_32F);
	//}
	//for (int i = 0; i < 256; i++) 
	//{
	//	for (int j = 0; j < 256; j++) 
	//	{
	//		float temp = 0.0;
	//		for (int p = 0; p < 25; p++) 
	//		{
	//			temp += decoded_cluster[p].at<float>(i, j);
	//		}
	//		temp = temp / 25.0;
	//		channel_out.at<float>(i, j) = temp;
	//	}
	//}
	return channel_out;
}













