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
    
    rotate(imageMat, imageMat, cv::ROTATE_90_CLOCKWISE);
    
    
    UIImage * processedImg = MatToUIImage(imageMat);
    
    
    return processedImg ;
   
    
}

@end
