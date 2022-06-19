#pragma once

#include <opencv2/opencv.hpp>

#include <Eigen/Eigen>

namespace dan
{

class Camera
{
public:
	Camera();
	~Camera();

	Camera(const Camera &rhs);

	void setParameters(
		int width, int height,
		double fx, double fy, double cx, double cy,
		double k1 = 0.0, double k2 = 0.0, double p1 = 0.0, double p2 = 0.0, double k3 = 0.0);

	void rescaleParameter(int scale);

	void changeParameters(float* camIntrinsic);

	void setExtrinsicCamToIMU(const Eigen::Matrix3d &Ric, const Eigen::Vector3d &Tic);

	void setStereoParameters(cv::Mat R, cv::Mat P);

	void projection(const Eigen::Vector3d &input, Eigen::Vector2d &output);

	//equal to undistortion
	void unprojection(const Eigen::Vector2d &input, Eigen::Vector3d &output);

	//if P is null, points are set in normalized coordinate
	void undistortPoints(const std::vector<cv::Point2f> &input, std::vector<cv::Point2f> &output,
						 Eigen::Matrix3d *P = nullptr, Eigen::Matrix3d *R = nullptr);

	void distortion(const Eigen::Vector2d &input, Eigen::Vector2d &output);

	bool saveToFilePointer(FILE* file);
	bool loadFromFilePointer(FILE* file);

	bool operator==(const Camera &other) const;

	Camera &operator=(const Camera &rhs);

	int width;
	int height;

	double fx;
	double fy;
	double cx;
	double cy;
	double k1;
	double k2;
	double p1;
	double p2;

	double inv_fx;
	double inv_fy;
	double inv_cx;
	double inv_cy;

	bool isDistortion;

	Eigen::Matrix3d Ric;
	Eigen::Vector3d Tic;

	cv::Mat K;
	cv::Mat D;

	//for stereoParameter
	cv::Mat R;
	cv::Mat P;
	Eigen::Matrix3d eigenR;
	Eigen::Matrix3d eigenP;
	double baseline;
};

}