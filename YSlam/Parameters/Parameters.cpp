#include "Parameters/Parameters.h"
#include "opencv2/opencv.hpp"

namespace dan {

	static Parameters* instance = nullptr;

	TrackerType TRACKER_TYPE = TrackerType::OPTICAL_FLOW;

	DataType DATA_TYPE = DataType::EUROC;
	int PYRAMID_LEVEL = 0;

	int X_GRID_NUM = 0;
	int Y_GRID_NUM = 0;

	int X_PIXEL_SEARCH_NUM = 0;
	int Y_PIXEL_SEARCH_NUM = 0;

	int SHOW_THRESHOLD_IMAGE = 0;
	int SHOW_ORIGIN_FEATURE = 0;
	int SHOW_PYRAMID_FEATRUE = 0;

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


		cv::FileNode xGridNum(fsSettings["xGridNum"]);
		X_GRID_NUM = (int)xGridNum;

		cv::FileNode yGridNum(fsSettings["yGridNum"]);
		Y_GRID_NUM = (int)yGridNum;

		cv::FileNode xPixelSearchNum(fsSettings["xPixelSearchNum"]);
		X_PIXEL_SEARCH_NUM = (int)xPixelSearchNum;
		 
		cv::FileNode yPixelSearchNum(fsSettings["yPixelSearchNum"]);
		Y_PIXEL_SEARCH_NUM = (int)yPixelSearchNum;

		/*Debug Obtion*/

		cv::FileNode Debug(fsSettings["Debug"]);
		int debugOn = (int)Debug;
		
		if (debugOn == 1) {
			cv::FileNode show_threshold(fsSettings["show_threshold"]);
			SHOW_THRESHOLD_IMAGE = (int)show_threshold;

			cv::FileNode show_feature(fsSettings["show_origin_feature"]);
			SHOW_ORIGIN_FEATURE = (int)show_feature;

			cv::FileNode show_pyramid_feature(fsSettings["show_pyramid_feature"]);
			SHOW_PYRAMID_FEATRUE = (int)show_pyramid_feature;


		}

		

		std::cout << "TrackerType : " << (int)DATA_TYPE << std::endl;
		std::cout << "TrackerType : " << (int)TRACKER_TYPE << std::endl;
		std::cout << "PyramidLevel : " << PYRAMID_LEVEL << std::endl;
		std::cout << "x Grid Num : " << X_GRID_NUM << std::endl;
		std::cout << "y Grid Num : " << Y_GRID_NUM << std::endl;
		std::cout << "Show threshold image : " << SHOW_THRESHOLD_IMAGE << std::endl;
		std::cout << "Show Origin Feature : " << SHOW_ORIGIN_FEATURE << std::endl;
		std::cout << "Show Pyramid Feature : " << SHOW_PYRAMID_FEATRUE << std::endl;
		return true;
	}


}