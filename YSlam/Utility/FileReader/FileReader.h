#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include "datastruct/Image.hpp"


#ifdef _WIN32

#elif __APPLE__
#include <dirent.h>

#endif

	class FileReader {
	public:
		FileReader();
		~FileReader();

		void openDirectory(std::string dataPath);

		bool getImage(std::shared_ptr<datastruct::Image> image);

	private:
		//std::vector<std::string> files;

		std::queue<std::pair<unsigned long long int, std::string>> imageInfos;
		//std::queue< std::shared_ptr<Image> > imageQueue;

		int imgCount = 0;
		int imgNum = 0;

	};


