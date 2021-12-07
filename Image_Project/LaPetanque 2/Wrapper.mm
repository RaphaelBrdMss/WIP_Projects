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
#import <chrono>

using namespace cv;
using namespace std;




@implementation Wrapper

+(UIImage *) findCircle:(UIImage *) srcImage{
    
    
    Mat imageMat;
    
    UIImageToMat(srcImage, imageMat);
    
    
    
    
    Mat grayimg;
    
    cvtColor(imageMat, grayimg, COLOR_BGR2GRAY);
    auto start = chrono::high_resolution_clock::now();
    
    medianBlur(grayimg, grayimg, 11);
    auto stop = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    
    cout << "Time taken by function: "
            << duration.count() << " microseconds" << endl;
    
    vector<Vec3f> circles;
    
       HoughCircles(grayimg, circles, HOUGH_GRADIENT, 1,
                    100,  // change this value to detect circles with different distances to each other
                    100, 30, 25, 100 // change the last two parameters
               // (min_radius & max_radius) to detect larger circles
       );
    
    

    
    for( size_t i = 0; i < circles.size(); i++ )
     {
         
         Vec3i c = circles[i];
         cv::Point center = cv::Point(c[0], c[1]);
        
         cv::circle( imageMat, center, 1, Scalar(0,100,100,255), 8, LINE_AA);
         
         int radius = c[2];
         cv::circle( imageMat, center, radius, Scalar(0,0,255,255), 5, LINE_AA);
         
         if(i == 15){
             break;
         }
     }
    rotate(grayimg, grayimg, cv::ROTATE_90_CLOCKWISE);
    rotate(imageMat, imageMat, cv::ROTATE_90_CLOCKWISE);
    
    
    UIImage * processedImg = MatToUIImage(imageMat);
    
    
    return processedImg ;
   
    
}

@end
