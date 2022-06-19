#include "Camera/Camera.h"

//#include "Parameters.h"

#include <stdio.h>

#include <opencv2/core/eigen.hpp>

using namespace std;

namespace dan
{

Camera::Camera() {
	int width = 0;
	int height = 0;

	double fx = 0.0;
	double fy = 0.0;
	double cx = 0.0;
	double cy = 0.0;
	double k1 = 0.0;
	double k2 = 0.0;
	double p1 = 0.0;
	double p2 = 0.0;

	double inv_fx = 0.0;
	double inv_fy = 0.0;
	double inv_cx = 0.0;
	double inv_cy = 0.0;

	bool isDistortion = false;

	Eigen::Matrix3d Ric = Eigen::Matrix3d::Identity();
	Eigen::Vector3d Tic = Eigen::Vector3d::Identity();

	cv::Mat K = cv::Mat(3, 3, CV_64FC1);
	cv::Mat D = cv::Mat(5, 1, CV_64FC1);

	//for stereoParameter
	cv::Mat R = cv::Mat(3, 3, CV_64FC1);
	cv::Mat P = cv::Mat(3, 3, CV_64FC1);
	Eigen::Matrix3d eigenR;
	Eigen::Matrix3d eigenP;
	double baseline;
}

Camera::~Camera() {

}

Camera::Camera(const Camera &rhs)
{
	width = rhs.width;
	height = rhs.height;
	fx = rhs.fx;
	fy = rhs.fy;
	cx = rhs.cx;
	cy = rhs.cy;
	k1 = rhs.k1;
	k2 = rhs.k2;
	p1 = rhs.p1;
	p2 = rhs.p2;

	inv_fx = rhs.inv_fx;
	inv_fy = rhs.inv_fy;
	inv_cx = rhs.inv_cx;
	inv_cy = rhs.inv_cy;

	isDistortion = rhs.isDistortion;

	Ric = rhs.Ric;
	Tic = rhs.Tic;

	K = rhs.K.clone();
	D = rhs.D.clone();

	R = rhs.R.clone();
	P = rhs.P.clone();
	eigenR = rhs.eigenR;
	eigenP = rhs.eigenP;
	baseline = rhs.baseline;
}

void Camera::setParameters(
	int _width, int _height,
	double _fx, double _fy, double _cx, double _cy,
	double _k1, double _k2, double _p1, double _p2, double k3) {

	width = _width;
	height = _height;
	fx = _fx;
	fy = _fy;
	cx = _cx;
	cy = _cy;

	k1 = _k1;
	k2 = _k2;
	p1 = _p1;
	p2 = _p2;

	if (_k1 != 0 || _k2 != 0 || _p1 != 0 || _p2 != 0) {
		isDistortion = true;
	}
	else {
		isDistortion = false;
	}

	inv_fx = 1.0 / fx;
	inv_fy = 1.0 / fy;
	inv_cx = -cx / fx;
	inv_cy = -cy / fy;

	K = cv::Mat(3, 3, CV_64FC1);
	cv::setIdentity(K);
	K.at<double>(0, 0) = fx;
	K.at<double>(1, 1) = fy;
	K.at<double>(0, 2) = cx;
	K.at<double>(1, 2) = cy;

	D = cv::Mat(5, 1, CV_64FC1);
	D.at<double>(0, 0) = k1;
	D.at<double>(1, 0) = k2;
	D.at<double>(2, 0) = p1;
	D.at<double>(3, 0) = p2;
	D.at<double>(4, 0) = k3;
}

void Camera::rescaleParameter(int scale) {
	width /= scale;
	height /= scale;
	fx /= scale;
	fy /= scale;
	cx /= scale;
	cy /= scale;

	inv_fx = 1.0 / fx;
	inv_fy = 1.0 / fy;
	inv_cx = -cx / fx;
	inv_cy = -cy / fy;

	K.at<double>(0, 0) = fx;
	K.at<double>(1, 1) = fy;
	K.at<double>(0, 2) = cx;
	K.at<double>(1, 2) = cy;
}

void Camera::changeParameters(float* camIntrinsic) {

	fx = camIntrinsic[0];
	fy = camIntrinsic[1];
	cx = camIntrinsic[2];
	cy = camIntrinsic[3];

	inv_fx = 1.0 / fx;
	inv_fy = 1.0 / fy;
	inv_cx = -cx / fx;
	inv_cy = -cy / fy;

	K.at<double>(0, 0) = fx;
	K.at<double>(1, 1) = fy;
	K.at<double>(0, 2) = cx;
	K.at<double>(1, 2) = cy;

}


void Camera::setExtrinsicCamToIMU(const Eigen::Matrix3d& _Ric, const Eigen::Vector3d& _Tic) {

	Ric = _Ric;
	Tic = _Tic;
}

void Camera::setStereoParameters(cv::Mat inputR, cv::Mat inputP) {

	R = inputR.clone();
	P = inputP.clone();

	cv::cv2eigen(R, eigenR);
	cv::Mat P2 = P(cv::Rect(0, 0, 3, 3)).clone();
	cv::cv2eigen(P2, eigenP);
	baseline = P.at<double>(0, 3);
}

void Camera::projection(const Eigen::Vector3d& input, Eigen::Vector2d& output) {

	Eigen::Vector2d p(input.x() / input.z(), input.y() / input.z());

	if (isDistortion == true) {
		Eigen::Vector2d dp;
		distortion(p, dp);
		p = p + dp;
	}

	output << fx * p.x() + cx,
		fy * p.y() + cy;
}

void Camera::unprojection(const Eigen::Vector2d& input, Eigen::Vector3d& output) {
	//double lambda;

	// Lift points to normalised plane
	Eigen::Vector2d np;
	np.x() = inv_fx * input.x() + inv_cx;
	np.y() = inv_fy * input.y() + inv_cy;

	if (isDistortion == true) {

		// Recursive distortion model
		int n = 8;
		Eigen::Vector2d d_u;
		Eigen::Vector2d np2;
		distortion(np, d_u);
		np2 = np - d_u;
		// Approximate value
		for (int i = 1; i < n; i++) {
			distortion(np2, d_u);
			np2 = np - d_u;
		}

		np = np2;
	}

	// Obtain a projective ray
	output << np.x(), np.y(), 1.0;
}

void Camera::undistortPoints(const vector<cv::Point2f>& input, vector<cv::Point2f>& output, Eigen::Matrix3d* P, Eigen::Matrix3d* R) {
	if (input.size() == 0) {
		return;
	}

	/*if (R != nullptr) {

		cv::Mat eigenToCVR;

		cv::eigen2cv(*R, eigenToCVR);

		if (P != nullptr) {
			cv::Mat eigenToCVP;
			cv::eigen2cv(*P, eigenToCVR);
			cv::undistortPoints(input, output, K, D, eigenToCVR, eigenToCVP);
		}


		cv::undistortPoints(input, output, K, D, eigenToCVR);
	}
	else {
		cv::undistortPoints(input, output, K, D);
	}
	*/

	const int pointSize = input.size();
	output.resize(pointSize);

	vector<Eigen::Vector3d> unprojPoints;
	unprojPoints.resize(pointSize);

	for (int i = 0; i < pointSize; i++) {
		unprojection(Eigen::Vector2d(input[i].x, input[i].y), unprojPoints[i]);
	}

	bool isSetMultiplyMatrix = false;
	Eigen::Matrix3d multiplyMatrix;
	multiplyMatrix.setIdentity();

	if (R != nullptr) {
		multiplyMatrix = *R;
		isSetMultiplyMatrix = true;
	}

	if (P != nullptr) {
		multiplyMatrix = *P * multiplyMatrix;
		isSetMultiplyMatrix = true;
	}


	if (isSetMultiplyMatrix == true) {
		for (int i = 0; i < pointSize; i++) {
			Eigen::Vector3d result = multiplyMatrix * unprojPoints[i];
			output[i].x = result.x() / result.z();
			output[i].y = result.y() / result.z();
		}
	}
	else {
		for (int i = 0; i < pointSize; i++) {
			output[i].x = unprojPoints[i].x();
			output[i].y = unprojPoints[i].y();
		}
	}
}

void Camera::distortion(const Eigen::Vector2d& input, Eigen::Vector2d& output) {
	double mx2_u, my2_u, mxy_u, rho2_u, rad_dist_u;

	mx2_u = input.x() * input.x();
	my2_u = input.y() * input.y();
	mxy_u = input.x() * input.y();

	rho2_u = mx2_u + my2_u;

	rad_dist_u = k1 * rho2_u + k2 * rho2_u * rho2_u;

	output << input.x() * rad_dist_u + 2.0 * p1 * mxy_u + p2 * (rho2_u + 2.0 * mx2_u),
		input.y() * rad_dist_u + 2.0 * p2 * mxy_u + p1 * (rho2_u + 2.0 * my2_u);
}

bool Camera::operator==(const Camera &other) const {
	return (width == other.width) && (height == other.height)
		&& (fx == other.fx) && (fy == other.fy) && (cx == other.cx) && (cy == other.cy)
		&& (k1 == other.k1) && (k2 == other.k2) && (p1 == other.p1) && (p2 == other.p2);
}

Camera& Camera::operator=(const Camera &rhs)
{
	width = rhs.width;
	height = rhs.height;
	fx = rhs.fx;
	fy = rhs.fy;
	cx = rhs.cx;
	cy = rhs.cy;
	k1 = rhs.k1;
	k2 = rhs.k2;
	p1 = rhs.p1;
	p2 = rhs.p2;

	inv_fx = rhs.inv_fx;
	inv_fy = rhs.inv_fy;
	inv_cx = rhs.inv_cx;
	inv_cy = rhs.inv_cy;

	isDistortion = rhs.isDistortion;

	Ric = rhs.Ric;
	Tic = rhs.Tic;

	K = rhs.K.clone();
	D = rhs.D.clone();

	R = rhs.R.clone();
	P = rhs.P.clone();
	eigenR = rhs.eigenR;
	eigenP = rhs.eigenP;
	baseline = rhs.baseline;

	return *this;
}

bool Camera::saveToFilePointer(FILE* file)
{
	fwrite(&width, sizeof(int), 1, file);
	fwrite(&height, sizeof(int), 1, file);
	fwrite(&fx, sizeof(double), 1, file);
	fwrite(&fy, sizeof(double), 1, file);
	fwrite(&cx, sizeof(double), 1, file);
	fwrite(&cy, sizeof(double), 1, file);

	fwrite(&k1, sizeof(double), 1, file);
	fwrite(&k2, sizeof(double), 1, file);
	fwrite(&p1, sizeof(double), 1, file);
	fwrite(&p2, sizeof(double), 1, file);
	return true;
}

bool Camera::loadFromFilePointer(FILE* file)
{
	fread(&width, sizeof(int), 1, file);
	fread(&height, sizeof(int), 1, file);
	fread(&fx, sizeof(double), 1, file);
	fread(&fy, sizeof(double), 1, file);
	fread(&cx, sizeof(double), 1, file);
	fread(&cy, sizeof(double), 1, file);

	fread(&k1, sizeof(double), 1, file);
	fread(&k2, sizeof(double), 1, file);
	fread(&p1, sizeof(double), 1, file);
	fread(&p2, sizeof(double), 1, file);

	if (k1 != 0 || k2 != 0 || p1 != 0 || p2 != 0) {
		isDistortion = true;
	}
	else {
		isDistortion = false;
	}

	inv_fx = 1.0 / fx;
	inv_fy = 1.0 / fy;
	inv_cx = -cx / fx;
	inv_cy = -cy / fy;

	K = cv::Mat(3, 3, CV_64FC1);
	cv::setIdentity(K);
	K.at<double>(0, 0) = fx;
	K.at<double>(1, 1) = fy;
	K.at<double>(0, 2) = cx;
	K.at<double>(1, 2) = cy;
	D = cv::Mat(5, 1, CV_64FC1);
	D.at<double>(0, 0) = k1;
	D.at<double>(1, 0) = k2;
	D.at<double>(2, 0) = p1;
	D.at<double>(3, 0) = p2;
	D.at<double>(4, 0) = 0.0;

	return true;
}

}