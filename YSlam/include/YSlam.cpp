#include "include/YSlam.h"

namespace YSlam {

	YSlam* instance = nullptr;

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


	bool YSlam::init() {
		return true;
	}
}