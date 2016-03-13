#ifndef __EDGES_SUBPIX_H__
#define __EDGES_SUBPIX_H__
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

CV_EXPORTS void applySteger(cv::Mat &gray,cv::Mat &dx, cv::Mat &dy);


#endif // __EDGES_SUBPIX_H__
