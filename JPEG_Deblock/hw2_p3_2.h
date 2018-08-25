#pragma once
#ifndef HW2_P3_2_H
#define HW2_P3_2_H
#include "stdafx.h"
#include "io.h"
#include "jpeg.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 
#include <opencv2/opencv.hpp> 
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

void HW2_Problem_three_part_two(void);
float Find_PSNR_OpenCV_256(Mat m1, Mat m2);


#endif