//
//  Wrapper.m
//  LaPetanque
//
//  Created by Raphael Braud-Mussi on 08/11/2021.
//


#import "Wrapper.h"
#import "opencv2/opencv.hpp"
#import <opencv2/imgcodecs/ios.h>
#import <UIKit/UIKit.h>
using namespace cv;
using namespace std;




@implementation Wrapper

+(UIImage *) findCircle:(UIImage *) srcImage{
    
    
    Mat imageMat;
    
    UIImageToMat(srcImage, imageMat);
    
    
    
    
    Mat grayimg;
    
    cvtColor(imageMat, grayimg, COLOR_BGR2GRAY);
    
    medianBlur(grayimg, grayimg, 31);
    vector<Vec3f> circles;
       HoughCircles(grayimg, circles, HOUGH_GRADIENT, 1,
                    50,  // change this value to detect circles with different distances to each other
                    100, 30, 25, 0 // change the last two parameters
               // (min_radius & max_radius) to detect larger circles
       );
    
    
    
    for( size_t i = 0; i < circles.size(); i++ )
     {
         
         Vec3i c = circles[i];
         cv::Point center = cv::Point(c[0], c[1]);
         // circle center
         cv::circle( imageMat, center, 1, Scalar(0,100,100,255), 1, LINE_AA);
         // circle outline
         int radius = c[2];
         cv::circle( imageMat, center, radius, Scalar(0,0,255,255), 1, LINE_AA);
     }
    
    //rotate(imageMat, imageMat, cv::ROTATE_90_CLOCKWISE);
    
    
    UIImage * processedImg = MatToUIImage(imageMat);
    
    
    return processedImg ;
   
    
}

@end
