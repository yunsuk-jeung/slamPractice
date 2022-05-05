#include <algorithm>

#include "Extractor/GridPixelExtractor.h"
#include "datastruct/Frame.h"
#include "datastruct/Feature.h"
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

	xPixelSearchNum = X_PIXEL_SEARCH_NUM;
	yPixelSearchNum = Y_PIXEL_SEARCH_NUM;
}

void GridPixelExtractor::makeHistogram(Frame* frame) {
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

	float* mag = (float*)(frame->getMagGradientPyramid()->images[0].data);

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


	//float* xgrad = (float*)(frame->getXGradPyramid()->images[0].data);
	//float* ygrad = (float*)(frame->getYGradPyramid()->images[0].data);

	Statistics::startTimer(HISTOGRAM);

	makeHistogram(frame);

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

	/* ------------------------------------------------*/
	/* ----------------- extract feature --------------*/
	
	int pyrlvl = frame->getPyramidLevel();
	   
	Statistics::startTimer(EXTRACTION);

	std::vector<std::vector<cv::Point2i>> outPoint;

	outPoint.resize(pyrlvl);

	for (int lvl = 0; lvl < pyrlvl; lvl++) {
		float* mag = (float*)(frame->getMagGradientPyramid()->images[lvl].data);

		int xPixelNum = xPixelSearchNum >> lvl;
		int yPixelNum = yPixelSearchNum >> lvl;

		int width = _width >> lvl;
		int height = _height >> lvl;

		int xRegion = (width + xPixelNum - 1) / xPixelNum;
		int yRegion = (height + yPixelNum - 1) / yPixelNum;

		for (int i = 0; i < yRegion; i++) {
			for (int j = 0; j < xRegion; j++) {

				std::vector<std::pair<int, cv::Point2i>> uvCandiates;

				int row = yPixelNum * i;
				int col = xPixelNum * j;

				int start = row * width + col;

				for (int k = 0; k < yPixelNum; k++) {

					int row2 = k * width;
					int v = row + k;

					if (v > height) {
						break;
					}

					for (int w = 0; w < xPixelNum; w++) {
						int u = col + w;

						if (u > width)
							break;

						//if (mag[start + row2 + w] > 40) {
						if (mag[start + row2 + w] > histThreshold[(u << lvl) / _xStep + (v << lvl) / _yStep]) {

							cv::Point2i temp;
							temp.x = u;
							temp.y = v;

							uvCandiates.push_back(std::pair<int, cv::Point2i>(mag[start + row2 + w], temp));

						}
					}

				}

				for (int kk = 0; kk < uvCandiates.size(); kk++) {
					outPoint[lvl].push_back(uvCandiates[kk].second);
				}

			}
		}

	}


	for (int i = 0; i < pyrlvl; i++) {
		int featureSize = outPoint[i].size();
		frame->features[i].reserve(featureSize);
		
		for (int j = 0; j < featureSize; j++) {
			datastruct::Feature feature(outPoint[i][j].x, outPoint[i][j].y, i);

			frame->features[i].push_back(feature);
		}

	}

	/* ----------------- extract feature --------------*/
	/* ------------------------------------------------*/

	Statistics::stopTimer(EXTRACTION);

	std::vector<TIME_PERIOD> statisticArray;
	statisticArray.push_back(HISTOGRAM);
	statisticArray.push_back(EXTRACTION);

	Statistics::report(statisticArray);

	if (SHOW_FEATURE == 1) {

		{
			cv::Mat debug = frame->getImagePyramid()->images[0].clone();
			
			for (int i = 0; i < pyrlvl; i++) {

			cv::Mat debug1;
			debug1 = debug.clone();

			cv::cvtColor(debug1, debug1, cv::COLOR_GRAY2BGR);



			for (int j = 0; j < outPoint[i].size(); j++) {
				cv::circle(debug1, cv::Point2i(outPoint[i][j].x << i, outPoint[i][j].y << i), 2 , cv::Scalar(0, 0, 255), 1);
			}




			cv::imshow("debug " + std::to_string(i), debug1);


			}
			cv::waitKey();

		}

	}





}




}