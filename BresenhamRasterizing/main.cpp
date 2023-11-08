//
// Created by Kaleido on 3/21/23.
//
#include<opencv2/opencv.hpp>
#include<Windows.h>
#include<math.h>
#include"rasterizer.h"

int main() {

    int width = 700, height = 700;  //画布宽度和高度
    Vector2f origin(width / 2.f, height / 2.f);  //原点

    rst::Rasterizer r(width, height);
    long time0 = GetTickCount64();

    //任务1
    int length = 320;  //线长
    r.mission1(origin, length);  //执行任务
    //输出任务1图像
    cv::Mat image1(height, width, CV_32FC3, r.frame_buffer().data());
    image1.convertTo(image1, CV_8UC3, 1.0f);
    cv::cvtColor(image1, image1, cv::COLOR_RGB2BGR);
    cv::imshow("mission1", image1);

    long time1 = GetTickCount64();
    std::cout << "Mission 1 execution time:" << (time1 - time0) / 1000.f << "s" << std::endl;
    r.clear_buffer();

    //任务2
    int unit_length = 12;  //栅格大小可以调整：栅格单位长度
    int a = 25;  //栅格（网格）横纵坐标最大值，调节网格线数量
    r.mission2(a, unit_length);  //执行任务
    //输出任务2图像
    cv::Mat image2(height, width, CV_32FC3, r.frame_buffer().data());
    image2.convertTo(image2, CV_8UC3, 1.0f);
    cv::cvtColor(image2, image2, cv::COLOR_RGB2BGR);
    cv::imshow("mission2", image2);

    std::cout << "Mission 2 execution time:" << (GetTickCount64() - time1) / 1000.f << "s" << std::endl;
    cv::waitKey(0);
    return 0;
}