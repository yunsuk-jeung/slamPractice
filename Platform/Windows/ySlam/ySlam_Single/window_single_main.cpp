#include <iostream>
#include <thread>

#include "include/YSlam.h"
#include "datastruct/Image.hpp"

#include "Utility/FileReader/FileReader.h"

#include "Eigen/Eigen"
#include "Eigen/Dense"
#include "Eigen/Core"

std::string dataPath = "D:/dataset/EUROC/MH_01_easy/mav0/cam0/";
std::string parameterPath = "../../../../parameters.yaml";

void mainFunc() {

	dan::FileReader fileReader;
	fileReader.openDirectory(dataPath);

	dan::datastruct::ImagePtr image(new dan::datastruct::Image());

	int interval = 0;

	while ( fileReader.getImage(image) ) {
		

		//if (interval != 2) {
		//	interval++;
		//	continue;
		//}
		//interval = 0;

		cv::imshow("original", image->cvImage);

		dan::YSlam::getInstance()->setNewFrame(image->cvImage.data, image->length, image->width, image->height, dan::datastruct::ColorFormat::GRAY8, image->timestamp);

		int key = cv::waitKey();

		if (key == 27) {
			break;
		}
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