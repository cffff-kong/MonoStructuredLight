#include "SSLReconstruction.h"

SSLReconstruction::SSLReconstruction(int ps_num, int img_width, int img_height, int T1, int T2, int T3)
{
	this->m_ps_num = ps_num;
	this->m_width = img_width;
	this->m_height = img_height;
	this->m_frequency = new int[3]{T1, T2, T3};
	m_T13 = T1 - T3;
	m_T23 = T2 - T3;
	m_T123 = m_T13 - m_T23;
	m_k_cam = (cv::Mat_<float>(3, 3) << 4917.83770754812, 0, 1214.55428929112,
			   0, 4923.82638685160, 1032.77712790899,
			   0, 0, 1);
	m_k_dlp = (cv::Mat_<float>(3, 3) << 1257.86693007908, 0, 473.175940813014,
			   0, 3154.36722942004, 810.775389044632,
			   0, 0, 1);
	m_rt_cam = (cv::Mat_<float>(3, 4) << 0.0282843801595056, 0.999453513215491, 0.0171075644147791, -167.273600072281,
				-0.991514508545440, 0.0258791135103581, 0.127394076894544, 48.7112427545775,
				0.126881729113720, -0.0205656608240837, 0.991704633654588, 1030.39786715420);
	m_rt_dlp = (cv::Mat_<float>(3, 4) << 0.0156316443525661, 0.998161200177202, -0.0585650933205363, -96.8716349512309,
				-0.992163655512276, 0.0227461190972831, 0.122857212838845, 30.6138996645475,
				0.123963431605260, 0.0561856968170766, 0.990694824402463, 950.178896112994);
}

void SSLReconstruction::loadImg()
{
	for (int i = 0; i < this->m_ps_num * 3; i++)
	{
		string path = m_path + "/" + to_string(i + 1) + ".bmp";
		cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
		m_imgs.push_back(img);
	}
}

cv::Mat SSLReconstruction::CalWrappedPhase(int index)
{
	vector<cv::Mat> imgs;
	for (int i = index * m_ps_num; i < index * m_ps_num + m_ps_num; i++)
	{
		imgs.push_back(m_img_queue.front());
		m_img_queue.pop();
		// imgs.push_back(m_imgs[i]);
	}
	cv::Mat sin_sum = cv::Mat::zeros(imgs[0].size(), CV_32F);
	cv::Mat cos_sum = cv::Mat::zeros(imgs[0].size(), CV_32F);

	for (int i = 0; i < this->m_ps_num; i++)
	{
		cv::Mat temp = imgs[i].clone(); // 使用 psImg[0] 的数据初始化 temp
		temp.convertTo(temp, CV_32FC1);
		float pk = 2 * i * CV_PI / m_ps_num;

		sin_sum += temp * sin(pk);
		cos_sum += temp * cos(pk);
	}
	cv::Mat pha(sin_sum.size(), CV_32F);
	for (int col = 0; col < m_width; col++)
	{
		for (int row = 0; row < m_height; row++)
		{
			pha.at<float>(row, col) = atan2(sin_sum.at<float>(row, col), cos_sum.at<float>(row, col));
		}
	}
	cv::Mat A = sin_sum.mul(sin_sum) + cos_sum.mul(cos_sum);
	sqrt(A, m_B);
	m_B = m_B * 2 / m_ps_num;
	m_B_mask = m_B > 10;
	m_B_mask.convertTo(m_B_mask, CV_32FC1);
	pha = -pha;
	cv::Mat pha_low_mask = pha <= 0;
	pha_low_mask.convertTo(pha_low_mask, CV_32FC1);
	pha = pha + pha_low_mask / 255 * CV_2PI;
	return pha;
}

cv::Mat SSLReconstruction::HeterodynePhase(cv::Mat phase1, int T1, cv::Mat phase2, int T2)
{
	int T12 = T1 - T2;
	float K = (float)T1 / float(T12);
	cv::Mat phase_diff = phase1 - phase2;
	cv::Mat index = phase_diff < 0;
	index.convertTo(index, CV_32FC1);
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			if (index.at<float>(row, col) > 0)
			{
				phase_diff.at<float>(row, col) += CV_2PI;
			}
		}
	}

	cv::Mat period = ((phase_diff * K - phase1) / (CV_2PI));
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			period.at<float>(row, col) = round(period.at<float>(row, col));
		}
	}
	cv::Mat pha = (period * CV_2PI + phase1) / K;

	return pha;
}

void SSLReconstruction::CombinePhase()
{
	// 计算绝对相位
	cv::Mat phase1 = m_phase123 * (float)(m_frequency[0]);

	// 计算 phase2 和 phase3，确保 round 逻辑与 Matlab 一致
	cv::Mat temp2 = (m_phase123 * (float)(m_frequency[1]) - m_wrapped_phase2) / CV_2PI;
	cv::Mat temp3 = (m_phase123 * (float)(m_frequency[2]) - m_wrapped_phase3) / CV_2PI;

	// 预分配和 m_phase123 相同大小的矩阵
	cv::Mat phase2 = cv::Mat::zeros(m_height, m_width, CV_32F);
	cv::Mat phase3 = cv::Mat::zeros(m_height, m_width, CV_32F);

	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			phase2.at<float>(row, col) = round(temp2.at<float>(row, col)) * CV_2PI + m_wrapped_phase2.at<float>(row, col);
			phase3.at<float>(row, col) = round(temp3.at<float>(row, col)) * CV_2PI + m_wrapped_phase3.at<float>(row, col);
		}
	}

	// 计算最终的绝对相位
	m_phase_abs = (phase1 + phase2 + phase3) / static_cast<float>(m_frequency[0] + m_frequency[1] + m_frequency[2]);
}
void SSLReconstruction::FilterPhase()
{
	// 滤波条件：对每个像素点：1.最大值大于245；2.最小值小于10；3.最大最小值差值小于10
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			int max_val = 0, min_val = 255;
			for (int i = 0; i < m_imgs.size(); i++)
			{

				if (m_imgs[i].at<uchar>(row, col) > max_val)
				{
					max_val = m_imgs[i].at<uchar>(row, col);
				}
				if (m_imgs[i].at<uchar>(row, col) < min_val)
				{
					min_val = m_imgs[i].at<uchar>(row, col);
				}
			}
			if (max_val > 253 || abs(max_val - min_val) <= 2 || min_val < 2)
			{
				m_phase_abs.at<float>(row, col) = 0;
			}
		}
	}
	// 因为无效点都设置的-100，如果直接归一化会让分母变得很大，进而导致每个元素都差不多，所以显示的时候基本都是纯白。
	// 为了正常显示，要除以整幅图里最大的值
}
void SSLReconstruction::saveToCSV(const cv::Mat &mat, const std::string &filename)
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	for (int i = 0; i < mat.rows; ++i)
	{
		for (int j = 0; j < mat.cols; ++j)
		{
			file << mat.at<float>(i, j);
			if (j < mat.cols - 1)
				file << ",";
		}
		file << "\n";
	}

	file.close();
	std::cout << "Saved to " << filename << " successfully." << std::endl;
}

cv::Mat SSLReconstruction::MonoFilter()
{
	cv::Mat mask = cv::Mat::zeros(m_phase_abs.size(), CV_32FC1);
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 1; col < m_width - 1; col++)
		{
			if (m_phase_abs.at<float>(row, col) > m_phase_abs.at<float>(row, col - 1) && m_phase_abs.at<float>(row, col) < m_phase_abs.at<float>(row, col + 1))
			{
				mask.at<float>(row, col) = 1;
			}
		}
	}
	return mask;
}

void SSLReconstruction::Mono()
{
	for (int row = 0; row < m_height; row++)
	{
		for (int col = 1; col < m_width - 1; col++)
		{
			if (col < 1 || col > m_width - 2 || row < 1 || row > m_height - 2)
			{
				m_phase_abs.at<float>(row, col) = -100;
			}
			else if (m_phase_abs.at<float>(row, col) != -100)
			{
				if (m_phase_abs.at<float>(row, col - 1) != -100)
				{
					if (
						m_phase_abs.at<float>(row, col) < m_phase_abs.at<float>(row, col - 1))
					{
						m_phase_abs.at<float>(row, col) = -100;
					}
				}
				if (m_phase_abs.at<float>(row, col + 1) != -100)
				{
					if (m_phase_abs.at<float>(row, col + 1) < m_phase_abs.at<float>(row, col))
					{
						m_phase_abs.at<float>(row, col) = -100;
					}
				}
				if (m_phase_abs.at<float>(row, col - 1) == 0 && m_phase_abs.at<float>(row, col + 1) == -100)
				{
					m_phase_abs.at<float>(row, col) = -100;
				}
				/*if (m_phase_abs.at<float>(row - 1, col) != -100)
				{
					if (m_phase_abs.at<float>(row, col) - m_phase_abs.at<float>(row - 1, col) > (CV_PI / m_width / 4))
					{
						m_phase_abs.at<float>(row, col) = -100;
					}
				}
				if (m_phase_abs.at<float>(row + 1, col) != -100)
				{
					if (m_phase_abs.at<float>(row + 1, col) - m_phase_abs.at<float>(row, col) > (CV_PI / m_width / 4))
					{
						m_phase_abs.at<float>(row + 1, col) = -100;
					}
				}*/
				if (m_phase_abs.at<float>(row - 1, col) == -100 && m_phase_abs.at<float>(row + 1, col) == -100)
				{
					m_phase_abs.at<float>(row, col) = -100;
				}
			}
		}
	}
}
cv::Mat SSLReconstruction::Decode(bool filter)
{
	m_wrapped_phase1 = CalWrappedPhase(0);
	m_wrapped_phase2 = CalWrappedPhase(1);
	m_wrapped_phase3 = CalWrappedPhase(2);
	m_phase13 = HeterodynePhase(m_wrapped_phase1, m_frequency[0], m_wrapped_phase3, m_frequency[2]);
	m_phase23 = HeterodynePhase(m_wrapped_phase2, m_frequency[1], m_wrapped_phase3, m_frequency[2]);
	m_phase123 = HeterodynePhase(m_phase13, m_T13, m_phase23, m_T23);
	CombinePhase();

	// FilterPhase();
	if (filter)
	{
		// 单调性滤波
		cv::Mat mask = MonoFilter();
		// 调制度滤波
		m_B_mask.convertTo(m_B_mask, CV_32FC1);
		for (int col = 0; col < m_width; col++)
		{
			for (int row = 0; row < m_height; row++)
			{
				if (m_B_mask.at<float>(row, col) == 0 || mask.at<float>(row, col) == 0)
				{
					m_phase_abs.at<float>(row, col) = 0;
				}
			}
		}
	}
	return m_phase_abs;
}

void SSLReconstruction::CloudPointFilter(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered)
{
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
	sor.setInputCloud(cloud);
	sor.setMeanK(50);
	sor.setStddevMulThresh(1.0);
	sor.filter(*cloud_filtered);
}

SSLReconstruction::~SSLReconstruction()
{
}

void SSLReconstruction::FindCentersPixel()
{
	if (m_point_queue.empty())
	{
		std::cout << "No points found in the image." << std::endl;
		return;
	}
	cv::Mat img = m_point_queue.front();
	// 二值化图像（如果尚未二值化）
	cv::Mat binaryImage;
	threshold(img, binaryImage, 100, 255, cv::THRESH_BINARY);
	/*cv::imshow("Binary Image", binaryImage);
	cv::waitKey(0);*/
	// 查找连通域
	cv::Mat labels, stats, centroids;
	int numLabels = connectedComponentsWithStats(binaryImage, labels, stats, centroids);
	
	// 设定筛选条件
	int minArea = 100;			 // 最小面积
	int maxArea = 3000;			 // 最大面积
	double minCircularity = 0.8; // 最小圆度

	// 创建一个彩色图像用于可视化
	cv::Mat outputImage;
	cvtColor(binaryImage, outputImage, cv::COLOR_GRAY2BGR);

	vector<cv::Point2f> centers;
	m_centers.clear();

	// 遍历所有连通域
	for (int i = 1; i < numLabels; i++)
	{ // 跳过背景（标签为0）
		int area = stats.at<int>(i, cv::CC_STAT_AREA);
		if (area < minArea || area > maxArea)
		{
			continue;
		}
		// 计算连通域的轮廓
		cv::Mat mask = (labels == i);
		vector<vector<cv::Point>> contours;
		findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		if (contours.empty())
			continue;

		// 计算周长
		double perimeter = arcLength(contours[0], true);

		// 计算圆度
		double circularity = (4 * CV_PI * area) / (perimeter * perimeter);

		// 筛选符合条件的连通域
		if (area > minArea && area < maxArea && circularity > minCircularity)
		{
			cout << "连通域 " << i << ": 面积 = " << area << ", 圆度 = " << circularity << endl;

			// 拟合椭圆
			cv::RotatedRect ellipse = fitEllipse(contours[0]);
			cv::Point2f center = ellipse.center; // 椭圆中心点
			m_centers.push_back(center);
			// 在图像上标记连通域
			 cv::Rect boundingBox(stats.at<int>(i, cv::CC_STAT_LEFT),
			 					 stats.at<int>(i, cv::CC_STAT_TOP),
			 					 stats.at<int>(i, cv::CC_STAT_WIDTH),
			 					 stats.at<int>(i, cv::CC_STAT_HEIGHT));
			 rectangle(outputImage, boundingBox, cv::Scalar(0, 255, 0), 2); // 用绿色矩形标记

			// 在图像上标记椭圆中心点
			 circle(outputImage, center, 5, cv::Scalar(255, 0, 0), -1); // 用蓝色圆点标记中心
			 putText(outputImage, "Center", cv::Point(center.x + 10, center.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1);
		}
		
	}
	/*cv::namedWindow("Output Image", cv::WINDOW_NORMAL);
	cv::imshow("Output Image", outputImage);
	cv::waitKey(0);*/
}

void SSLReconstruction::FindCenters3D()
{
	m_points3D.clear(); // 清空之前的 3D 点
	std::cout<< m_points3D.size()<<std::endl;
	Decode(false);
	cv::Mat phase_temp;
	medianBlur(m_phase_abs, phase_temp, 5); // 使用 5×5 的中值滤波
	cout << phase_temp.type() << endl;

	cv::Mat A(3, 3, CV_32FC1);
	cv::Mat b = cv::Mat(3, 1, CV_32FC1);
	cv::Mat xyz;
	cv::Mat m_q_cam = m_k_cam * m_rt_cam;
	cv::Mat m_q_dlp = m_k_dlp * m_rt_dlp;
	// 截断标识点二维坐标
	std::vector<cv::Point> points2D; // 存储截断后的 int 坐标

	for (const auto &pt : m_centers)
	{
		points2D.emplace_back(
			static_cast<int>(pt.x), // 直接截断小数部分
			static_cast<int>(pt.y));
	}
	for (const auto &pt : points2D)
	{
		int row = pt.y; // 行坐标（对应图像高度）
		int col = pt.x; // 列坐标（对应图像宽度）

		if (phase_temp.at<float>(row, col) > 0)
		{
			float up = phase_temp.at<float>(row, col) * 912 / (2 * CV_PI);
			float uc = col;
			float vc = row;

			// 计算 A 和 b
			A.at<float>(0, 0) = m_q_cam.at<float>(0, 0) - m_q_cam.at<float>(2, 0) * uc;
			A.at<float>(0, 1) = m_q_cam.at<float>(0, 1) - m_q_cam.at<float>(2, 1) * uc;
			A.at<float>(0, 2) = m_q_cam.at<float>(0, 2) - m_q_cam.at<float>(2, 2) * uc;

			A.at<float>(1, 0) = m_q_cam.at<float>(1, 0) - m_q_cam.at<float>(2, 0) * vc;
			A.at<float>(1, 1) = m_q_cam.at<float>(1, 1) - m_q_cam.at<float>(2, 1) * vc;
			A.at<float>(1, 2) = m_q_cam.at<float>(1, 2) - m_q_cam.at<float>(2, 2) * vc;

			A.at<float>(2, 0) = m_q_dlp.at<float>(0, 0) - m_q_dlp.at<float>(2, 0) * up;
			A.at<float>(2, 1) = m_q_dlp.at<float>(0, 1) - m_q_dlp.at<float>(2, 1) * up;
			A.at<float>(2, 2) = m_q_dlp.at<float>(0, 2) - m_q_dlp.at<float>(2, 2) * up;

			b.at<float>(0, 0) = m_q_cam.at<float>(2, 3) * uc - m_q_cam.at<float>(0, 3);
			b.at<float>(1, 0) = m_q_cam.at<float>(2, 3) * vc - m_q_cam.at<float>(1, 3);
			b.at<float>(2, 0) = m_q_dlp.at<float>(2, 3) * up - m_q_dlp.at<float>(0, 3);

			cv::solve(A, b, xyz);

			// 存储 3D 点
			m_points3D.emplace_back(
				xyz.at<float>(0, 0),
				xyz.at<float>(1, 0),
				xyz.at<float>(2, 0));
		}
	}
	for (const auto& pt : m_points3D)
	{
		pcl::PointXYZL pointsl;
		pointsl.x = pt.x;
		pointsl.y = pt.y;
		pointsl.z = pt.z;
		pointsl.label = 1;
		m_cloud_label->push_back(pointsl);
	}
	// 获取当前时间
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	// 将当前时间转换为 tm 结构
	std::tm tm_now = *std::localtime(&now_time);

	// 使用 stringstream 格式化时间字符串
	std::ostringstream oss;
	oss << std::put_time(&tm_now, "%Y%m%d_%H%M%S"); // 格式：20250119_143015
	pcl::io::savePCDFileASCII(oss.str() + ".pcd", *m_cloud_label);
	//SavePointsToTXT(points3D, "output_3d_points.txt");
}

void SSLReconstruction::SavePointsToTXT(const std::vector<cv::Point3f> &points3D, const std::string &filename)
{
	std::ofstream outFile(filename);
	if (!outFile.is_open())
	{
		std::cerr << "Error: Could not open file for writing!" << std::endl;
		return;
	}

	// 写入格式：x y z（每行一个点）
	for (const auto &pt : points3D)
	{
		outFile << pt.x << " " << pt.y << " " << pt.z << "\n";
	}

	outFile.close();
	std::cout << "3D points saved to " << filename << std::endl;
}

void SSLReconstruction::Reconstruction(bool is_save)
{
	cv::Mat phase = Decode();

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZL>::Ptr cloud_label(new pcl::PointCloud<pcl::PointXYZL>);
	cv::Mat A(3, 3, CV_32FC1);
	cv::Mat b = cv::Mat(3, 1, CV_32FC1);
	cv::Mat xyz;
	cv::Mat m_q_cam = m_k_cam * m_rt_cam;
	cv::Mat m_q_dlp = m_k_dlp * m_rt_dlp;

	// 重建
	for (int row = 1; row < m_height; row++)
	{
		for (int col = 1; col < m_width; col++)
		{
			if (phase.at<float>(row, col) > 0)
			{
				float up = phase.at<float>(row, col) * 912 / (2 * CV_PI);
				float uc = col;
				float vc = row;

				// A 的计算
				A.at<float>(0, 0) = m_q_cam.at<float>(0, 0) - m_q_cam.at<float>(2, 0) * uc;
				A.at<float>(0, 1) = m_q_cam.at<float>(0, 1) - m_q_cam.at<float>(2, 1) * uc;
				A.at<float>(0, 2) = m_q_cam.at<float>(0, 2) - m_q_cam.at<float>(2, 2) * uc;

				A.at<float>(1, 0) = m_q_cam.at<float>(1, 0) - m_q_cam.at<float>(2, 0) * vc;
				A.at<float>(1, 1) = m_q_cam.at<float>(1, 1) - m_q_cam.at<float>(2, 1) * vc;
				A.at<float>(1, 2) = m_q_cam.at<float>(1, 2) - m_q_cam.at<float>(2, 2) * vc;

				A.at<float>(2, 0) = m_q_dlp.at<float>(0, 0) - m_q_dlp.at<float>(2, 0) * up;
				A.at<float>(2, 1) = m_q_dlp.at<float>(0, 1) - m_q_dlp.at<float>(2, 1) * up;
				A.at<float>(2, 2) = m_q_dlp.at<float>(0, 2) - m_q_dlp.at<float>(2, 2) * up;

				// b 的计算
				b.at<float>(0, 0) = m_q_cam.at<float>(2, 3) * uc - m_q_cam.at<float>(0, 3);
				b.at<float>(1, 0) = m_q_cam.at<float>(2, 3) * vc - m_q_cam.at<float>(1, 3);
				b.at<float>(2, 0) = m_q_dlp.at<float>(2, 3) * up - m_q_dlp.at<float>(0, 3);

				cv::solve(A, b, xyz);

				pcl::PointXYZ points;
				points.x = xyz.at<float>(0, 0);
				points.y = xyz.at<float>(1, 0);
				points.z = xyz.at<float>(2, 0);
				cloud->push_back(points);

				pcl::PointXYZL pointsl;
				pointsl.x = xyz.at<float>(0, 0);
				pointsl.y = xyz.at<float>(1, 0);
				pointsl.z = xyz.at<float>(2, 0);
				pointsl.label =0;
				cloud_label->push_back(pointsl);

			}
		}
	}
	m_cloud_label=cloud_label;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filterd(new pcl::PointCloud<pcl::PointXYZ>);
	CloudPointFilter(cloud, cloud_filterd);

	// pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;
	// voxel_filter.setInputCloud(cloud_filterd);
	// voxel_filter.setLeafSize(0.5, 0.5, 0.5);
	// voxel_filter.filter(*cloud_filterd);

	m_cloud = cloud_filterd;  //直接赋值，不需要清空的操作

	if (is_save)
	{
		// 获取当前时间
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);

		// 将当前时间转换为 tm 结构
		std::tm tm_now = *std::localtime(&now_time);

		// 使用 stringstream 格式化时间字符串
		std::ostringstream oss;
		oss << std::put_time(&tm_now, "%Y%m%d_%H%M%S"); // 格式：20250119_143015
		pcl::io::savePCDFileASCII(oss.str() + ".pcd", *m_cloud_label);
	}
	cout << "点云生成完成" << endl;
}
