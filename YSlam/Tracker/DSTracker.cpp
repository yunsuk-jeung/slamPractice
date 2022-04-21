#include <opencv2/opencv.hpp>

#include "Tracker/DSTracker.h"
#include "datastruct/Frame.h"

#include "Extractor/Extractor.h"
#include "Extractor/GridPixelExtractor.h"

#include "Parameters/Parameters.h"

namespace dan {
	DSTracker::DSTracker() {
		std::cout << "Tracker is DSTracker" << std::endl;

		extractor = Extractor::createExtractor(TRACKER_TYPE);

	}
	DSTracker::~DSTracker() {
		delete extractor;
	}

	void DSTracker::process(datastruct::ImagePtr image) {
		
		cv::imshow("test", image->cvImage);
		cv::waitKey(1);


		Frame frame;
		frame.createImagePyramid(image);
		frame.createGradientPyramid();


	




	}

}