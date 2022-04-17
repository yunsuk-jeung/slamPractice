#pragma once

#include <opencv2/opencv.hpp>

namespace dan {
namespace datastruct {

class Image {

public:
	long long int timestamp;
	cv::Mat cvImage;

	int length = 0;
	int width = 0;
	int height = 0;

private:

};

typedef std::shared_ptr<Image> ImagePtr;

}
}