
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

//structure deffinissant une boule
struct boule{
    double distCochonnet;
    int radius;
    cv::Point centre;
   
    bool isCochonet = false;
   
};


//algorithme de tri
int partitionnage(vector<boule>& temp, int deb, int end){
    int cpt = deb;
    double pivot = temp[deb].distCochonnet;
    int i;
    for(  i =deb+1 ; i<end; i++){
        if(temp[i].distCochonnet<pivot){
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
    
    medianBlur(imageMat, grayimg, 15);
    cvtColor(grayimg, grayimg, COLOR_BGR2GRAY);
    rotate(grayimg , grayimg, cv::ROTATE_90_CLOCKWISE);
    // boucle qui lisse les pixels trop blanc et reduit les rend plus gris
    for(int i=0; i<grayimg.rows; i++){
        for(int j=0; j<grayimg.cols; j++){
            if(grayimg.at<uchar>(i,j) >200 ){
                grayimg.at<uchar>(i,j) = 150;
            }
        }
    }
        
    vector<Vec3f> circles;
        //transformé de hough qui permet de detecter les cercle dans l'image
    //parm1 : grayimg, image sur la quelle la transformé va s'appliquer
    //parm2 : circles, liste de Vec3 liste de cercle
    //parm3 :  HOUGH_GRADIENT_ALT, type de transformé utilisant le filtre de Scharr
    //param4 : 1, echelle de l'image (1 = inchangé)
    //param5 : 200, seuil max pour la detection de canny (Min = Max/2)
    //param6 : 0.8,  taux de perfection d'un cercle (1= cercle parfait)
    //param7 : 15, rayon minimum d'un cercle
    //param8 : 60, rayon maximym d'un cercle
       HoughCircles(grayimg, circles, HOUGH_GRADIENT_ALT, 1,
                    100,
                    200, 0.80, 15, 60);
    
    
    
    Vec3f cochonet;
    int minRadius=500;
    vector<boule> boules;
    //recherche du plus petit cercle (cochonet)
    for( size_t i = 0; i < circles.size(); i++ )
     {
         
         if(minRadius>circles[i][2]){
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
    
    
    //calcule des distance exacte entre les boules et le cochonet
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
    //tri des boule en fonction de leur distance.
    tri_rapide(boules);
    int cpt =1;
    
    //affichage des boules sur l'image et suppresion des eventuels cercle fantome.
    for(boule b : boules){
        
        
        if(!b.isCochonet && b.distCochonnet>0){
            std::cout << "nb : " << cpt << "\n";
            std::cout << b.distCochonnet << "\n";
            //std::cout << b.radius << "\n";
           
            if(b.radius<20|| b.radius>40){
            
            cv::putText(imageMat, to_string(cpt), cv::Point(b.centre.x+70,b.centre.y+70), FONT_HERSHEY_PLAIN, 5.0, Scalar(250,0,50,255),2.0);
            cv::circle( imageMat, b.centre, b.radius, Scalar(0,162,255,255), 4, LINE_AA);
            cv::circle( imageMat, b.centre, 1, Scalar(0,100,100,255), 8, LINE_AA);
            cv::line(imageMat, cv::Point(cochonet[0],cochonet[1]), cv::Point(b.centre), Scalar(0,0,0,255),3,LINE_AA);
            cpt++;
            }else{continue;}
          
            
            
               
        }
        else
        {
            std::cout << b.radius << "\n";
            cv::circle( imageMat, b.centre, b.radius, Scalar(0,255,0,255), 3, LINE_AA);
            cv::circle( imageMat, b.centre, 1, Scalar(0,100,100,255), 6, LINE_AA);
                     
           
        }
    
        
    }

    
    UIImage * processedImg = MatToUIImage(imageMat);
    
    
    return processedImg ;
   
    
}

@end
