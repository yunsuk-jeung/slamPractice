#pragma once

#include <opencv2/opencv.hpp>



namespace datastruct {

class ImagePyramid {

public:

	ImagePyramid() {}
	~ImagePyramid() {}

	long long int timestamp;
	int level = 0;

	std::vector<cv::Mat> images;


private:

};

typedef std::shared_ptr<ImagePyramid> ImagePyramidPtr;

}
