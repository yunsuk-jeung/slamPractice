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

	randUC = new unsigned char[_width * _height];

	for (int i = 0; i < _width*_height; i++) {
		randUC[i] = rand() & 0xFF;
	}

}

void GridPixelExtractor::makeHistogram(Frame* frame) {

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
				histThreshold[i] = j + 7;
				break;
			}
		}
	}
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


}

void GridPixelExtractor::process(Frame* frame) {


	//float* xgrad = (float*)(frame->getXGradPyramid()->images[0].data);
	//float* ygrad = (float*)(frame->getYGradPyramid()->images[0].data);

	Statistics::startTimer(HISTOGRAM);

	makeHistogram(frame);

	Statistics::stopTimer(HISTOGRAM);

	extract(frame);
	//extract_scath(frame);       

}

void GridPixelExtractor::extract(Frame* frame, int space, float thFactor) {

	std::vector<cv::Mat >& magImages = frame->getMagGradientPyramid()->images;
	std::vector<cv::Mat >& xImages = frame->getXGradPyramid()->images;
	std::vector<cv::Mat >& yImages = frame->getYGradPyramid()->images;


	float* mag0 = (float*)magImages[0].data;
	float* mag1 = (float*)magImages[1].data;
	float* mag2 = (float*)magImages[2].data;

	float* dx0 = (float*)xImages[0].data;
	float* dx1 = (float*)xImages[1].data;
	float* dx2 = (float*)xImages[2].data;

	float* dy0 = (float*)yImages[0].data;
	float* dy1 = (float*)yImages[1].data;
	float* dy2 = (float*)yImages[2].data;


	const Eigen::Vector2f directions[16] = {
		 Eigen::Vector2f(0,    1.0000),
		 Eigen::Vector2f(0.3827,    0.9239),
		 Eigen::Vector2f(0.1951,    0.9808),
		 Eigen::Vector2f(0.9239,    0.3827),
		 Eigen::Vector2f(0.7071,    0.7071),
		 Eigen::Vector2f(0.3827,   -0.9239),
		 Eigen::Vector2f(0.8315,    0.5556),
		 Eigen::Vector2f(0.8315,   -0.5556),
		 Eigen::Vector2f(0.5556,   -0.8315),
		 Eigen::Vector2f(0.9808,    0.1951),
		 Eigen::Vector2f(0.9239,   -0.3827),
		 Eigen::Vector2f(0.7071,   -0.7071),
		 Eigen::Vector2f(0.5556,    0.8315),
		 Eigen::Vector2f(0.9808,   -0.1951),
		 Eigen::Vector2f(1.0000,    0.0000),
		 Eigen::Vector2f(0.1951,   -0.9808) };

	int dw0 = 4 * space;
	int dh0 = 4 * space;
	int dw1 = 2 * space;
	int dh1 = 2 * space;
	int dw2 = space;
	int dh2 = space;

	int n0 = 0, n1 = 0, n2 = 0;

	std::vector<cv::Point2i> test0;
	std::vector<cv::Point2i> test1;
	std::vector<cv::Point2i> test2;

	for (int y0 = 0; y0 < _height; y0 += dh0) for (int x0 = 0; x0 < _width; x0 += dh0) {


		int w1 = (dw0 > _width - x0) ? _width - x0 : dw0;
		int h1 = (dh0 > _height - y0) ? _height - y0 : dh0;

		int bestX2 = -1;
		int bestY2 = -1;
		float bestN2 = 0;
		Eigen::Vector2f dir2 = directions[randUC[n2] & 0xF];

		for (int y1 = 0; y1 < h1; y1 += dh1) for (int x1 = 0; x1 < w1; x1 += dw1) {

			int x01 = x0 + x1;
			int y01 = y0 + y1;

			int w2 = (dw1 > _width - x01) ? _width - x01 : dw1;
			int h2 = (dh1 > _height - y01) ? _height - y01 : dh1;

			int bestX1 = -1;
			int bestY1 = -1;
			float bestN1 = 0;
			Eigen::Vector2f dir1 = directions[randUC[n1] & 0xF];

			for (int y2 = 0; y2 < h2; y2 += dh2) for (int x2 = 0; x2 < w2; x2 += dw2) {

				int x012 = x01 + x2;
				int y012 = y01 + y2;

				int w3 = (dw2 > _width - x012) ? _width - x012 : dw2;
				int h3 = (dh2 > _height - y012) ? _height - y012 : dh2;

				int bestX0 = -1;
				int bestY0 = -1;
				float bestN0 = 0;
				Eigen::Vector2f dir0 = directions[randUC[n0] & 0xF];

				for (int y3 = 0; y3 < h3; y3++) for (int x3 = 0; x3 < w3; x3++) {

					int x0123 = x012 + x3;
					int y0123 = y012 + y3;

					assert(x0123 < _width);
					assert(y0123 < _height);

					int idx = y0123 * _width + x0123;

					float threshold = histThreshold[(y0123 >> 5) * _xGridNum + (x0123 >> 5)] * thFactor;
					float mag = mag0[idx];

					if (mag > threshold) {

						Eigen::Vector2f dI0(dx0[idx], dy0[idx]);
						float norm = fabs(dI0.dot(dir0));

						if (norm > bestN0) {
							bestN0 = norm;
							bestX0 = x0123;
							bestY0 = y0123;
							bestX1 = -2;
							bestY1 = -2;
							bestX2 = -2;
							bestY2 = -2;

						}

					}
					if (bestX1 == -2) continue;

					int idx1 = (y0123 >> 1) * (_width >> 1) + x0123 >> 1;
					mag = mag1[idx1];
					if (mag > threshold * 0.75) {

						Eigen::Vector2f dI1(dx1[idx1], dy1[idx1]);
						float norm = dI1.dot(dir1);

						if (norm > bestN1) {

							bestN1 = norm;
							bestX0 = -2;
							bestY0 = -2;
							bestX1 = x0123;
							bestY1 = y0123;
							bestX2 = -2;
							bestY2 = -2;
						}
					}
					if (bestX2 == -2) continue;

					int idx2 = (y0123 >> 2) * (_width >> 2) + x0123 >> 2;
					mag = mag2[idx2];

					if (mag > threshold * 0.5) {

						Eigen::Vector2f dI2(dx2[idx2], dy2[idx2]);
						float norm = dI2.dot(dir2);

						if (norm > bestN2) {

							bestN2 = norm;
							bestX0 = -2;
							bestY0 = -2;
							bestX1 = -2;
							bestY1 = -2;
							bestX2 = x0123;
							bestY2 = y0123;

						}
					}

				}

				if (bestX0 > 0) {
					test0.push_back(cv::Point2i(bestX0, bestY0));
					n0++;
					bestN1 = 1e10;
				}

			}

			if (bestX1 > 0) {
				test1.push_back(cv::Point2i(bestX1, bestY1));

				n1++;
				bestN2 = 1e10;
			}

		}

		if (bestX2 > 0) {
			test2.push_back(cv::Point2i(bestX2, bestY2));
			n2++;
		}

	}

	if (SHOW_FEATURE) {

		cv::Mat testImage0 = frame->getImagePyramid()->images[0].clone();
		cv::Mat testImage1 = frame->getImagePyramid()->images[0].clone();
		cv::Mat testImage2 = frame->getImagePyramid()->images[0].clone();

		cv::cvtColor(testImage0, testImage0, CV_GRAY2BGR);
		cv::cvtColor(testImage1, testImage1, CV_GRAY2BGR);
		cv::cvtColor(testImage2, testImage2, CV_GRAY2BGR);

		for (int i = 0; i < test0.size(); i++) {
			cv::circle(testImage0, test0[i], 3, cv::Scalar(0, 0, 255));
		}

		for (int i = 0; i < testImage0.rows; i += space) {
			cv::line(testImage0, cv::Point2i(0, i), cv::Point2i(testImage0.cols, i), cv::Scalar(255, 255, 0));
		}
		for (int i = 0; i < testImage0.cols; i += space) {
			cv::line(testImage0, cv::Point2i(i, 0), cv::Point2i(i, testImage0.rows), cv::Scalar(255, 255, 0));
		}

		for (int i = 0; i < test1.size(); i++) {
			cv::circle(testImage1, test1[i], 3, cv::Scalar(0, 255, 0));
		}
		for (int i = 0; i < testImage0.rows; i += (space << 1)) {
			cv::line(testImage1, cv::Point2i(0, i), cv::Point2i(testImage0.cols, i), cv::Scalar(255, 255, 0));
		}
		for (int i = 0; i < testImage0.cols; i += (space << 1)) {
			cv::line(testImage1, cv::Point2i(i, 0), cv::Point2i(i, testImage0.rows), cv::Scalar(255, 255, 0));
		}

		for (int i = 0; i < test2.size(); i++) {
			cv::circle(testImage2, test2[i], 3, cv::Scalar(255, 0, 0));
		}

		for (int i = 0; i < testImage0.rows; i += (space << 2)) {
			cv::line(testImage2, cv::Point2i(0, i), cv::Point2i(testImage0.cols, i), cv::Scalar(255, 255, 0));
		}
		for (int i = 0; i < testImage0.cols; i += (space << 2)) {
			cv::line(testImage2, cv::Point2i(i, 0), cv::Point2i(i, testImage0.rows), cv::Scalar(255, 255, 0));
		}

		cv::imshow("mag0", magImages[0] / 255);
		cv::imshow("mag1", magImages[1] / 255);
		cv::imshow("mag2", magImages[2] / 255);


		cv::hconcat(testImage0, testImage1, testImage1);
		cv::hconcat(testImage1, testImage2, testImage2);
		cv::imshow("testImage", testImage2);
		cv::waitKey(1);
	}


	std::cout << n0 << " " << n1 << " " << n2 << std::endl;

}


void GridPixelExtractor::extract_scath(Frame* frame) {
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
					cv::circle(debug1, cv::Point2i(outPoint[i][j].x << i, outPoint[i][j].y << i), 2, cv::Scalar(0, 0, 255), 1);
				}




				cv::imshow("debug " + std::to_string(i), debug1);


			}
			cv::waitKey();

		}

	}



}

}