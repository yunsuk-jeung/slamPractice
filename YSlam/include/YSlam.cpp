#include "include/YSlam.h"

#include "datastruct/Frame.h"
#include "datastruct/Image.hpp"

#include "Extractor/Extractor.h"
#include "Tracker/Tracker.h"

#include "Graph/Graph.h"

#include "flann/config.h"

namespace dan {

	static YSlam* instance = nullptr;

	Tracker* tracker = nullptr;
	Extractor* extractor = nullptr;
	Graph* graph = nullptr;

	YSlam::YSlam() {
	}

	YSlam::~YSlam() {
		if (tracker != nullptr) {
			delete tracker;
			tracker = nullptr;
		}

		if (extractor != nullptr) {
			delete extractor;
			extractor = nullptr;
		}

		if (graph != nullptr) {
			delete graph;
			graph = nullptr;
		}

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
		bool parameterSet = Parameters::getInstance()->setParameters(parameterPath);

		if (!parameterSet) {
			std::cout << "Parameter Setting fail" << std::endl;
			return false;
		}

		graph = new Graph();
		extractor = Extractor::createExtractor(TRACKER_TYPE);
		tracker = Tracker::createTracker(TRACKER_TYPE);
		tracker->setGraph(graph);

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

		Frame* frame = new Frame();
		frame->createImagePyramid(image);
		frame->createGradientPyramid();
		frame->createMagGradientPyramid();

		//extractor->process(frame);
		tracker->process(frame);

	}
}