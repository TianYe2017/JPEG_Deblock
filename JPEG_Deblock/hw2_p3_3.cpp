#include "stdafx.h"
#include "hw2_p3_3.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 
#include <sstream>


using namespace std;
using namespace cv;

float Find_PSRN_OpenCV_256_RGB(Mat v1, Mat v2)
{
	float output;
	Mat channel1[3];
	Mat channel2[3];
	split(v1, channel1);
	split(v2, channel2);

	float mse = 0.0;

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			mse += pow((channel1[0].at<float>(i, j) - channel2[0].at<float>(i, j)), 2);
			mse += pow((channel1[1].at<float>(i, j) - channel2[1].at<float>(i, j)), 2);
			mse += pow((channel1[2].at<float>(i, j) - channel2[2].at<float>(i, j)), 2);
		}
	}
	mse = mse / (65536.0 * 3.0);

	output = 10.0 * log10(255.0*255.0 / mse);

	return output;

}


void JPEG_JEPG_Gray(int num)
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
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock" + str + ".jpg";
	Mat clock = imread(path_in, 0);
	Mat clock_extended;
	//copyMakeBorder(clock, clock_extended, 3, 4, 3, 4, BORDER_REPLICATE);
	copyMakeBorder(clock, clock_extended, 3, 4, 3, 4, BORDER_CONSTANT,0);
	clock_extended.convertTo(clock_extended, CV_32F);
	int index = 0;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			stringstream ss;
			string str;
			ss << index;
			ss >> str;
			string path = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\cluster\\sub" + str + ".jpg";
			Mat sub = Mat(256, 256, CV_32F);
			for (int i = 0; i < 256; i++)
			{
				for (int j = 0; j < 256; j++)
				{
					sub.at<float>(i, j) = clock_extended.at<float>(y + i, x + j);
				}
			}
			vector<int> compression_parameter;
			compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
			compression_parameter.push_back(quality);
			imwrite(path, sub, compression_parameter);
			index++;
		}
	}
	//average
	Mat subc[64];
	for (int i = 0; i < 64; i++)
	{
		stringstream ss;
		string str;
		ss << i;
		ss >> str;
		string path = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\cluster\\sub" + str + ".jpg";
		subc[i] = imread(path, 0);
		subc[i].convertTo(subc[i], CV_32F);
	}
	Mat result = Mat(256, 256, CV_32F);
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			float sum = 0.0;
			for (int p = 0; p < 64; p++)
			{
				sum += subc[p].at<float>(i, j);
			}
			sum = sum / 64.0;
			result.at<float>(i, j) = sum;
		}
	}
	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.tif", 0);
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSNR_OpenCV_256(Ref, result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)result.at<float>(i, j)));
		}
	}
	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_clock.raw";
	Write_Image_To_Dat(output, path_out);

	cout << "done." << endl;
}


void Reapply_Jpeg_Gray(int num) 
{
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	//string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock" + str + ".jpg";
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.tif";
	Mat clock = imread(path_in, 0);
	clock.convertTo(clock,CV_32F);

	Reapply_Jpeg_Learner* p = new Reapply_Jpeg_Learner(clock, num);
	Mat result = p->Process_and_Output(-3,4,-3,4);

	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.tif", 0);
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSNR_OpenCV_256(Ref, result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)result.at<float>(i, j)));
		}
	}
	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_clock_reapply_jpeg" + str + ".raw";
	Write_Image_To_Dat(output, path_out);

	cout << "done." << endl;

}


void Reapply_Jpeg_RGB(int num) 
{
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper" + str + ".jpg";
	Mat pepper = imread(path_in);
	pepper.convertTo(pepper, CV_32F);

	Mat channel_pepper[3];
	split(pepper, channel_pepper);
	Mat channel_pepper_out[3];

	Reapply_Jpeg_Learner* p_0 = new Reapply_Jpeg_Learner(channel_pepper[0], num);
	Reapply_Jpeg_Learner* p_1 = new Reapply_Jpeg_Learner(channel_pepper[1], num);
	Reapply_Jpeg_Learner* p_2 = new Reapply_Jpeg_Learner(channel_pepper[2], num);

	channel_pepper_out[0] = p_0->Process_and_Output(-3, 4, -3, 4);
	channel_pepper_out[1] = p_1->Process_and_Output(-3, 4, -3, 4);
	channel_pepper_out[2] = p_2->Process_and_Output(-3, 4, -3, 4);

	Mat result;
	merge(channel_pepper_out, 3, result);
	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper.tif");
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSRN_OpenCV_256_RGB(Ref, result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)channel_pepper_out[2].at<float>(i, j)));
			output.push_back(Zero_255_to_U8((int)channel_pepper_out[1].at<float>(i, j)));
			output.push_back(Zero_255_to_U8((int)channel_pepper_out[0].at<float>(i, j)));
		}
	}

	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_pepper_reapply_jpeg" + str + ".raw";
	Write_Image_To_Dat(output, path_out);
	cout << "done." << endl;
}




void simple_low_pass_filter_Gray(int num) 
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
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock" + str + ".jpg";
	Mat clock = imread(path_in, 0);
	clock.convertTo(clock, CV_32F);
	Mat blur_clock;
	GaussianBlur(clock, blur_clock, cv::Size(3, 3), 1);
	blur_clock.convertTo(blur_clock, CV_32F);
	Mat result = Mat(256, 256, CV_32F);
	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j < 256; j++) 
		{
			result.at<float>(i, j) = clock.at<float>(i, j);
		}
	}
   // x_axis
	for (int i = 1; i <= 31; i++) 
	{
		int real_i = i * 8 - 1;
		for (int y = real_i - 2; y <= real_i + 2; y++)
		{
			for (int x = 0; x < 256; x++) 
			{
				result.at<float>(y, x) = blur_clock.at<float>(y, x);
			}
		}
	}
	// y_axis
	for (int i = 1; i <= 31; i++)
	{
		int real_i = i * 8 - 1;
		for (int x = real_i - 2; x <= real_i + 2; x++)
		{
			for (int y = 0; y < 256; y++)
			{
				result.at<float>(y, x) = blur_clock.at<float>(y, x);
			}
		}
	}
	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.tif", 0);
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSNR_OpenCV_256(Ref,result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)result.at<float>(i, j)));
		}
	}
	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_clock_lowpass" + str + ".raw";
	Write_Image_To_Dat(output, path_out);
	cout << "done." << endl;
   
}


void simple_low_pass_filter_RGB(int num) 
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
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper" + str + ".jpg";
	Mat clock = imread(path_in);
	clock.convertTo(clock, CV_32F);
	Mat blur_clock;
	GaussianBlur(clock, blur_clock, cv::Size(3, 3), 1);
	blur_clock.convertTo(blur_clock, CV_32F);

	Mat channel_clock[3];
	Mat channel_clock_blur[3];

	split(clock, channel_clock);
	split(blur_clock, channel_clock_blur);

	Mat channel_result[3];
	for (int i = 0; i < 3; i++) 
	{
		channel_result[i] = Mat(256, 256, CV_32F);
	}
	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j < 256; j++) 
		{
			channel_result[0].at <float>(i, j) = channel_clock[0].at<float>(i, j);
			channel_result[1].at <float>(i, j) = channel_clock[1].at<float>(i, j);
			channel_result[2].at <float>(i, j) = channel_clock[2].at<float>(i, j);
		}
	}

	//x_axis
	for (int i = 1; i <= 31; i++)
	{
		int real_i = i * 8 - 1;
		for (int y = real_i - 2; y <= real_i + 2; y++)
		{
			for (int x = 0; x < 256; x++)
			{
				channel_result[0].at<float>(y, x) = channel_clock_blur[0].at<float>(y, x);
				channel_result[1].at<float>(y, x) = channel_clock_blur[1].at<float>(y, x);
				channel_result[2].at<float>(y, x) = channel_clock_blur[2].at<float>(y, x);
			}
		}
	}
	//y_axis
	for (int i = 1; i <= 31; i++)
	{
		int real_i = i * 8 - 1;
		for (int x = real_i - 2; x <= real_i + 2; x++)
		{
			for (int y = 0; y < 256; y++)
			{
				channel_result[0].at<float>(y, x) = channel_clock_blur[0].at<float>(y, x);
				channel_result[1].at<float>(y, x) = channel_clock_blur[1].at<float>(y, x);
				channel_result[2].at<float>(y, x) = channel_clock_blur[2].at<float>(y, x);
			}
		}
	}
	Mat result;
	merge(channel_result, 3, result);
	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper.tif");
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSRN_OpenCV_256_RGB(Ref, result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)channel_result[2].at<float>(i, j)));
			output.push_back(Zero_255_to_U8((int)channel_result[1].at<float>(i, j)));
			output.push_back(Zero_255_to_U8((int)channel_result[0].at<float>(i, j)));
		}
	}

	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_pepper_lowpass" + str + ".raw";
	Write_Image_To_Dat(output, path_out);
	cout << "done." << endl;


}


void Dual_Mode_Deblock_Gray(int num) 
{
	
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock" + str + ".jpg";
	Mat clock = imread(path_in, 0);
	clock.convertTo(clock, CV_32F);

	Dual_Mode_Deblock_Learner *p = new Dual_Mode_Deblock_Learner(clock);
	Mat result = p->Process_and_Output();

	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.tif", 0);
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSNR_OpenCV_256(Ref, result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)result.at<float>(i, j)));
		}
	}
	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_clock_dual_filter" + str + ".raw";
	Write_Image_To_Dat(output, path_out);
	cout << "done." << endl;

}


void Dual_Mode_Deblock_RGB(int num) 
{
	stringstream ss;
	string str;
	ss << num;
	ss >> str;
	string path_in = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper" + str + ".jpg";
	Mat pepper = imread(path_in);
	pepper.convertTo(pepper, CV_32F);

	Mat channel_pepper[3];
	split(pepper, channel_pepper);
	Mat channel_pepper_out[3];

	Dual_Mode_Deblock_Learner *p_0 = new Dual_Mode_Deblock_Learner(channel_pepper[0]);
	Dual_Mode_Deblock_Learner *p_1 = new Dual_Mode_Deblock_Learner(channel_pepper[1]);
	Dual_Mode_Deblock_Learner *p_2 = new Dual_Mode_Deblock_Learner(channel_pepper[2]);

	channel_pepper_out[0] = p_0->Process_and_Output();
	channel_pepper_out[1] = p_1->Process_and_Output();
	channel_pepper_out[2] = p_2->Process_and_Output();

	Mat result;
	merge(channel_pepper_out, 3, result);
	Mat Ref = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper.tif");
	Ref.convertTo(Ref, CV_32F);
	cout << "PSNR: " << Find_PSRN_OpenCV_256_RGB(Ref, result) << endl;

	vector<unsigned char> output;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			output.push_back(Zero_255_to_U8((int)channel_pepper_out[2].at<float>(i, j)));
			output.push_back(Zero_255_to_U8((int)channel_pepper_out[1].at<float>(i, j)));
			output.push_back(Zero_255_to_U8((int)channel_pepper_out[0].at<float>(i, j)));
		}
	}

	string path_out = "E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\deblocked_pepper_dual_filter" + str + ".raw";
	Write_Image_To_Dat(output, path_out);
	cout << "done." << endl;

}


void HW2_Problem_three_part_three(void)
{
	//---------------------------------------------------------------------------------------------------//
	/*string path = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\clock.raw";
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
	compression_parameter.push_back(34);
	imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\clock1.jpg", img, compression_parameter);

	Mat img1 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\clock1.jpg", 0);
	cout << "PSNR: " << Find_PSNR_OpenCV_256(img, img1) << endl;*/
	//---------------------------------------------------------------------------------------------------//
	/*string path = "E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper.raw";
	vector<unsigned char> temp = ReadBinaryFile(path);
	float R[256 * 256];
	float G[256 * 256];
	float B[256 * 256];
	for (int i = 0; i < 65536; i++) 
	{
		R[i] = (float)U8_to_0_255(temp[3 * i]);
		G[i] = (float)U8_to_0_255(temp[3 * i + 1]);
		B[i] = (float)U8_to_0_255(temp[3 * i + 2]);
	}
	Mat channel_R = Mat(256, 256, CV_32F);
	Mat channel_G = Mat(256, 256, CV_32F);
	Mat channel_B = Mat(256, 256, CV_32F);
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			channel_R.at<float>(y, x) = R[256 * y + x];
			channel_G.at<float>(y, x) = G[256 * y + x];
			channel_B.at<float>(y, x) = B[256 * y + x];
		}
	}
	Mat Channel[3];
	Channel[0] = channel_R;
	Channel[1] = channel_G;
	Channel[2] = channel_B;
	Mat IMG;
	merge(Channel, 3, IMG);*/

	//Mat IMG = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper.tif");
	//IMG.convertTo(IMG, CV_32F);

	///*Mat img1 = imread("E:\\MY_LOG\\669\\hw2\\input\\P3_IN\\pepper5.jpg");
	//img1.convertTo(img1,CV_32F);
	//
	//cout << "PSNR: " << Find_PSRN_OpenCV_256_RGB(IMG,img1) << endl;*/
	//vector<int> compression_parameter;
	//compression_parameter.push_back(CV_IMWRITE_JPEG_QUALITY);
	//compression_parameter.push_back(34);
	//imwrite("E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\pepper_test.jpg", IMG, compression_parameter);


	//Mat IMG2 = imread("E:\\MY_LOG\\669\\hw2\\output\\P3_3OUT\\pepper_test.jpg");
	//IMG2.convertTo(IMG2, CV_32F);
	//cout << "PSNR: " << Find_PSRN_OpenCV_256_RGB(IMG, IMG2) << endl; 

	//---------------------------------------------------------------------------------------------------//
	//  APPROACH: lowpass filter 
	//---------------------------------------------------------------------------------------------------//
	/*for (int i = 1; i <= 5; i++) 
	{
		simple_low_pass_filter_Gray(i);
		simple_low_pass_filter_RGB(i);
	}*/
	
	//---------------------------------------------------------------------------------------------------//
	//  APPROACH: Reapply_JPEG
	//---------------------------------------------------------------------------------------------------//
	for (int i = 1; i <= 5; i++)
	{
		Reapply_Jpeg_Gray(i);
		Reapply_Jpeg_RGB(i);
	}	
	//---------------------------------------------------------------------------------------------------//
	//  APPROACH: dual mode deblock
	//---------------------------------------------------------------------------------------------------//
	/*for (int i = 1; i <= 5; i++)
	{
		Dual_Mode_Deblock_Gray(i);
		Dual_Mode_Deblock_RGB(i);
	}*/
   







	
	






}


