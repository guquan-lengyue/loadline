#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void abs_sobel_thresh(InputArray image,
                      OutputArray t_gray,
                      char orient = 'x',
                      int sobel_kernel = 3);

void show_image(const string &window_name, const Mat &image);

void color_thresh(Mat &src,
                  Mat &dst,
                  int l_thresh,
                  int v_thresh);

void rgb_select(Mat &src,
                Mat &dst,
                int r_thresh,
                int g_thresh);

void get_channel(Mat &src, Mat &channel, int channel_num);

void interesting_area(Mat &src, Mat &dst);


const static Size WINDOW_SIZE(512, 512);

int main() {

//    Mat image = imread("/home/a/Desktop/workspace/high-load-line/assets/image1.jpg");
    VideoCapture cap("/home/a/Desktop/workspace/high-load-line/assets/dirver.mp4");
    Mat image;
    namedWindow("origin", WINDOW_NORMAL);
    resizeWindow("origin", WINDOW_SIZE);
    namedWindow("color_thresh", WINDOW_NORMAL);
    resizeWindow("color_thresh", WINDOW_SIZE);
    while (cap.read(image)) {
        imshow("origin", image);

        Mat color_thresh_rst;
        color_thresh(
                image,
                color_thresh_rst,
                74,
                70);


        Mat bgr_thresh_rst;
        rgb_select(image,
                   bgr_thresh_rst,
                   70,
                   60);
        Mat gaus_rst, grad_x;
        int kernel = 5;
        GaussianBlur(image,
                     gaus_rst,
                     Size(5, 5),
                     1,
                     1);
        abs_sobel_thresh(image, grad_x, 'x', kernel);
        Mat combine = color_thresh_rst & bgr_thresh_rst & grad_x;
        Mat interesting_area_image;
        interesting_area(combine,interesting_area_image);
        show_image("interesting_area_image", interesting_area_image);

        waitKey(10);
    }
    return 0;
}


void abs_sobel_thresh(InputArray image,
                      OutputArray dst_gray,
                      char orient,
                      int sobel_kernel) {
    //计算X或Y方向的方向梯度
    //转化成灰度图像
    cvtColor(image, dst_gray, COLOR_BGR2GRAY);
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


void get_channel(Mat &src, Mat &channel, int channel_num) {
    Mat cvt;
    vector<Mat> channels;
    cv::split(src, channels);
    channels[channel_num].copyTo(channel);
}


void color_thresh(Mat &src,
                  Mat &dst,
                  int l_thresh,
                  int v_thresh) {
    Mat hls, hsv;
    cvtColor(src, hls, COLOR_BGR2HLS);
    cvtColor(src, hsv, COLOR_BGR2HSV);
    get_channel(hls, hls, 1);
    get_channel(hsv, hsv, 2);
    threshold(hls, hls, l_thresh, 255, THRESH_BINARY);
    threshold(hsv, hsv, v_thresh, 255, THRESH_BINARY);
    dst = hls & hsv;
}

void interesting_area(Mat &src, Mat &dst) {
    Mat mask = Mat::zeros(src.size(), src.type());
    Point points[5];
    points[0] = Point(0, mask.rows * 0.7);
    points[1] = Point(0, mask.rows);
    points[2] = Point(mask.cols, mask.rows);
    points[3] = Point(mask.cols * 0.55, mask.rows * 0.5);
    points[4] = Point(mask.cols * 0.4, mask.rows * 0.5);
    fillConvexPoly(
            mask,
            points,
            5,
            Scalar(255)
    );
    bitwise_and(mask, src, dst);
}

void show_image(const string &window_name, const Mat &image) {
    namedWindow(window_name, WINDOW_NORMAL);
    resizeWindow(window_name, WINDOW_SIZE);
    imshow(window_name, image);
}

void rgb_select(Mat &src,
                Mat &dst,
                int r_thresh,
                int g_thresh) {
    Mat g, r;

    get_channel(src, g, 1);
    get_channel(src, r, 2);

    threshold(g, g, g_thresh, 255, THRESH_BINARY);
    threshold(r, r, r_thresh, 255, THRESH_BINARY);
    dst = r & g;
}