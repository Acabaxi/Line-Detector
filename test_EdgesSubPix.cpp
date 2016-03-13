#include <opencv2/opencv.hpp>
//#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <cvaux.h>
#include <cxcore.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "EdgesSubPix.h"
#include "GaussianDerivates.h"
using namespace cv;
using namespace std;

void getLines(Mat src, Mat gray_image);

int main(int argc, char *argv[])
{
    
    long n;
    double sigma = 0.8;
    Mat kernel = compute_gauss_mask_1(&n,sigma);//gauss kernel derivative

    Mat one = Mat::ones(Size(1, 1), CV_32F);
    Mat blur ,blur_two;
    Mat image;

    Mat src = imread(argv[1],CV_LOAD_IMAGE_ANYDEPTH);//leer imagen de 16 bits
    src.convertTo(image,CV_8UC1);
    Mat derivative_x,derivative_y;
    // fx = f(x,y)*g'(sigma)    derivative in direction x : (1,0)
    // fy = f(x,y)*g'(sigma)    derivative in direction y : (0,1)
    sepFilter2D(src, derivative_x, -1, kernel, one);
    sepFilter2D(src, derivative_y, -1, one, kernel);
    /*imshow("Derivative x",derivative_x);
    imshow("Derivative Y",derivative_y);
    waitKey();*/
    applySteger(src,derivative_x,derivative_y);
    imwrite("./steger.tif",src);
    imshow("Steger",src);

    Mat binary ;
    threshold(src,binary, 0, 255, CV_THRESH_OTSU);
    imshow("Binary",binary);
    waitKey();
    getLines(image,binary);

    //maxValues.convertTo(image,CV_8UC1);
    //imshow(" Max Values ",src);
    //waitKey();
    //imwrite("./output.tif",src);
    /*
    Mat img = Mat::ones(image.size(),CV_16UC1);
    drawContour(contour,img);
    Mat binary = Mat::zeros(image.size(),CV_8U);
    img.convertTo(img,CV_8U);
    
    //CannyThreshold(binary,binary);
    imshow("Binaria",binary);
    //imwrite("./output.tiff",img);
    src.convertTo(image,CV_8U);
    getLines(image,binary);*/
    return 0;
}

void getLines(Mat src, Mat gray_image){
  Mat image;
  float theta,rho;
  int threshold = 50;//numero de puntos intersectados que se consideran que son colineales
  int indexVertical = 5,indexHorizontal = 5;
  
  int thiningLines = 2;

  vector<Vec2f> lines;
  vector<Vec2f> lines_selected;
  Vec2f currentLine;

  cvtColor(src,image ,CV_GRAY2RGB);
  HoughLines(gray_image,lines, 1, CV_PI/180,threshold,0,0);
  list <Vec2f> linesVerticalDetected;
  list <Vec2f> linesHorizontalDetected;
    
    /*vector<Vec4i> lines;
    HoughLinesP(gray_image, lines, 1, CV_PI/180, 80, 2, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( image, Point(lines[i][0], lines[i][1]),
            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }*/
  
  for(int i = 0 ; i< lines.size() ; i++){
    currentLine = lines[i];
    rho = currentLine[0];//distancia rho
    theta = currentLine[1];//angulo theta
    if(theta >= 87.0*CV_PI/180.0 && theta <= CV_PI/2.0){//linea horizontal
      linesHorizontalDetected.push_back(currentLine);
    }else if(theta == 0.0 && theta <= 5*CV_PI/180.0 ){//linea vertical
      linesVerticalDetected.push_back(currentLine);
    }
  }

  while (!linesVerticalDetected.empty()){
    currentLine = linesVerticalDetected.back();
   
    linesVerticalDetected.pop_back();
    rho = currentLine[0];
    theta = currentLine[1];

    lines_selected.push_back(Vec2f(rho,theta));
    
    Point pt1(rho/cos(theta),0);//punto de interseccion de la linea con la primera fila
    Point pt2((rho-image.rows*sin(theta))/cos(theta),image.rows);//punto de interseccion de la linea con la ultima fila
    line(image,pt1,pt2,Scalar(0,0,255),thiningLines);
    indexVertical--;
    
  }
  
  while (!linesHorizontalDetected.empty()){
    currentLine = linesHorizontalDetected.front();
    lines_selected.push_back(currentLine);
    linesHorizontalDetected.pop_front();
    rho = currentLine[0];
    theta = currentLine[1];
    
    lines_selected.push_back(Vec2f(rho,theta));
    
    Point pt3(0,rho/sin(theta));//punto de interseccion de la linea con la primera columna
    Point pt4(image.cols,(rho-image.cols*cos(theta))/sin(theta));//punto de interseccion de la linea con la ultima columna
    line(image,pt3,pt4,Scalar(0,0,255),thiningLines);
    indexHorizontal--;
  }

  /*vector<Point2f> corners = listIntersect(lines_selected,gray_image.rows);
  for(int i = 0; i < corners.size(); i++){
    circle(image, corners.at(i), 3, CV_RGB(0,0,255), 2);
  }*/
  
  namedWindow("lineas",WINDOW_AUTOSIZE );
  imshow("lineas",image);
  waitKey();

}