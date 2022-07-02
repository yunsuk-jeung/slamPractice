#include <opencv2/opencv.hpp>

#include "Tracker/DSTracker.h"
#include "Graph/Frame.h"

#include "Initializer/DSInitializer.h"

#include "Parameters/Parameters.h"

namespace dan {
DSTracker::DSTracker() {
	std::cout << "Tracker is DSTracker" << std::endl;
	initializer = Initializer::createInitializer(TRACKER_TYPE);
}

DSTracker* DSTracker::createTracker() {
	return new DSTracker();
	
}

DSTracker::~DSTracker() {
}

void DSTracker::process(Frame* frame) {

	switch (systemState) {
	case(SystemState::INITIALIZE): 
		initializer->process(frame);
		break;
	}
	
}

}