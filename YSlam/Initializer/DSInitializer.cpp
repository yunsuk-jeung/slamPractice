#include <opencv2/opencv.hpp>

#include "Extractor/GridPixelExtractor.h"

#include "Tracker/DSTracker.h"
#include "datastruct/Frame.h"
#include "datastruct/MapPoint.h"

#include "Graph/Graph.h"

#include "Initializer/DSInitializer.h"
#include "Parameters/Parameters.h"

namespace dan {
DSInitializer::DSInitializer() {
	
	extractor = Extractor::createExtractor(TRACKER_TYPE);
}

DSInitializer* DSInitializer::createInitializer() {
	return new DSInitializer();

}

DSInitializer::~DSInitializer() {
}

bool DSInitializer::process(Frame* frame) {

	if (prev == nullptr) {

		prev = frame;
		extractor->process(frame);

		int pyrlvl = frame->getPyramidLevel();

		frames.push_back(frame);

		for (int i = 0; i < pyrlvl; i++) {
			std::vector<datastruct::Feature>& features = frame->features[i];

			for (int j = 0; j < features.size(); j++) {

				MapPoint* mp = new MapPoint();
				mp->setPosition(features[j].uv, pyrlvl);
				
				mapPoints.push_back(mp);

			}
		}
		
		return false;
	}

	curr = frame;

	bool suc= initialize();

	if (!suc){
		delete prev;
		prev = curr;
	}
	
	return suc;

}

bool DSInitializer::initialize() {
	

	return false;
}


void DSInitializer::makeNN() {
	const float distFactor = 0.05;

	//typedef nanoflan::


}

}