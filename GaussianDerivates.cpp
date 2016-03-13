/*  Convolve an image with the derivatives of Gaussians; part of detect-lines.
    Copyright (C) 1996-1998 Carsten Steger

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* Changes Made by R. Balasubramanian for incorporating the the detect lines code to incorporate
   within GRASP (May 10th 1999) 

   The type of the pointer image has been changed to float from unsigned char in routines
   convolve_rows_gauss() and convolve_gauss()

   */

#include "GaussianDerivates.h"
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;

int MASK_SIZE(double MAX, double sigma) {
  return (int)ceil(MAX*sigma); /* Maximum mask index */
}

double normal(double x){
  return 0.5*erfc(-x/SQRT2);
}

double phi0(double x, double sigma){
  return normal(x/sigma);
}

/* The Gaussian function */
double phi1(double x, double sigma){
  double t;

  t = x/sigma;
  return SQRT_2_PI_INV/sigma*exp(-0.5*t*t);
}

/* First derivative of the Gaussian function */
double phi2(double x,double sigma){
  double t;
  t = x/sigma;
  return -x*SQRT_2_PI_INV/pow(sigma,3.0)*exp(-0.5*t*t);
}

void *xcalloc(size_t nelem, size_t elsize)
{
  void *ptr;

  if (elsize == 0)
    elsize = 1;
  if (nelem == 0)
    nelem = 1;
  ptr = calloc(nelem,elsize);
  
  return ptr;
}

Mat compute_gauss_mask_0(long *num,double sigma)
{
  long   i, n;
  double limit;
  double *h, *mask;

  limit = MASK_SIZE(MAX_SIZE_MASK_0,sigma); /* Error < 0.001 on each side */
  n = (long)limit;
  h = (double*)xcalloc(2*n+1,sizeof(double));
  mask = h + n;
  for (i=-n+1;i<=n-1;i++)
    mask[i] = phi0(-i+0.5,sigma) - phi0(-i-0.5,sigma);
  mask[-n] = 1.0 - phi0(n-0.5,sigma);
  mask[n] = phi0(-n+0.5,sigma);
  *num = n;

  Mat kernel = Mat::zeros(2*n+1 , 1 , CV_32F); 
  for(int i =0 ; i < 2*n+1 ; i++){
    printf("%.4f\n",h[i]);
    kernel.at<float>(i,0) = h[i];
  }
  return kernel;
}


/* First derivative of Gaussian smoothing mask */
Mat compute_gauss_mask_1( long   *num, double sigma)
{
  long   i, n;
  double limit;
  double *h, *mask;

  limit = MASK_SIZE(MAX_SIZE_MASK_1,sigma); /* Error < 0.001 on each side */
  n = (long)limit;
  h = (double*) xcalloc(2*n+1,sizeof(double));
  mask = h + n;
  for (i=-n+1;i<=n-1;i++)
    mask[i] = phi1(-i+0.5,sigma) - phi1(-i-0.5,sigma);
  mask[-n] = -phi1(n-0.5,sigma);
  mask[n] = phi1(-n+0.5,sigma);
  *num = n;

  Mat kernel = Mat::zeros(2*n+1 , 1 , CV_32F); 
  for(int i =0 ; i < 2*n+1 ; i++){
    printf("%.4f\n",h[i]);
    kernel.at<float>(i,0) = h[i];
  }
  return kernel;
}

/* Second derivative of Gaussian smoothing mask */
Mat compute_gauss_mask_2(long   *num,  double sigma){
  long   i, n;
  double limit;
  double *h, *mask;

  limit = MASK_SIZE(MAX_SIZE_MASK_2,sigma); /* Error < 0.001 on each side */
  n = (long)limit;
  h = (double*) xcalloc(2*n+1,sizeof(double));
  mask = h + n;
  for (i=-n+1;i<=n-1;i++)
    mask[i] = phi2(-i+0.5,sigma) - phi2(-i-0.5,sigma);
  mask[-n] = -phi2(n-0.5,sigma);
  mask[n] = phi2(-n+0.5,sigma);
  *num = n;
  
  Mat kernel = Mat::zeros(2*n+1 , 1 , CV_32F); 
  for(int i =0 ; i < 2*n+1 ; i++){
    printf("%.4f\n",h[i]);
    kernel.at<double>(i,0) = h[i];
  }
  return kernel;
}
