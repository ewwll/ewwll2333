#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
    Mat img1 = imread("right01.jpg");
    Mat img2 = imread("left01.jpg");
    Mat img3 = imread("right02.jpg");
    Mat img4 = imread("left02.jpg");
    Mat img_1, img_2, img_3, img_4;
    vector<Point2f>k1, k2, k3, k4;
    vector<vector<Point2f>>p;
    findChessboardCorners(img1, Size(9, 6), k1);
    findChessboardCorners(img2, Size(9, 6), k2);
    findChessboardCorners(img3, Size(9, 6), k3);
    findChessboardCorners(img4, Size(9, 6), k4);
    cvtColor(img1, img_1, COLOR_RGB2GRAY);
    find4QuadCornerSubpix(img_1, k1, Size(11, 11));
    cvtColor(img2, img_2, COLOR_RGB2GRAY);
    find4QuadCornerSubpix(img_2, k1, Size(11, 11));
    cvtColor(img3, img_3, COLOR_RGB2GRAY);
    find4QuadCornerSubpix(img_3, k1, Size(11, 11));
    cvtColor(img4, img_4, COLOR_RGB2GRAY);
    find4QuadCornerSubpix(img_4, k1, Size(11, 11));
    TermCriteria criteria=TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,40,0.01);
    cornerSubPix(img_1, k1, cv::Size(11, 11), cv::Size(-1, -1),criteria);
    cornerSubPix(img_2, k2, cv::Size(11, 11), cv::Size(-1, -1), criteria);
    cornerSubPix(img_3, k3, cv::Size(11, 11), cv::Size(-1, -1), criteria);
    cornerSubPix(img_4, k4, cv::Size(11, 11), cv::Size(-1, -1), criteria);
    Mat cm, dc;
    vector<Mat>rv, tv;
    p.push_back(k1);
    p.push_back(k2);
    p.push_back(k3);
    p.push_back(k4);
    vector<vector<Point3f>>p3;
    for (auto i : p)
    {
        vector<Point3f>p;
        for (auto j : i)
        {
            Point3f d3;
            d3.z = 0;
            d3.x = j.x * 30;
            d3.y = j.y * 30;
            p.push_back(d3);
        }
        p3.push_back(p);
    }
    cout << calibrateCamera(p3, p, Size(img1.rows, img1.cols), cm, dc, rv, tv) << endl;;
    cout << "camerMatric = " << cm << endl;
    cout << "distCoeffs  = " << dc << endl;
    for (auto i = 0; i < rv.size(); i++)
    {
        cout << "第" << i << "图像的旋转矩阵： " << rv[i] << endl;
        cout << "第" << i << "图像的平移矩阵： " << tv[i] << endl;
    }
    drawChessboardCorners(img1, Size(9, 6), k1, true);
    drawChessboardCorners(img2, Size(9, 6), k2, true);
    drawChessboardCorners(img3, Size(9, 6), k3, true);
    drawChessboardCorners(img4, Size(9, 6), k4, true);
    //solvePnP(p3, Points2D, camera_matrix, distortion_coefficients, rvec, tvec, false, CV_P3P);
    Mat image1 = imread("0002.png");
    Mat image2 = imread("0012.png");
    shared_ptr<ORB>orb = ORB::create();
    vector<KeyPoint>kp1, kp2;
    orb->detect(image1, kp1);
    orb->detect(image2, kp2);
    Mat des1, des2,res1;
    orb->compute(image1, kp1, des1);
    orb->compute(image2, kp2, des2);
    vector<DMatch>matcher;
    BFMatcher Matches;
    Matches.match(des1, des2, matcher);
    drawMatches(image1, kp1, image2, kp2, matcher, res1,Scalar(0,0,255),Scalar(255,0,0));
    imshow("result", res1);
    imshow("img1", img1);
    imshow("img2", img2);
    imshow("img3", img3);
    imshow("img4", img4);
    waitKey(0);
    return 0;
}