#pragma once

#include <opencv2/opencv.hpp>


namespace dan {
namespace datastruct {

class ImagePyramid {

public:
	long long int timestamp;

	std::vector<cv::Mat> iamges;


private:

};

typedef std::shared_ptr<ImagePyramid> ImagePtr;

}
}