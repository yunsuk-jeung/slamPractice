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

	std::string line;
	getline(file, line);
	std::cout << "File Format : " << line << std::endl;

	while (file.eof() == false) {
		getline(file, line);

		unsigned long long int timestamp;

		int pos = line.find(",");
		string data = line.substr(0, pos);
		line = line.substr(pos + 1, line.length() - pos);
		timestamp = std::stoull(data);

		string imageName;

		pos = line.find(",");
		data = line.substr(0, pos);
		line = line.substr(pos + 1, line.length() - pos);
		imageName = dataPath + "data/"+ data;

		imageInfos.push(std::pair<unsigned long long int, string>(timestamp, imageName));
	}
}

FileReader::~FileReader() {
}

bool FileReader::getImage(std::shared_ptr<datastruct::Image> image) {

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

	return true;
}
}