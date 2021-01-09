#include <iostream>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include<pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
// 定义点云类型
typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;

// 相机内参
const double camera_factor = 500;
const double camera_cx = 300;//325.5;
const double camera_cy = 250;//253.5;
const double camera_fx = 500;//518.0;
const double camera_fy = 500;//519.0;

// 主函数
int main(int argc, char** argv)
{
	// 读取./data/rgb.png和./data/depth.png，并转化为点云

	// 图像矩阵
	cv::Mat rgb, depth;
	// 使用cv::imread()来读取图像
	// API: http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html?highlight=imread#cv2.imread
	rgb = cv::imread("0002.png");
	// rgb 图像是8UC3的彩色图像
	// depth 是16UC1的单通道图像，注意flags设置-1,表示读取原始数据不做任何修改
	depth = cv::imread("0000.png", -1);

	// 点云变量
	// 使用智能指针，创建一个空点云。这种指针用完会自动释放。
	PointCloud::Ptr cloud(new PointCloud);
	// 遍历深度图
	for (int m = 0; m < depth.rows; m++)
		for (int n = 0; n < depth.cols; n++)
		{
			// 获取深度图中(m,n)处的值
			ushort d = depth.ptr<ushort>(m)[n];
			// d 可能没有值，若如此，跳过此点
			if (d == 0)
				continue;
			// d 存在值，则向点云增加一个点
			PointT p;

			// 计算这个点的空间坐标
			p.z = double(d) / camera_factor;
			p.x = (n - camera_cx) * p.z / camera_fx;
			p.y = (m - camera_cy) * p.z / camera_fy;

			// 从rgb图像中获取它的颜色
			// rgb是三通道的BGR格式图，所以按下面的顺序获取颜色
			p.b = rgb.ptr<uchar>(m)[n * 3];
			p.g = rgb.ptr<uchar>(m)[n * 3 + 1];
			p.r = rgb.ptr<uchar>(m)[n * 3 + 2];

			// 把p加入到点云中
			cloud->points.push_back(p);
		}
	// 设置并保存点云
	cloud->height = 1;
	cloud->width = cloud->points.size();
	std::cout << "point cloud size = " << cloud->points.size() << std::endl;
	cloud->is_dense = false;
	pcl::io::savePCDFile("pointcloud1.pcd", *cloud);
	// 清除数据并退出
	cloud->points.clear();
	std::cout << "Point cloud saved." << std::endl;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr c (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPCDFile<pcl::PointXYZRGB>("pointcloud1.pcd", *c);
	pcl::visualization::PCLVisualizer viewer("Sift keypoint");
	viewer.setBackgroundColor(255, 255, 255);
	viewer.addPointCloud(c, "cloud");
	while (!viewer.wasStopped())
	{
		viewer.spinOnce();
	}
	c->points.clear();
	return 0;
}