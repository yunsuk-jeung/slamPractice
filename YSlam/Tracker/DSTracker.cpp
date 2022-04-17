#include "Tracker/DSTracker.h"

namespace dan {
	DSTracker::DSTracker() {
		std::cout << "Tracker is DSTracker" << std::endl;
	}
	DSTracker::~DSTracker() {}

	void DSTracker::process(datastruct::ImagePtr image) {
		cv::imshow("test", image->cvImage);
		cv::waitKey();
	}

}