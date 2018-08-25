#pragma once
#ifndef JPEG_H
#define JPEG_H
#include "stdafx.h"
#include "io.h"
#include "algorithm"
#include "convert_data_brutal.h"
#include "p2_2.h"
#include "haffman_encode_table.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 


class JPEG_Learner
{
private:
	vector<unsigned char> original_file;
	vector<vector<vector<int>>> block_array;
	vector<vector<vector<int>>> processed_array;
	int Q50[8][8] = { {16, 11, 10, 16, 24, 40, 51, 61},
					  {12, 12, 14, 19, 26, 58, 60, 55},
					  {14, 13, 16, 24, 40, 57, 69, 56},
					  {14, 17, 22, 29, 51, 87, 80, 62},
					  {18, 22, 37, 56, 68, 109, 103, 77},
					  {24, 35, 55, 64, 81, 104, 113, 92},
					  {49, 64, 78, 87, 103, 121, 120, 101},
					  {72, 92, 95, 98, 112, 100, 103, 99}
	                                                      };

	vector<vector<vector<int>>> Covert_To_Block_Array(void);
	double Compute_Single_DCT(int i, int j, vector<vector<int>> input);




public:
	JPEG_Learner(string file_path)
	{
		original_file = ReadBinaryFile(file_path);
		block_array = Covert_To_Block_Array();
		cout << "" << endl;

	}

	void DCT_Transform(int quality);
	void Print_Response_Map(void);
	void Compress(string Path);





};






#endif

