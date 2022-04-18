#include "datastruct/Frame.h"
#include "Parameters/Parameters.h"

namespace dan{
	Frame::Frame() {}
	Frame::~Frame() {}

	void Frame::createImagePyramid(datastruct::ImagePtr imagePtr) {
		
		imagePyramid.timestamp = imagePtr->timestamp;
		imagePyramid.level = PYRAMID_LEVEL;
		imagePyramid.images.reserve(PYRAMID_LEVEL);

		cv::Mat image;
		image = imagePtr->cvImage.clone();

		imagePyramid.images.push_back(image);

		for (int i = 0; i < PYRAMID_LEVEL; i++) {
			cv::pyrDown(image, image);
			imagePyramid.images.push_back(image);

		}

		for (int i = 0; i < PYRAMID_LEVEL; i++) {
			cv::imshow( " " + std::to_string(i), imagePyramid.images[PYRAMID_LEVEL - i]);
			cv::waitKey();
		}

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

		for (int i = 0; PYRAMID_LEVEL; i++) {
			cv::Mat xgrad;
			cv::Mat ygrad;

			cv::Mat floatImage;
			imagePyramid.images[i].convertTo(floatImage, CV_32FC1);

			cv::filter2D(floatImage, xgrad, -1, xKernal);
			cv::filter2D(floatImage, ygrad, -1, yKernal);

			xGradientPyramid.images[i].push_back(xgrad);
			yGradientPyramid.images[i].push_back(ygrad);

		}

		for (int i = 0; i < PYRAMID_LEVEL; i++) {
			cv::Mat xgrad = xGradientPyramid.images[i];
			cv::Mat ygrad = yGradientPyramid.images[i];

			cv::hconcat(xgrad, ygrad, xgrad);

			cv::imshow("grad " + std::to_string(i), xgrad);
			cv::waitKey();
		}

	}

}