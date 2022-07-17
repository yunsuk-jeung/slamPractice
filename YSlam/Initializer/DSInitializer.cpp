#include <opencv2/opencv.hpp>

#include "Extractor/GridPixelExtractor.h"

#include "Tracker/DSTracker.h"

#include "Graph/Frame.h"
#include "Graph/InvPoint.h"

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

	auto& mapPoints = graph->mapPoints;

	if (mapPoints.empty()) {

		for (int i = 0; i < pyrlvl; i++) {
			std::vector<datastruct::Feature>& features = currF->features[i];

			for (int j = 0; j < features.size(); j++) {

				InvPoint* mp = new InvPoint();
				Eigen::Vector2f d;
				d << 0.5, 0.5;
				mp->setPosition(features[j].uv + d, pyrlvl);
				mp->id = mapPoints.size();
				mapPoints.push_back(mp);

			}
		}

	}

	Optimizer::InitialBA(prevF ,currF, mapPoints);


	return false;
}


void DSInitializer::makeNN() {
	const float distFactor = 0.05;

	//typedef nanoflan::


}

