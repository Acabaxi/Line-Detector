#ifndef __GAUSSIAN_DERIVATES_H__
#define __GAUSSIAN_DERIVATES_H__
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>/* erfc */
using namespace cv;
/* Constants */
#define SQRT2 1.4142135
/* 1/sqrt(2*PI) */
#define SQRT_2_PI_INV 0.398942280401432677939946059935
#define MAX_SIZE_MASK_0 3.09023230616781   /* Size for Gaussian mask */
#define MAX_SIZE_MASK_1 3.46087178201605   /* Size for 1st derivative mask */
#define MAX_SIZE_MASK_2 3.82922419517181   /* Size for 2nd derivative mask */

CV_EXPORTS double normal(double x);/* Compute the integral of the Gaussian, i.e., the normal distribution. */
CV_EXPORTS void *xcalloc(size_t nelem, size_t elsize);
CV_EXPORTS int MASK_SIZE(double MAX, double sigma);
CV_EXPORTS double phi0(double x, double sigma);
CV_EXPORTS double phi1(double x, double sigma);
CV_EXPORTS double phi2(double x,double sigma);
CV_EXPORTS Mat compute_gauss_mask_0(long *num,double sigma);
CV_EXPORTS Mat compute_gauss_mask_1(long  *num,double sigma);
CV_EXPORTS Mat compute_gauss_mask_2(long  *num,double sigma);

#endif // __GAUSSIAN_DERIVATES_H__
