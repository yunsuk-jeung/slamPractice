#include "Tracker/OFTracker.h"

namespace dan {
	OFTracker::OFTracker() {
		std::cout << "Tracker is OFTracker" << std::endl;
	}
	OFTracker::~OFTracker() {}

	void OFTracker::process(datastruct::ImagePtr image) {
		cv::imshow("test", image->cvImage);
		cv::waitKey();
	}

}