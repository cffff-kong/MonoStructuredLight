#include "PointCloudProcess.h"

PointCloudProcess::PointCloudProcess()
{
}

PointCloudProcess::~PointCloudProcess()
{
}

pcl::PointCloud<pcl::PointXYZL>::Ptr PointCloudProcess::MergePointCloud(std::string path)
{
	// 读取点云
	pcl::PointCloud<pcl::PointXYZL>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZL>);
	if (pcl::io::loadPCDFile<pcl::PointXYZL>(path, *cloud) == -1)
	{
		PCL_ERROR("Couldn't read file %s\n", path.c_str());
		return pcl::PointCloud<pcl::PointXYZL>::Ptr();
	}
	if (m_is_first)
	{
		m_is_first = false;
		ExtractLabelPoints(cloud);
		m_last_points = m_current_points; // 把当前的标识点坐标存下来给下一帧用
		SaveTransformedCloud(path, "result", cloud);

		return cloud;
	}
	ExtractLabelPoints(cloud);
	// 计算位姿变换
    Eigen::Matrix3d R;
	Eigen::Vector3d t;
    std::vector<std::pair<cv::Point3f, cv::Point3f>> matched_pairs;
    std::vector<double> errors;
	RansacPointCloudRegistration(R,t,matched_pairs,errors);
	// 应用变换
	Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();
	transform.block<3, 3>(0, 0) = R.cast<float>();
	transform.block<3, 1>(0, 3) = t.cast<float>();
	pcl::PointCloud<pcl::PointXYZL>::Ptr transformed_cloud(new pcl::PointCloud<pcl::PointXYZL>);
	pcl::transformPointCloud(*cloud, *transformed_cloud, transform);
	// 保存点云
	SaveTransformedCloud(path, "result", transformed_cloud);
	//对三维点进行变换
	m_last_points.clear();
	for (auto& pt : m_current_points)
	{
		Eigen::Vector3d p(pt.x, pt.y, pt.z);
		Eigen::Vector3d p_transformed = R * p + t;
		pt.x = static_cast<float>(p_transformed.x());
		pt.y = static_cast<float>(p_transformed.y());
		pt.z = static_cast<float>(p_transformed.z());
		m_last_points.push_back(pt);
	}
    return transformed_cloud;
}

bool PointCloudProcess::RansacPointCloudRegistration(Eigen::Matrix3d& best_R, Eigen::Vector3d& best_t,
	std::vector<std::pair<cv::Point3f, cv::Point3f>>& matched_pairs,
	std::vector<double>& errors,
	double threshold,
	int max_iterations)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> dist_src(0, m_current_points.size() - 1);
	std::uniform_int_distribution<size_t> dist_dst(0, m_last_points.size() - 1);

	size_t best_inliers = 0;
	std::vector<cv::Point3f> best_src_inliers, best_dst_inliers;

	for (int iter = 0; iter < max_iterations; ++iter)
	{
		std::vector<cv::Point3f> src_sample, dst_sample;
		for (int i = 0; i < 3; ++i)
		{
			size_t idx_src = dist_src(gen);
			size_t idx_dst = dist_dst(gen);
			src_sample.push_back(m_current_points[idx_src]);
			dst_sample.push_back(m_last_points[idx_dst]);
		}

		Eigen::Matrix3d R;
		Eigen::Vector3d t;
		ComputeRigidTransform(src_sample, dst_sample, R, t);

		std::vector<cv::Point3f> src_inliers, dst_inliers;
		for (size_t i = 0; i < m_current_points.size(); ++i)
		{
			Eigen::Vector3d transformed = R * Eigen::Vector3d(m_current_points[i].x, m_current_points[i].y, m_current_points[i].z) + t;

			double min_error = std::numeric_limits<double>::max();
			size_t best_match = 0;
			for (size_t j = 0; j < m_last_points.size(); ++j)
			{
				double error = (transformed - Eigen::Vector3d(m_last_points[j].x, m_last_points[j].y, m_last_points[j].z)).norm();
				if (error < min_error)
				{
					min_error = error;
					best_match = j;
				}
			}

			if (min_error < threshold)
			{
				src_inliers.push_back(m_current_points[i]);
				dst_inliers.push_back(m_last_points[best_match]);
			}
		}

		if (src_inliers.size() > best_inliers)
		{
			best_inliers = src_inliers.size();
			best_src_inliers = src_inliers;
			best_dst_inliers = dst_inliers;
			best_R = R;
			best_t = t;
		}
	}

	errors.clear();
	matched_pairs.clear();
	for (size_t i = 0; i < best_src_inliers.size(); ++i)
	{
		Eigen::Vector3d transformed = best_R * Eigen::Vector3d(best_src_inliers[i].x, best_src_inliers[i].y, best_src_inliers[i].z) + best_t;
		Eigen::Vector3d target(best_dst_inliers[i].x, best_dst_inliers[i].y, best_dst_inliers[i].z);
		double error = (transformed - target).norm();
		errors.push_back(error);
		matched_pairs.emplace_back(best_src_inliers[i], best_dst_inliers[i]);
	}
	return false;

}

void PointCloudProcess::ComputeRigidTransform(const std::vector<cv::Point3f> &src, const std::vector<cv::Point3f> &dst, Eigen::Matrix3d &R, Eigen::Vector3d &t)
{
	Eigen::Vector3d centroid_src = Eigen::Vector3d::Zero(), centroid_dst = Eigen::Vector3d::Zero();
	for (const auto& p : src) centroid_src += Eigen::Vector3d(p.x, p.y, p.z);
	for (const auto& p : dst) centroid_dst += Eigen::Vector3d(p.x, p.y, p.z);
	centroid_src /= src.size();
	centroid_dst /= dst.size();

	Eigen::MatrixXd X(3, src.size()), Y(3, dst.size());
	for (size_t i = 0; i < src.size(); ++i)
	{
		X.col(i) = Eigen::Vector3d(src[i].x, src[i].y, src[i].z) - centroid_src;
		Y.col(i) = Eigen::Vector3d(dst[i].x, dst[i].y, dst[i].z) - centroid_dst;
	}

	Eigen::MatrixXd S = X * Y.transpose();
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(S, Eigen::ComputeFullU | Eigen::ComputeFullV);
	R = svd.matrixV() * svd.matrixU().transpose();
	if (R.determinant() < 0)
	{
		Eigen::Matrix3d V = svd.matrixV();
		V.col(2) *= -1;
		R = V * svd.matrixU().transpose();
	}
	t = centroid_dst - R * centroid_src;
}
void PointCloudProcess::ExtractLabelPoints(pcl::PointCloud<pcl::PointXYZL>::Ptr cloud)
{
	m_current_points.clear();

	for (int i = static_cast<int>(cloud->size()) - 1; i >= 0; --i)
	{
		const auto& pt = cloud->points[i];
		if (pt.label == 1)
		{
			m_current_points.emplace_back(pt.x, pt.y, pt.z);
		}
		if (pt.label == 0)
		{
			return;
		}
	}
}

void PointCloudProcess::SaveTransformedCloud(const std::string& original_path, const std::string& save_path, pcl::PointCloud<pcl::PointXYZL>::Ptr transformed_cloud)
{
	fs::path file_path(original_path);
	fs::path parent_dir = file_path.parent_path();
	fs::path stem = file_path.stem(); // 不带扩展名的文件名
	fs::path result_dir = parent_dir / save_path;

	fs::create_directories(result_dir); // 自动创建 result 目录

	fs::path new_filename = stem.string() + "_transform.pcd";
	fs::path new_path = result_dir / new_filename;

	pcl::io::savePCDFile(new_path.string(), *transformed_cloud);
}
