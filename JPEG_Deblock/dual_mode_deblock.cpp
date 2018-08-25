#include "stdafx.h"
#include "dual_mode_deblock.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream>

using namespace std;
using namespace cv;


float Dual_Mode_Deblock_Learner::Find_Min_a3_k(void)
{
	float a[3];
	a[0] = abs(a3_k[0]);
	a[1] = abs(a3_k[1]);
	a[2] = abs(a3_k[2]);
	float min = abs(a3_k[0]);
	for (int i = 0; i <= 2; i++) 
	{
		if (a[i] < min) 
		{
			min = a[i];
		}
	}
	return min;
}

float Dual_Mode_Deblock_Learner::Max(void)
{
	float max = v[1];
	for (int i = 1; i <= 8; i++)
	{
		if (v[i] > max)
		{
			max = v[i];
		}
	}
	return max;
}

float Dual_Mode_Deblock_Learner::Min(void)
{
	float min = v[1];
	for (int i = 1; i <= 8; i++)
	{
		if (v[i] < min)
		{
			min = v[i];
		}
	}
	return min;
}

void Dual_Mode_Deblock_Learner::Update_a3_k(void) 
{
	a3_k[0] = (c1*v[1] - c2*v[2] + c2*v[3] - c1*v[4]) / c3;
	a3_k[1] = (c1*v[3] - c2*v[4] + c2*v[5] - c1*v[6]) / c3;
	a3_k[2] = (c1*v[5] - c2*v[6] + c2*v[7] - c1*v[8]) / c3;
}

int Dual_Mode_Deblock_Learner::Compute_F(void) 
{   
	int output = 0;
	for (int i = 0; i <= 8; i++) 
	{
		int temp = abs(v[i] - v[i + 1]);
		if (temp <= T1) 
		{
			output++;
		}
	}
	return output;
}

float Dual_Mode_Deblock_Learner::Compute_Pm(int m) 
{
	if (m == 0) 
	{
		if (abs(v[1] - v[0]) < QP) 
		{
			return v[0];
		}
		else 
		{
			return v[1];
		}
	}
	else if (m == 9) 
	{
		if (abs(v[9] - v[8]) < QP) 
		{
			return v[9];
		}
		else 
		{
			return v[8];
		}
	}
	else 
	{
		if (m < 1) 
		{
			if (abs(v[1] - v[0]) < QP)
			{
				return v[0];
			}
			else
			{
				return v[1];
			}
		}
		else if (m >= 1 && m <= 8) 
		{
			return v[m];
		}
		else 
		{
			if (abs(v[9] - v[8]) < QP)
			{
				return v[9];
			}
			else
			{
				return v[8];
			}
		}
	}
}

float Dual_Mode_Deblock_Learner::Compute_bk(int k) 
{    
	float output;
	switch (k) 
	{
	  case -4: 
	  {
		  output = 1;
		  break;
	  }
	  case -3: 
	  {
		  output = 1;
		  break;
	  }
	  case -2: 
	  {
		  output = 2;
		  break;
	  }
	  case -1: 
	  {
		  output = 2;
		  break;
	  }
	  case 0: 
	  {
		  output = 4;
		  break;
	  }
	  case 1: 
	  {
		  output = 2;
		  break;
	  }
	  case 2: 
	  {
		  output = 2;
		  break;
	  }
	  case 3: 
	  {
		  output = 1;
		  break;
	  }
	  case 4: 
	  {
		  output = 1;
		  break;
	  }	
	}
	return output;
}

void Dual_Mode_Deblock_Learner::Update_v(void)
{   
	for (int i = 0; i < 10; i++)
	{
		v_update[i] = v[i];
	}
	for (int n = 1; n <= 8; n++) 
	{
		float temp = 0.0;
		for (int k = -4; k <= 4; k++) 
		{    
			int m = n + k;
			temp += Compute_Pm(m) * Compute_bk(k);
		}
		temp = temp / 16.0;
		v_update[n] = temp;
	}
}

float Dual_Mode_Deblock_Learner::Compute_Updated_a3_1(void) 
{
	if (a3_k[1] != 0) 
	{
		return (a3_k[1] * Find_Min_a3_k() / abs(a3_k[1]));
	}
	else 
	{
		return 0;
	}
}

void Dual_Mode_Deblock_Learner::Update_d(void)
{    
	float lower_bound;
	float upper_bound;
	if (v[4] > v[5]) 
	{
		lower_bound = 0.0;
		upper_bound = (v[4] - v[5]) / 2.0;
	}
	else 
	{
		lower_bound = (v[4] - v[5]) / 2.0;
		upper_bound = 0;
	}
	d = c2 / c3  * (Compute_Updated_a3_1() - a3_k[1]);
	if (d < lower_bound) 
	{
		d = lower_bound;
	}
	else if (d>upper_bound)
	{
		d = upper_bound;
	}
	else 
	{
		d = d;
	}
}

void Dual_Mode_Deblock_Learner::Update_v4_v5(void)
{   
	for (int i = 0; i < 10; i++)
	{
		v_update[i] = v[i];
	}
	v_update[4] = v[4] - d;
	v_update[5] = v[5] + d;
}

void Dual_Mode_Deblock_Learner::Process_single_v(void) 
{
	float max = Dual_Mode_Deblock_Learner::Max();
	float min = Dual_Mode_Deblock_Learner::Min();
	Update_a3_k();
	int F = Compute_F();

	if (F >= T2) 
	{  
		//smooth region 
		if (abs(max - min) < 2 * QP) 
		{
			Update_v();
		}
		else 
		{
			for (int i = 0; i < 10; i++) 
			{
				v_update[i] = v[i];
			}
		}
	}
	else 
	{   //default mode
		if (abs(a3_k[1]) < QP) 
		{
			Update_d();
			Update_v4_v5();
		}
		else 
		{
			for (int i = 0; i < 10; i++)
			{
				v_update[i] = v[i];
			}
		}
	}

}

void Dual_Mode_Deblock_Learner::Process_Single_Channel_Along_X(void) 
{
	for (int i = 1; i <= 31; i++) 
	{
		for (int j = 1; j <= 31; j++) 
		{			
			int x_start = 8 * i - 1;
			int y = 8 * j - 1;
			int real_x_start = x_start - 4;
			int real_x_end = x_start + 5;
			//read one v
			for (int p = real_x_start; p <= real_x_end; p++) 
			{
				v[p - real_x_start] = single_channel.at<float>(p, y);
			}
			//Process v
			Process_single_v();
			//Write to corresponding position
			for (int p = real_x_start; p <= real_x_end; p++)
			{
				updated_single_channel_x.at<float>(p, y) = v_update[p - real_x_start];		
			}
		}
	}
}

void Dual_Mode_Deblock_Learner::Process_Single_Channel_Along_Y(void) 
{
	for (int i = 0; i < 256; i++) 
	{
		for (int j = 0; j < 256; j++) 
		{
			updated_single_channel_y.at<float>(i, j) = updated_single_channel_x.at<float>(i, j);
		}
	}
	for (int i = 1; i <= 31; i++)
	{
		for (int j = 1; j <= 31; j++)
		{
			int x = 8 * i - 1;
			int y_start = 8 * j - 1;
			int real_y_start = y_start - 4;
			int real_y_end = y_start + 5;
			//read one v
			for (int p = real_y_start; p <= real_y_end; p++)
			{
				v[p - real_y_start] = updated_single_channel_x.at<float>(x, p);
			}
			//Process v
			Process_single_v();
			//Write to corresponding position
			for (int p = real_y_start; p <= real_y_end; p++)
			{
				updated_single_channel_y.at<float>(x, p) = v_update[p - real_y_start];
			}
		}
	}
}

Mat Dual_Mode_Deblock_Learner::Process_and_Output(void)
{
	Process_Single_Channel_Along_X();
	Process_Single_Channel_Along_Y();
	return updated_single_channel_y;
}
