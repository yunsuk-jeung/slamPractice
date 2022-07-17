#include "Graph/Frame.h"
#include "Parameters/Parameters.h"
#include "datastruct/Pose.h"

Frame::Frame() {}
Frame::~Frame() {}

void Frame::setCamera(Camera& _cam) {
	cam = _cam;
}

Camera& Frame::getCamera() {
	return cam;
}

void Frame::createImagePyramid(datastruct::ImagePtr imagePtr) {

	imagePyramid.timestamp = imagePtr->timestamp;
	imagePyramid.level = PYRAMID_LEVEL;
	//imagePyramid.images.reserve(PYRAMID_LEVEL);

	imagePyramid.images.reserve(PYRAMID_LEVEL);

	cv::Mat image;
	image = imagePtr->cvImage.clone();

	//imagePyramid.images.push_back(image);

	for (int i = 0; i < PYRAMID_LEVEL; i++) {
		imagePyramid.images.push_back(image);
		cv::pyrDown(image, image);
	}

	features.resize(PYRAMID_LEVEL);

	for (int i = 0; i < features.size(); i++) {
		features[i].reserve(400);
	}

	//for (int i = 0; i < PYRAMID_LEVEL; i++) {
	//	cv::Mat test;
	//	test = imagePyramid.images[i];
	//	cv::imshow("test" + std::to_string(i) , test);
	//	cv::waitKey(-1);
	//}

}

void Frame::createGradientPyramid() {
	float kernal[] = { -0.5f, 0.0f, 0.5f };

	cv::Mat xKernal(1, 3, CV_32FC1, kernal);
	cv::Mat yKernal(3, 1, CV_32FC1, kernal);

	xGradientPyramid.timestamp = imagePyramid.timestamp;
	xGradientPyramid.level = PYRAMID_LEVEL;
	xGradientPyramid.images.reserve(PYRAMID_LEVEL);

	yGradientPyramid.timestamp = imagePyramid.timestamp;
	yGradientPyramid.level = PYRAMID_LEVEL;
	yGradientPyramid.images.reserve(PYRAMID_LEVEL);


	for (int i = 0; i < PYRAMID_LEVEL; i++) {
		cv::Mat xgrad = cv::Mat(imagePyramid.images[i].size(), CV_32FC1);
		cv::Mat ygrad = cv::Mat(imagePyramid.images[i].size(), CV_32FC1);;

		cv::Mat floatImage;

		imagePyramid.images[i].convertTo(floatImage, CV_32FC1);

		cv::filter2D(floatImage, xgrad, -1, xKernal);
		cv::filter2D(floatImage, ygrad, -1, yKernal);

		xGradientPyramid.images.push_back(xgrad);
		yGradientPyramid.images.push_back(ygrad);
	}

	//for (int i = 0; i < PYRAMID_LEVEL; i++) {
	//	cv::Mat xgrad = xGradientPyramid.images[i];
	//	cv::Mat ygrad = yGradientPyramid.images[i];
	//	cv::Mat display;
	//	cv::hconcat(xgrad, ygrad, display);
	//	cv::imshow("grad " + std::to_string(i), display /255.0);
	//	cv::waitKey();
	//}

}

void Frame::createMagGradientPyramid() {
	magGradientPyramid.level = imagePyramid.level;
	magGradientPyramid.level = imagePyramid.timestamp;
	magGradientPyramid.images.reserve(PYRAMID_LEVEL);

	int pyrlvl = imagePyramid.level;


	for (int i = 0; i < pyrlvl; i++) {
		float* dIdx = (float*)(xGradientPyramid.images[i].data);
		float* dIdy = (float*)(yGradientPyramid.images[i].data);

		cv::Mat magImage = cv::Mat(imagePyramid.images[i].size(), CV_32FC1);
		float* mag = (float*)magImage.data;

		int length = xGradientPyramid.images[i].cols * xGradientPyramid.images[i].rows;
		for (int j = 0; j < length; j++) {
			mag[j] = sqrt(dIdx[j] * dIdx[j] + dIdy[j] * dIdy[j]);
		}

		magGradientPyramid.images.push_back(magImage);
	}
}

int Frame::getPyramidLevel() {
	return imagePyramid.level;
}

datastruct::ImagePyramid* Frame::getImagePyramid() {
	return &imagePyramid;
}

datastruct::ImagePyramid* Frame::getXGradPyramid() {
	return &xGradientPyramid;
}

datastruct::ImagePyramid* Frame::getYGradPyramid() {
	return &yGradientPyramid;
}

datastruct::ImagePyramid* Frame::getMagGradientPyramid() {
	return &magGradientPyramid;
}

datastruct::Pose* Frame::getPwc() {
	return &Pwc;
}

void Frame::getCeresParameter(double* pose) {
	Eigen::Vector3d& position = Pwc.position;
	Eigen::Quaterniond& rotation = Pwc.rotation;


	pose[0] = position.x();
	pose[1] = position.y();
	pose[2] = position.z();

	pose[3] = rotation.x();
	pose[4] = rotation.y();
	pose[5] = rotation.z();
	pose[6] = rotation.w();
}

void Frame::setCeresParameter(double* pose) {
	Eigen::Vector3d& position = Pwc.position;
	Eigen::Quaterniond& rotation = Pwc.rotation;

	position.x() = pose[0];
	position.y() = pose[1];
	position.z() = pose[2];

	rotation.x() = pose[3];
	rotation.y() = pose[4];
	rotation.z() = pose[5];
	rotation.w() = pose[6];
}


