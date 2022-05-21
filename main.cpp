#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void abs_sobel_thresh(InputArray image,
                      OutputArray t_gray,
                      char orient = 'x',
                      int sobel_kernel = 3);

void thresh(Mat &src, OutputArray dst, uchar from, uchar to);

void color_gradient_threshold(){

}

int main() {
    Mat image = imread("/home/a/Desktop/workspace/high-load-line/assets/image1.jpg");
    Mat gray;
    abs_sobel_thresh(image,
                     gray,
                     'x',
                     3
    );
    thresh(gray, gray, 150, 255);
    imshow("thresh x", gray);
    abs_sobel_thresh(image,
                     gray,
                     'y',
                     3
    );
    thresh(gray, gray, 150, 255);
    imshow("thresh y", gray);
    waitKey(0);
    return 0;
}


void abs_sobel_thresh(InputArray image,
                      OutputArray dst_gray,
                      char orient,
                      int sobel_kernel) {
    //计算X或Y方向的方向梯度
    //转化成灰度图像
    cvtColor(image, dst_gray, COLOR_BGR2GRAY);
    imshow("gray", dst_gray);
    // 求X或Y方向的方向梯度
    if ('x' == orient) {
        Sobel(
                dst_gray,
                dst_gray,
                CV_8U,
                1,
                0,
                sobel_kernel

        );
    }
    if ('y' == orient) {
        Sobel(
                dst_gray,
                dst_gray,
                CV_8U,
                0,
                1,
                sobel_kernel

        );
    }
    convertScaleAbs(dst_gray, dst_gray);
}

void thresh(Mat &src, OutputArray dst, uchar from, uchar to) {
    Mat zeros = Mat::zeros(src.size(), src.type());
    for (int y = 0; y < src.rows; ++y) {
        uchar *row = src.ptr(y);
        uchar *outRow = zeros.ptr(y);
        for (int x = 0; x < src.cols; ++x) {
            if (from <= row[x] && row[x] <= to) {
                outRow[x] = 255;
            }
        }
    }
    dst.assign(zeros);
}