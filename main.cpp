#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "parallax2depth.hpp"
#include "readflow.hpp"
#include <GSLAM/core/GSLAM.h>

cv::Mat getFakeFlow(cv::Mat depth,GSLAM::SE3 T,GSLAM::Camera camera){
    cv::Mat r(depth.rows,depth.cols,CV_32FC2);
    for(int y=0;y<depth.rows;y++)
        for(int x=0;x<depth.cols;x++){
            float d=depth.at<float>(cv::Point(x,y));
            GSLAM::Point2f f=camera.Project(T*camera.UnProject(x,y)*d)-GSLAM::Point2d(x,y);
            r.at<GSLAM::Point2f>(cv::Point(x,y))=f;
        }
    return r;
}

cv::Mat flow2idepth(cv::Mat flow,GSLAM::SE3 T,GSLAM::Camera camera){
    cv::Mat depth(flow.rows,flow.cols,CV_32FC1);
    auto k=camera.getParameters();
    float fx=k[2];
    float fy=k[3];
    float cx=k[4];
    float cy=k[5];
    auto  t=T.get_translation();
    auto  r=T.get_rotation();
    double kf2ref[12];
    T.getMatrix(kf2ref);
    for(int y=0;y<depth.rows;y++)
        for(int x=0;x<depth.cols;x++){
            GSLAM::Point2f p1(x,y);
            GSLAM::Point2f f=flow.at<GSLAM::Point2f>(cv::Point(x,y));
            GSLAM::Point2f p2=p1+f;
//            if(fabs(f.x)>fabs(f.y)){
                depth.at<float>(cv::Point(x,y))=fx * t.x / f.x;
//            }
//            else{

//            }

        }
    return depth;
}


int main(int argc,char** argv)
{
//    svar.ParseMain(argc,argv);
    cv::Mat depth=cv::imread(svar.GetString("DepthImage","/mnt/PI_Lab/users/zhaoyong/Dataset/TUM/RGBD/rgbd_dataset_freiburg1_xyz/depth/1305031102.160407.png"));
//    depth.convertTo(depth,CV_32F,1./5000);
    GSLAM::Camera cam({depth.cols,depth.rows,500,500,depth.cols/2,depth.rows/2});
    GSLAM::SE3 T=GSLAM::SE3(GSLAM::SO3(),GSLAM::Point3d(1,0,0));
//    cv::Mat flow=getFakeFlow(depth,T,cam);

//    std::string flow2save=svar.GetString("Flow2Save","");
//    if(flow2save.size()){
//        flowrite(flow2save,flow);
//    }

//    cv::Mat depthEstimate=flow2idepth(flow,T,cam);
//    cv::imshow("depth",depth);
//    cv::imshow("depthEst",depthEstimate);
//    cv::waitKey(0);
//    return 0;

//    cv::Mat dispMap = cv::imread("../dispMaps.png");
    cv::Mat dispMap;
    floread("/home/heyu/code/ex/parallax2depth/build/0flow.flo",dispMap);
    auto depthMap=flow2idepth(dispMap,T,cam);
//    float maxDepth=0;
//    for(float* f=(float*)depthMap.data,*fend=((float*)depthMap.data)+depthMap.total();f<fend;f++)
//    {
//      if(*f>maxDepth) maxDepth=*f;
//    }
//    cv::Mat depthMap(dispMap.rows, dispMap.cols, CV_16UC1);

//    disp2Depth(dispMap, depthMap);

    cv::imshow("depthMap", depthMap/1000);
    cv::waitKey(0);

    return 0;
}
