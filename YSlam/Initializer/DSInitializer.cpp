#include <opencv2/opencv.hpp>

#include "Extractor/GridPixelExtractor.h"

#include "Tracker/DSTracker.h"

#include "Graph/Frame.h"
#include "Graph/MapPoint.h"

#include "Graph/Graph.h"

#include "Initializer/DSInitializer.h"
#include "Parameters/Parameters.h"

#include "Optimizer/Optimizer.h"

DSInitializer::DSInitializer() {

	extractor = Extractor::createExtractor(TRACKER_TYPE);
}

DSInitializer* DSInitializer::createInitializer() {
	return new DSInitializer();

}

DSInitializer::~DSInitializer() {
}

bool DSInitializer::process(Frame* frame) {

	if (prevF == nullptr) {

		prevF = frame;
		extractor->process(frame);

		return false;
	}

	currF = frame;

	bool suc = initialize();

	//if (!suc){
	//	delete prevF;
	//	prevF = currF;
	//}

	return suc;

}

bool DSInitializer::initialize() {

	int pyrlvl = prevF->getPyramidLevel();


	if (mapPoints.empty()) {

		for (int i = 0; i < pyrlvl; i++) {
			std::vector<datastruct::Feature>& features = currF->features[i];

			for (int j = 0; j < features.size(); j++) {

				MapPoint* mp = new MapPoint();
				mp->setPosition(features[j].uv, pyrlvl);

				mapPoints.push_back(mp);

			}
		}

	}

	Optimizer::InitialBA(currF, mapPoints);


	return false;
}


void DSInitializer::makeNN() {
	const float distFactor = 0.05;

	//typedef nanoflan::


}

