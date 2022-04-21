#include <iostream>
#include <thread>

#include "include/YSlam.h"
#include "datastruct/Image.hpp"

#include "Utility/FileReader/FileReader.h"


std::string dataPath = "D:/dataset/EUROC/MH_01_easy/mav0/cam0/";
std::string parameterPath = "../../../../parameters.yaml";

void mainFunc() {

	dan::FileReader fileReader;
	fileReader.openDirectory(dataPath);

	dan::datastruct::ImagePtr image(new dan::datastruct::Image());

	while ( fileReader.getImage(image) ) {

		dan::YSlam::getInstance()->setNewFrame(image->cvImage.data, image->length, image->width, image->height, dan::datastruct::ColorFormat::GRAY8, image->timestamp);
		break;
	}

	dan::YSlam::getInstance()->deleteInstance();
}


int main() {

	if (dan::YSlam::getInstance()->init(dataPath, parameterPath)) {
		std::cout << "YSlam init success" << std::endl;
	}

	std::thread mainThread;
	mainThread = std::thread(mainFunc);
	
	if (mainThread.joinable())
		mainThread.join();

	return 0;
}