#include <algorithm>

#include "Extractor/GridPixelExtractor.h"
#include "datastruct/Frame.h"

#include "Parameters/Parameters.h"


namespace dan {
GirdPixelExtractor::GirdPixelExtractor() {
	std::cout << "Extractor : GridPixelExtractor" << std::endl;

	setConfig();
	histBins.resize(_xGridNum * _yGridNum);
	histThreshold.resize(_xGridNum * _yGridNum);
	int histBinSize = histBins.size();
	for (int i = 0; i < histBinSize; i++) {
		histBins[i].resize(50, 0);
	}


}
GirdPixelExtractor::~GirdPixelExtractor() {
}


void GirdPixelExtractor::extract(Frame* frame) {

	int prlvl = frame->getPyramidLevel();

	float* xgrad = (float*)(frame->getXGradPyramid()->images[0].data);
	float* ygrad = (float*)(frame->getYGradPyramid()->images[0].data);
	float* mag = (float*)(frame->getMagGradientPyramid()->images[0].data);

	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {

			float dx = xgrad[i * _width + j];
			float dy = ygrad[i * _width + j];

			int sum = mag[i * _width + j] - 1;

			if (sum < 0) {
				sum = 0;
			}
			else if (sum > 49) {
				sum = 49;
			}

			histBins[i / _yStep * _xGridNum + j / _xStep][sum] += 1;

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
				histThreshold[i] = j + 15;
				break;
			}
		}
	}

	if (SHOW_THRESHOLD_IMAGE) {
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

}




}