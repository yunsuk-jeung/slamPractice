#include <opencv2/opencv.hpp>

#include "Tracker/DSTracker.h"
#include "datastruct/Frame.h"

#include "Initializer/DSInitializer.h"
#include "Parameters/Parameters.h"

namespace dan {
DSInitializer::DSInitializer() {

}

DSInitializer* DSInitializer::createInitializer() {
	return new DSInitializer();

}

DSInitializer::~DSInitializer() {
}

bool DSInitializer::process(Frame* frame) {


	if (prev == nullptr) {
		prev = frame;
		return false;
	}

	curr = frame;

	std::cout << "Processing Initialize" << std::endl;

	bool suc = false;

	if (!suc){
		delete prev;
		prev = curr;
	}
	
	return suc;

}

}