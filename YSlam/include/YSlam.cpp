#include "include/YSlam.h"
#include "datastruct/Image.hpp"
#include "Tracker/Tracker.h"


namespace dan {

	static YSlam* instance = nullptr;

	Tracker* tracker = nullptr;

	YSlam::YSlam() {
	
	}
	YSlam::~YSlam() {
	
	}

	YSlam* YSlam::getInstance() {
		if (instance == nullptr) {
			instance = new YSlam;
		}

		return instance;
	}

	void YSlam::deleteInstance() {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}

		return;
	}

	bool YSlam::init(std::string dataPath, std::string parameterPath) {

		std::cout << "parameter Path : " <<  parameterPath << std::endl;
		Parameters::getInstance()->setParameters(parameterPath);

		tracker = Tracker::createTracker(TRACKER_TYPE);

		return true;
	}

	void YSlam::setNewFrame(Byte* data, int length, int width, int height, datastruct::ColorFormat format, unsigned long long int timestmap) {

		datastruct::ImagePtr image(new datastruct::Image());
		image->cvImage = cv::Mat(height, width, CV_8UC1);



		image->timestamp = timestmap;
		memcpy(image->cvImage.data, data, length);

		if (DATA_TYPE == DataType::EUROC) {
			image->cvImage = image->cvImage(cv::Rect(8, 0, 736, 480));
		}

		image->width = image->cvImage.cols;
		image->height = image->cvImage.rows;
		image->length = image->cvImage.cols * image->cvImage.rows;

		tracker->process(image);

	}
}