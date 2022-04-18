#include "Tracker/DSTracker.h"
#include "datastruct/Frame.h"
#include <opencv2/opencv.hpp>

namespace dan {
	DSTracker::DSTracker() {
		std::cout << "Tracker is DSTracker" << std::endl;
	}
	DSTracker::~DSTracker() {}

	void DSTracker::process(datastruct::ImagePtr image) {
		
		cv::imshow("test", image->cvImage);
		cv::waitKey();


		Frame frame;
		frame.createImagePyramid(image);





	}

}