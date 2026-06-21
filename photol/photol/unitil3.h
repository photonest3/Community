#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;

double get_scale(int type, int width, int height);
double calMEAN(Scalar result);
void load_bitmap(HINSTANCE his, Mat& img, UINT resID);
