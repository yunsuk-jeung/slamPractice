//
//  FileReader.cpp
//  DirectSLAM_Tutorial
//
//  Created by Yunsuk on 2021/12/12.
//
#include <iostream>
#include <fstream> 

#include "Utility/FileReader/FileReader.h"

#include "datastruct/Image.hpp"

using namespace std;

namespace dan {

FileReader::FileReader() {
	//    std::cout << "Creating File Reader" << std::endl;
}

void FileReader::openDirectory(std::string dataPath) {
	std::ifstream file;
	file.open(dataPath + "data.csv");

	//std::string imageType = ".png";

	std::string line;
	getline(file, line);
	std::cout << "File Format : " << line << std::endl;

	while (file.eof() == false) {
		getline(file, line);

		unsigned long long int timestamp;

		int pos = line.find(",");
		string data = line.substr(0, pos);
		line = line.substr(pos + 1, line.length() - pos);
		timestamp = stoull(data);


		string imageName;

		pos = line.find(",");
		data = line.substr(0, pos);
		line = line.substr(pos + 1, line.length() - pos);
		imageName = dataPath + "data/"+ data;

		//cv::Mat cvImage = cv::imread(dataPath + "data/" + imageName, cv::IMREAD_GRAYSCALE);
		//ImagePtr image(new Image);
		//image->timestamp = timestamp;
		//image->image = cvImage.clone();

		imageInfos.push(std::pair<unsigned long long int, string>(timestamp, imageName));
		//imageNames.push(make_pair(timestamp, imageName));
		//imageQueue.push(image);
		//std::cout << "reading Image" << std::endl;
	}
}

FileReader::~FileReader() {
	//    std::cout << "Deleting File Reader" << std::endl;
}

bool FileReader::getImage(std::shared_ptr<datastruct::Image> image) {
	//cv::Mat image = cv::imread(dataPath + files[imgCount], cv::IMREAD_GRAYSCALE);

	if (imageInfos.empty()) {
		return false;
	}

	std::pair<unsigned long long int, std::string> imageInfo;
	imageInfo = imageInfos.front();

	image->timestamp = imageInfo.first;
	image->cvImage  = cv::imread(imageInfo.second, cv::IMREAD_GRAYSCALE);
	image->width  = image->cvImage.cols;
	image->height = image->cvImage.rows;
	image->length  = image->cvImage.cols * image->cvImage.rows;

	imageInfos.pop();

	//if (!isEnd) {
	//	cv::Mat image = cv::imread(dataPath + "1403636579763555584.png", cv::IMREAD_GRAYSCALE);
	//	_image = cv::imread(dataPath + "1403636579763555584.png", cv::IMREAD_GRAYSCALE);

	//	cv::imshow("test", _image);
	//	cv::waitKey();
	//}
	//1403636579763555584.png
	//cv::pyrDown(image,image,cv::Size(image.cols/2, image.rows/2));
	//cv::pyrDown(image,image,cv::Size(image.cols/2, image.rows/2));
//    cv::pyrDown(image,image,cv::Size(image.cols/2, image.rows/2));

	//cv::resize(image,image,cv::Size(640,480),CV_INTER_LINEAR);
	//cv::equalizeHist(image,image);

	//cv::Mat imageF= cv::Mat::zeros(480,640,CV_32FC1);

	//image.convertTo(imageF,CV_32FC1,1.0/255.0);

	return true;
}
}