#include <algorithm>

#include "Extractor/GridPixelExtractor.h"
#include "datastruct/Frame.h"

#include "Parameters/Parameters.h"

#include "Utils/Timer.h"


namespace dan {
GridPixelExtractor::GridPixelExtractor() {
	std::cout << "Extractor : GridPixelExtractor" << std::endl;

}

GridPixelExtractor::~GridPixelExtractor() {
}

GridPixelExtractor* GridPixelExtractor::createGridPixelExtractor() {
	return new GridPixelExtractor();
}

void GridPixelExtractor::allocate() {
	histBins.resize(_xGridNum * _yGridNum);
	histThreshold.resize(_xGridNum * _yGridNum);
	int histBinSize = histBins.size();
	for (int i = 0; i < histBinSize; i++) {
		histBins[i].resize(50, 0);
	}
}

void GridPixelExtractor::makeHistogram(float* mag) {
	//	 todo histogram 기준으로 for 문 돌리게 수정해야 할듯? -> 나누기 안하게
	/*for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {

			int sum = mag[i * _width + j] - 1;

			if (sum < 0) {
				sum = 0;
			}
			else if (sum > 49) {
				sum = 49;
			}

			histBins[i / _yStep * _xGridNum + j / _xStep][sum] += 1;

		}
	}*/


	for (int i = 0; i < _yGridNum; i++) {
		for (int j = 0; j < _xGridNum; j++) {

			std::vector<int>& histBin = histBins[i * _xGridNum + j];
			int start = _yStep * i * _width + _xStep * j;

			for (int k = 0; k < _yStep; k++) {
				int row = k * _width;

				for (int w = 0; w < _xStep; w++) {

					int sum = mag[start + row + w];
					if (sum < 0) {
						sum = 0;
					}
					else if (sum > 49) {
						sum = 49;
					}
					histBin[sum] += 1;
				}
			}
		}
	}

	int histSize = histBins.size();

	for (int i = 0; i < histSize; i++) {
		int binSize = histBins[i].size();
		int count = 0;
		int pixelNum = _yStep * _xStep;
		pixelNum >>= 1;
		for (int j = 0; j < binSize; j++) {
			count += histBins[i][j];
			if (count > pixelNum) {
				histThreshold[i] = j + 20;
				break;
			}
		}
	}

}

void GridPixelExtractor::process(Frame* frame) {

	int prlvl = frame->getPyramidLevel();

	float* xgrad = (float*)(frame->getXGradPyramid()->images[0].data);
	float* ygrad = (float*)(frame->getYGradPyramid()->images[0].data);
	float* mag = (float*)(frame->getMagGradientPyramid()->images[0].data);

	Statistics::startTimer(HISTOGRAM);

	makeHistogram(mag);

	Statistics::stopTimer(HISTOGRAM);


	if (SHOW_THRESHOLD_IMAGE == 1) {
		{
			cv::Mat origin = frame->getImagePyramid()->images[0].clone();
			cv::Mat test1 = origin.clone();
			cv::Mat test = frame->getMagGradientPyramid()->images[0].clone();

			unsigned char* imgData = test1.data;
			float* magData = (float*)(test.data);

			for (int i = 0; i < _height; i++) {
				for (int j = 0; j < _width; j++) {

					float mag = magData[i * _width + j];

					if (mag > histThreshold[i / _yStep * _xGridNum + j / _xStep]) {
						imgData[i * _width + j] = 255;
					}
					else {
						imgData[i * _width + j] = 0.0f;
					}


				}
			}

			cv::hconcat(origin, test1, test1);

			cv::imshow("xthresh", test1);
			cv::waitKey(1);
		}
	}

	/*int xRegion = _xGridNum << 1;
	int yRegion = _yGridNum << 1;

	int xPixelNum = _xStep >> 1;
	int yPixelNum = _yStep >> 1;
*/

	int xPixelNum = 10;
	int yPixelNum = 10;

	int xRegion = (_width + xPixelNum - 1) / xPixelNum;
	int yRegion = (_height + yPixelNum - 1) / yPixelNum;



	Statistics::startTimer(EXTRACTION);

	std::vector<cv::Point2f> uvs;
	for (int i = 0; i < yRegion; i++) {
		for (int j = 0; j < xRegion; j++) {

			int row = yPixelNum * i;
			int col = xPixelNum * j;

			int start = row * _width + col;

			int count = 0;

			for (int k = 0; k < yPixelNum; k++) {


				int row2 = k * _width;
				int v = row + k;

				if (v > _height) {
					break;
				}
				if (count > 1) {
					break;
				}

				for (int w = 0; w < xPixelNum; w++) {
					int u = col + w;

					if (u > _width)
						break;
					if (count > 1) {
						break;
					}
					// 그냥 20 박아놔도 그럴듯함...?
					//if (mag[start + row2 + w] > 40) {
					if (mag[start + row2 + w] > histThreshold[u / _xStep + v / _yStep]) {
						
						cv::Point2f temp;
						temp.x = u;
						temp.y = v;
						uvs.push_back(temp);
						count++;

					}
				}

			}

		}
	}


	Statistics::stopTimer(EXTRACTION);

	std::vector<TIME_PERIOD> statisticArray;
	statisticArray.push_back(HISTOGRAM);
	statisticArray.push_back(EXTRACTION);

	Statistics::report(statisticArray);

	if (SHOW_FEATURE == 1) {
		cv::Mat debug;
		debug = frame->getImagePyramid()->images[0].clone();
		cv::cvtColor(debug, debug, cv::COLOR_GRAY2BGR);
		for (int i = 0; i < uvs.size(); i++) {
			cv::circle(debug, uvs[i], 2, cv::Scalar(0, 0, 255), -1);
		}

		cv::imshow("debug", debug);
		cv::waitKey(1);
	}





}




}