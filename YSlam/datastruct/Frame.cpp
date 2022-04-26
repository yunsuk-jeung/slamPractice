#include "datastruct/Frame.h"
#include "Parameters/Parameters.h"

namespace dan{
	Frame::Frame() {}
	Frame::~Frame() {}

	void Frame::createImagePyramid(datastruct::ImagePtr imagePtr) {
		
		imagePyramid.timestamp = imagePtr->timestamp;
		imagePyramid.level = PYRAMID_LEVEL;
		//imagePyramid.images.reserve(PYRAMID_LEVEL);

		cv::Mat image;
		image = imagePtr->cvImage.clone();

		if (DATA_TYPE == DataType::EUROC) {
			image = image(cv::Rect(8, 0, 736, 480)); 
		}

		//imagePyramid.images.push_back(image);

		for (int i = 0; i < PYRAMID_LEVEL; i++) {
			imagePyramid.images.push_back(image);
			cv::pyrDown(image, image);
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

		yGradientPyramid.timestamp = imagePyramid.timestamp;
		yGradientPyramid.level = PYRAMID_LEVEL;


		for (int i = 0; i <  PYRAMID_LEVEL; i++) {
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

		//	cv::hconcat(xgrad, ygrad, xgrad);

		//	cv::imshow("grad " + std::to_string(i), xgrad/255.0);
		//	cv::waitKey();
		//}

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

}