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


struct boule{
    double distCochonnet;
    int radius;
    cv::Point centre;
   
    bool isCochonet = false;
   
};


int partitionnage(vector<boule>& temp, int deb, int end){
    int cpt = deb;
    double pivot = temp[deb].distCochonnet;
    int i;
    for(  i =deb+1 ; i<end; i++){
        if(temp[i].distCochonnet<=pivot){
            cpt++;
            iter_swap(temp.begin()+cpt, temp.begin()+i);
            
        }

    }
    iter_swap(temp.begin()+cpt, temp.begin()+deb);
    return cpt;
    
}

void tri_util(vector<boule>& temp, int deb, int end){
    if(deb<end){
        int pivot = partitionnage(temp, deb, end);
        tri_util(temp, deb, pivot-1);
        tri_util(temp, pivot+1, end);
    }
}

void tri_rapide(vector<boule>& boules){
    tri_util(boules, 0,(int)boules.size());
}


+(UIImage *) findCircle:(UIImage *) srcImage{
    
    
    Mat imageMat;
    Mat srcPur;
    
    UIImageToMat(srcImage, imageMat);
    UIImageToMat(srcImage, srcPur);
    
    
   
    Mat grayimg;
    //GaussianBlur(imageMat, imageMat, cv::Size(5,5), 1.5 ,1.5, BORDER_DEFAULT);
    medianBlur(imageMat, grayimg, 7);
    cvtColor(grayimg, grayimg, COLOR_BGR2GRAY);
    rotate(grayimg , grayimg, cv::ROTATE_90_CLOCKWISE);
    vector<Vec3f> circles;
    
       HoughCircles(grayimg, circles, HOUGH_GRADIENT, 1,
                    100,  // change this value to detect circles with different distances to each other
                    100, 30, 10, 50 // change the last two parameters
               // (min_radius & max_radius) to detect larger circles
                    
        );
    
    Vec3f cochonet;
    int minRadius=500;
    
    vector<boule> boules;
    
    for( size_t i = 0; i < circles.size(); i++ )
     {
         
         if(minRadius>circles[i][2]){
             printf("hola radius\n");
             minRadius = circles[i][2];
             cochonet = circles[i];
             
         }

     }
    
    
    boule cochon;
    cochon.radius = cochonet[2];
    cochon.isCochonet = true;
    cochon.centre = cv::Point(cochonet[0],cochonet[1]);
    cochon.distCochonnet = 0;
    boules.push_back(cochon);
    
    
    
    for( size_t i = 0; i < circles.size(); i++ ){
        
        if(circles[i][2] == cochon.radius){
            continue;
        }
        else
        {
        boule Currentboule;
            Currentboule.distCochonnet = cv::norm(cv::Point(cochonet[0],cochonet[1])-cv::Point(circles[i][0],circles[i][1]))
            -cochonet[2]-circles[i][2]; //distance du bord de la boule avec le bord du cochonet. pour prendre en comtpe la largeur des boules. 
            
        Currentboule.centre = cv::Point(circles[i][0],circles[i][1]);
            
        Currentboule.radius = circles[i][2];
        Currentboule.isCochonet = false;
        boules.push_back(Currentboule);
        }
    }
    
    rotate(imageMat, imageMat, cv::ROTATE_90_CLOCKWISE);
    tri_rapide(boules);
    int cpt =1;
    for(boule b : boules){
        
        
        if(!b.isCochonet && b.distCochonnet>0){
            std::cout << "nb : " << cpt << "\n";
            std::cout << b.distCochonnet << "\n";
            
            cv::putText(imageMat, to_string(cpt), cv::Point(b.centre.x+30,b.centre.y+30), FONT_HERSHEY_PLAIN, 5.0, Scalar(250,0,50,255),2.0);
            
            cv::circle( imageMat, b.centre, b.radius, Scalar(0,162,255,255), 4, LINE_AA);
            cv::circle( imageMat, b.centre, 1, Scalar(0,100,100,255), 8, LINE_AA);
            cv::line(imageMat, cv::Point(cochonet[0],cochonet[1]), cv::Point(b.centre), Scalar(0,0,0,255),3,LINE_AA);
            cpt++;
               
        }
        else
        {
            printf("cochon \n");
            cv::circle( imageMat, b.centre, b.radius, Scalar(0,255,0,255), 3, LINE_AA);
            cv::circle( imageMat, b.centre, 1, Scalar(0,100,100,255), 6, LINE_AA);
                     
           
        }
        
        
        
        
    }

    
    
    
    
    
    UIImage * processedImg = MatToUIImage(imageMat);
    
    
    return processedImg ;
   
    
}

@end
