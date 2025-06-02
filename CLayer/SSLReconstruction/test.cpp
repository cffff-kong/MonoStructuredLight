#include "SSLReconstruction.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <random>
#include <algorithm>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <pcl/common/transforms.h>
#include <Eigen/Geometry>
#include <unordered_set>

using namespace Eigen;

// 全局点集
std::vector<cv::Point3f> g_all_points;
std::vector<cv::Point3f> g_last_points;
// 哈希和比较定义，用于过滤重复点
struct Point3fHash
{
    std::size_t operator()(const cv::Point3f& p) const
    {
        return std::hash<float>()(p.x) ^ std::hash<float>()(p.y) ^ std::hash<float>()(p.z);
    }
};

struct Point3fEqual
{
    bool operator()(const cv::Point3f& a, const cv::Point3f& b) const
    {
        return std::abs(a.x - b.x) < 1e-6f &&
            std::abs(a.y - b.y) < 1e-6f &&
            std::abs(a.z - b.z) < 1e-6f;
    }
};

void computeRigidTransform(const std::vector<cv::Point3f>& src, const std::vector<cv::Point3f>& dst,
    Matrix3d& R, Vector3d& t)
{
    Vector3d centroid_src = Vector3d::Zero(), centroid_dst = Vector3d::Zero();
    for (const auto& p : src) centroid_src += Vector3d(p.x, p.y, p.z);
    for (const auto& p : dst) centroid_dst += Vector3d(p.x, p.y, p.z);
    centroid_src /= src.size();
    centroid_dst /= dst.size();

    MatrixXd X(3, src.size()), Y(3, dst.size());
    for (size_t i = 0; i < src.size(); ++i)
    {
        X.col(i) = Vector3d(src[i].x, src[i].y, src[i].z) - centroid_src;
        Y.col(i) = Vector3d(dst[i].x, dst[i].y, dst[i].z) - centroid_dst;
    }

    MatrixXd S = X * Y.transpose();
    JacobiSVD<MatrixXd> svd(S, ComputeFullU | ComputeFullV);
    R = svd.matrixV() * svd.matrixU().transpose();
    if (R.determinant() < 0)
    {
        Matrix3d V = svd.matrixV();
        V.col(2) *= -1;
        R = V * svd.matrixU().transpose();
    }
    t = centroid_dst - R * centroid_src;
}

void ransacPointCloudRegistration(
    const std::vector<cv::Point3f>& src_points,
    const std::vector<cv::Point3f>& dst_points,
    Matrix3d& best_R, Vector3d& best_t,
    std::vector<std::pair<cv::Point3f, cv::Point3f>>& matched_pairs,
    std::vector<double>& errors,
    double threshold = 2,
    int max_iterations = 1000000)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist_src(0, src_points.size() - 1);
    std::uniform_int_distribution<size_t> dist_dst(0, dst_points.size() - 1);

    size_t best_inliers = 0;
    std::vector<cv::Point3f> best_src_inliers, best_dst_inliers;

    for (int iter = 0; iter < max_iterations; ++iter)
    {
        std::vector<cv::Point3f> src_sample, dst_sample;
        for (int i = 0; i < 3; ++i)
        {
            size_t idx_src = dist_src(gen);
            size_t idx_dst = dist_dst(gen);
            src_sample.push_back(src_points[idx_src]);
            dst_sample.push_back(dst_points[idx_dst]);
        }

        Matrix3d R;
        Vector3d t;
        computeRigidTransform(src_sample, dst_sample, R, t);

        std::vector<cv::Point3f> src_inliers, dst_inliers;
        for (size_t i = 0; i < src_points.size(); ++i)
        {
            Vector3d transformed = R * Vector3d(src_points[i].x, src_points[i].y, src_points[i].z) + t;

            double min_error = std::numeric_limits<double>::max();
            size_t best_match = 0;
            for (size_t j = 0; j < dst_points.size(); ++j)
            {
                double error = (transformed - Vector3d(dst_points[j].x, dst_points[j].y, dst_points[j].z)).norm();
                if (error < min_error)
                {
                    min_error = error;
                    best_match = j;
                }
            }

            if (min_error < threshold)
            {
                src_inliers.push_back(src_points[i]);
                dst_inliers.push_back(dst_points[best_match]);
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
        Vector3d transformed = best_R * Vector3d(best_src_inliers[i].x, best_src_inliers[i].y, best_src_inliers[i].z) + best_t;
        Vector3d target(best_dst_inliers[i].x, best_dst_inliers[i].y, best_dst_inliers[i].z);
        double error = (transformed - target).norm();
        errors.push_back(error);
        matched_pairs.emplace_back(best_src_inliers[i], best_dst_inliers[i]);
    }
}

int main()
{
    static bool is_first = true;
    SSLReconstruction ssl(4, 2448, 2048, 100, 99, 90);
    for (int i = 1; i < 21; i++)
    {
        std::string path = "F:/Repo/MonoStructuredLight/build/x64/bin/Release/data";
        std::string filepath = path + "/" + std::to_string(i);

        ssl.m_point_queue.push(cv::imread(filepath + "/0.BMP", cv::IMREAD_GRAYSCALE));
        for (int j = 0; j < 12; j++)
        {
            std::string img_path = filepath + "/" + std::to_string(j + 1) + ".BMP";
            if (ssl.m_img_queue.size() >= 12) ssl.m_img_queue.pop();
            ssl.m_img_queue.push(cv::imread(img_path, cv::IMREAD_GRAYSCALE));
        }
        ssl.FindCentersPixel();
        ssl.FindCenters3D();
        std::cout << "points calculated done" << std::endl;

        Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();

        if (is_first)
        {
            g_all_points.insert(g_all_points.end(), ssl.m_points3D.begin(), ssl.m_points3D.end());
            g_last_points= ssl.m_points3D;
            is_first = false;
        }
        else
        {
            Matrix3d R;
            Vector3d t;
            std::vector<std::pair<cv::Point3f, cv::Point3f>> matched_pairs;
            std::vector<double> errors;

            ransacPointCloudRegistration(ssl.m_points3D, g_last_points, R, t, matched_pairs, errors);
            std::cout << "=== Transformation Matrix ===\n";
            std::cout << "Rotation:\n" << R << std::endl;
            std::cout << "Translation:\n" << t.transpose() << std::endl;

            transform.block<3, 3>(0, 0) = R.cast<float>();
            transform.block<3, 1>(0, 3) = t.cast<float>();

            std::unordered_set<cv::Point3f, Point3fHash, Point3fEqual> matched_set;
            for (const auto& pair : matched_pairs)
                matched_set.insert(pair.first);
            g_last_points.clear();
            for (auto& pt : ssl.m_points3D)
            {
                if (matched_set.count(pt)) continue;
                Eigen::Vector3d p(pt.x, pt.y, pt.z);
                Eigen::Vector3d p_transformed = R * p + t;
                pt.x = static_cast<float>(p_transformed.x());
                pt.y = static_cast<float>(p_transformed.y());
                pt.z = static_cast<float>(p_transformed.z());
                g_all_points.push_back(pt);
                g_last_points.push_back(pt);
            }

            std::cout << "pose calculated done" << std::endl;
        }

        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::io::loadPCDFile(filepath + "/" + std::to_string(i) + ".pcd", *cloud);
        std::cout << "pcd load done" << std::endl;

        pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::transformPointCloud(*cloud, *transformed_cloud, transform);
        pcl::io::savePCDFile(path + "/result/" + std::to_string(i) + ".pcd", *transformed_cloud);
        std::cout << "pcd saved done" << std::endl;
        std::cout << "---------------" << std::to_string(i) << " done -------------------" << std::endl;
    }

    return 0;
}
