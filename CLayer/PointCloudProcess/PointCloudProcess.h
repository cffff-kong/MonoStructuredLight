#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/common/transforms.h>
#include <boost/thread/thread.hpp>
#include <pcl/filters/voxel_grid.h>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <random>
#include <algorithm>
#include <iomanip>
#include <filesystem>
namespace fs = std::filesystem;

#ifdef POINTCLOUDPROCESS_EXPORTS
#define POINTCLOUDPROCESS_API __declspec(dllexport)
#else
#define POINTCLOUDPROCESS_API __declspec(dllimport)
#endif

class POINTCLOUDPROCESS_API PointCloudProcess
{
public:
    PointCloudProcess();
    ~PointCloudProcess();
    /// @brief 点云配准入口函数
    /// @param path 点云路径，具体到.pcd
    /// @return 旋转之后的点云
    pcl::PointCloud<pcl::PointXYZL>::Ptr RegistrationPointCloud(std::string path);

    private:
    /// @brief Ransac求解点云配准矩阵
    /// @param best_R 最优的R
    /// @param best_t 最优的t
    /// @param matched_pairs 匹配的点对
    /// @param errors 误差
    /// @param threshold 距离阈值
    /// @param max_iterations 最大迭代次数
    /// @return 
    bool RansacPointCloudRegistration(Eigen::Matrix3d& best_R, Eigen::Vector3d& best_t,
        std::vector<std::pair<cv::Point3f, cv::Point3f>>& matched_pairs,
        std::vector<double>& errors,
        double threshold = 2,
        int max_iterations = 1000000);
    
    /// @brief 计算RT矩阵
    /// @param src 源点集
    /// @param dst 目标点集
    /// @param R 
    /// @param t 
    void PointCloudProcess::ComputeRigidTransform(const std::vector<cv::Point3f>& src, const std::vector<cv::Point3f>& dst, Eigen::Matrix3d& R, Eigen::Vector3d& t);

    /// @brief 提取label为1的点
    /// @param  输入的点云
    void ExtractLabelPoints(pcl::PointCloud<pcl::PointXYZL>::Ptr cloud);

    /// @brief 保存变换后的点云
    /// @param original_path 点云路径，abc/xxx.pcd
    /// @param save_path 保存路径，abc/save_path
    /// @param transformed_cloud 变换后的点云
    void SaveTransformedCloud(const std::string& original_path, const std::string& save_path,pcl::PointCloud<pcl::PointXYZL>::Ptr transformed_cloud);
public:
    bool m_is_first = true; //第一次调用标识符
private:
    std::vector<cv::Point3f> m_current_points; //当前帧的标识点坐标
    std::vector<cv::Point3f> m_last_points;    //上一帧的标识点坐标

};

