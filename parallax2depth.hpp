#include<iostream>
#include<opencv2/opencv.hpp>

void disp2Depth(cv::Mat dispMap, cv::Mat depthMap)
{

    int type = dispMap.type();

//    float fx = K.at<float>(0, 0);
//    float fy = K.at<float>(1, 1);
//    float cx = K.at<float>(0, 2);
//    float cy = K.at<float>(1, 2);
    float fx = 3329.13;
    float fy = 3326.41;
    float cx = 1971.41;
    float cy = 1478.27;
    float baseline = 1; // To get relative depth map
    std::cout << dispMap.type() << std::endl;
    if (type == CV_8U)
    {
        const float PI = 3.14159265358;
        int height = dispMap.rows;
        int width = dispMap.cols;
        uchar* dispData = (uchar*)dispMap.data;
        ushort* depthData = (ushort*)depthMap.data;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int id = i * width + j;
                if (!dispData[id])  continue;  // Prevent 0 from divisor
                depthData[id] = ushort( (float)fx * baseline / ((float)dispData[id]) );
            }
        }
    }
    else
    {
        std::cout << "please confirm dispImg's type!" << std::endl;
        cv::waitKey(0);
    }
}
