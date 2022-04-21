#include "Parameters/Parameters.h"
#include "opencv2/opencv.hpp"

namespace dan {

static Parameters* instance = nullptr;

TrackerType TRACKER_TYPE = TrackerType::OPTICAL_FLOW;

DataType DATA_TYPE = DataType::EUROC;

int PYRAMID_LEVEL = 0;

Parameters::Parameters() {}

Parameters::~Parameters() {}

Parameters* Parameters::getInstance() {
	if (instance == nullptr) {
		instance = new Parameters();
	}

	return instance;
}

void Parameters::deleteInstance() {
	if (instance != nullptr) {
		delete instance;
	}
	return;
}

bool Parameters::setParameters(std::string parameterPath) {
	
	cv::FileStorage fsSettings(parameterPath, cv::FileStorage::READ);

	if (!fsSettings.isOpened()) {
		std::cout << "Parameter file Path is wrong" << std::endl;
		return false;
	}

	int val = 0;;
	cv::FileNode data(fsSettings["DataType"]);

	val = (int)data;
	DATA_TYPE = static_cast<DataType>(val);


	cv::FileNode tracker(fsSettings["TrackerType"]);
	val = (int)tracker;
	TRACKER_TYPE = static_cast<TrackerType>(val);

	cv::FileNode pyramidLevel(fsSettings["PyramidLevel"]);
	PYRAMID_LEVEL = (int)pyramidLevel;
	

	std::cout << "TrackerType : " << (int)DATA_TYPE << std::endl;
	std::cout << "TrackerType : " << (int)TRACKER_TYPE << std::endl;
	std::cout << "PyramidLevel : "<< PYRAMID_LEVEL << std::endl;
	return true;
}


}