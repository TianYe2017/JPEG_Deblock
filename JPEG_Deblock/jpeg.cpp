#include "stdafx.h"
#include "JPEG.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream>

using namespace std;

int My_Round(double input) 
{   
	int c = ceil(input);
	int f = floor(input);
	if (abs((double)c - input) <= abs((double)f - input)) 
	{
		return c;
	}
	else 
	{
		return f;
	}
}



vector<vector<vector<int>>> JPEG_Learner::Covert_To_Block_Array(void) 
{   
	int size = 8;
	vector<vector<vector<int>>> output;
	vector<vector<unsigned char>> matrix;
	//初始化
	for (int y = 0; y < 16; y++)
	{
		vector<unsigned char> temp;
		for (int x = 0; x < 16; x++)
		{
			temp.push_back(0x00);
		}
		matrix.push_back(temp);
	}
	//导入
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			matrix[y][x] = original_file[16 * y + x];
		}
	}
	//准备临时block
	vector<vector<int>> block_temp;
	for (int i = 0; i < 8; i++) 
	{
		vector<int> temp;
		for (int j = 0; j < 8; j++) 
		{
			temp.push_back(0);
		}
		block_temp.push_back(temp);
	}
	//开始分割
	int it = 16 / size;
	for (int y = 0; y < it; y++)
	{
		for (int x = 0; x < it; x++)
		{   
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					block_temp[i][j] = U8_to_0_255(matrix[y*size + i][x*size + j]) - 128;
				}
			}//block
			output.push_back(block_temp);
		}
	}//image
	return output;
}


double JPEG_Learner::Compute_Single_DCT(int i, int j, vector<vector<int>> input) 
{
	double Ci, Cj;
	if (i == 0) 
	{
		Ci = 1 / sqrt(2);
	}
	else 
	{
		Ci = 1.0;
	}
	if (j == 0)
	{
		Cj = 1 / sqrt(2);
	}
	else
	{
		Cj = 1.0;
	}
	double C = 1.0 / 4.0;	
	double output = 0.0;
	for (int y = 0; y < 8; y++) 
	{
		for (int x = 0; x < 8; x++) 
		{   
			double px = (2 * x + 1)*i*3.1415926 / 16.0;
			px = cos(px);
			double py = (2 * y + 1)*j*3.1415926 / 16.0;
			py = cos(py);
			output += (double)input[y][x] * px*py;
		}
	}
	output = output * C * Ci * Cj;
	cout << "dct" << output << endl;
	return output;
}


void JPEG_Learner::DCT_Transform(int quality)
{   

	for (int p = 0; p < block_array.size(); p++) 
	{   
		//cout << "block_array" << block_array.size() << " " << block_array[0].size() << " " << block_array[1][2].size() << endl;
	//step1 拆出一个block
		vector<vector<int>> temp = block_array[p];
    //step2 DCT
		vector<vector<double>> temp_double;
		for (int i = 0; i < 8; i++) 
		{   
			vector<double> zero;
			for (int j = 0; j < 8; j++) 
			{
				zero.push_back(0.0);
			}
			temp_double.push_back(zero);
		}
		for (int i = 0; i < 8; i++) 
		{
			for (int j = 0; j < 8; j++) 
			{
				temp_double[i][j] = Compute_Single_DCT(i, j, temp);
			}
		}
     //step3 Quantization
		double Q_flex[8][8];
		if (quality == 50) 
		{	
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					Q_flex[i][j] = (double)Q50[i][j];
				}
			}//for
		}
		else 
		{   
			if (quality > 50)
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						Q_flex[i][j] = (double)(100 - quality) / 50.0 * (double)Q50[i][j];
					}
				}//for
			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						Q_flex[i][j] =  50.0  /  (double)quality * (double)Q50[i][j];
					}
				}//for
			}
		}// Q_N done
		vector<vector<int>> tt;
		for (int i = 0; i < 8; i++)
		{   
			vector<int> t;
			for (int j = 0; j < 8; j++)
			{
				t.push_back(0);
			}
			tt.push_back(t);
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				tt[i][j] = My_Round(temp_double[i][j] / Q_flex[i][j]);
				cout << "tt" << tt[i][j] << endl;
			}
		}
		processed_array.push_back(tt);
	}
}


void JPEG_Learner::Print_Response_Map(void) 
{
	for (int i = 0; i < processed_array.size(); i++)
	{
		cout << "response_block #" << i << " :" << endl;
		for (int y = 0; y < 8; y++) 
		{
			for (int x = 0; x < 8; x++) 
			{
				cout << processed_array[i][y][x] << " ";
			}
			cout << "" << endl;
		}
		cout << "" << endl;
		
	}
}


void JPEG_Learner::Compress(string Path)
{   
	vector<unsigned char> h_a;
	for (int i = 0; i < processed_array.size(); i++)
	{
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{   
				if (processed_array[i][y][x] > 0) 
				{
					h_a.push_back(Zero_255_to_U8(processed_array[i][y][x]));
				}
				else 
				{
					h_a.push_back(Zero_255_to_U8( 128-processed_array[i][y][x]));
				}			
			}
		}
	}
	//-----------------step#2 transfer four .dat files to pdf-----------------//
	cout << "sorting..." << endl;
	vector<data_pair> h_a_pdf = Sort_Raw_Data(h_a);
	cout << "sort done." << endl;
	
	double entropy = Compute_Entropy(h_a_pdf);
	
	cout << "Entropy: " << entropy << endl;
	vector<haffman_node> tree;
	Create_Leaves(tree, h_a_pdf);
	haffman_node* haffman_tree_output = Build_Haffman_Tree(tree, h_a_pdf);
	cout << "size of haffman tree is: " << tree.size() << endl;
	//-----------------step#5 create encode table-----------------//
	vector<haffman_encode_pair> Table_haffman = Create_Haffman_Encode_Table_Without_Using_Recursion(tree);
	for (vector<haffman_encode_pair>::iterator it = Table_haffman.begin(); it != Table_haffman.end(); it++)
	{
		cout << (int)it->symbol << "  " << it->code << endl;
	}
	//-----------------step#7 Use Table To encode data-----------------//
	string outcome = "";
	cout << "rebuild..." << endl;
	for (vector<unsigned char>::iterator it = h_a.begin(); it != h_a.end(); it++)
	{
		outcome += Look_For_Haffman_Table(*it, Table_haffman);
	}
	Write_String_To_Dat(outcome, Path);

}




