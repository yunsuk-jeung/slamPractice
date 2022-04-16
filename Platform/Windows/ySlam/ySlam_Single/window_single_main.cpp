#include <iostream>
#include "include/YSlam.h"

int main() {

	YSlam::YSlam* ySlam = YSlam::YSlam::getInstance();

	if (YSlam::YSlam::getInstance()->init()) {
		std::cout << "success" << std::endl;
	}

	return 0;
}