#include <algorithm>

#include "Extractor/GridPixelExtractor.h"
#include "Graph/Frame.h"
#include "Parameters/Parameters.h"

#include "Utils/Timer.h"



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

	extractInOrigin(frame);
	extractInPyramid(frame);

	if (SHOW_ORIGIN_FEATURE) {

		cv::Mat testImage0 = frame->getImagePyramid()->images[0].clone();
		cv::cvtColor(testImage0, testImage0, CV_GRAY2BGR);

		std::vector<datastruct::Feature>& feature = frame->features[0];

		for (int i = 0; i < feature.size(); i++) {
			cv::circle(testImage0, cv::Point2f(feature[i].uv.x(), feature[i].uv.y()), 3, cv::Scalar(0, 0, 255));
		}

		cv::imshow("testImage", testImage0);
		cv::waitKey(1);
	}

	if (SHOW_PYRAMID_FEATRUE) {

		for (int i = 1; i < frame->getPyramidLevel(); i++) {

			cv::Mat image = frame->getImagePyramid()->images[0];
			cv::cvtColor(image, image, CV_GRAY2BGR);
			std::vector<datastruct::Feature>& feature = frame->features[i];

			std::cout << "feature size : " << feature.size() << std::endl;
 			for (int ii = 0; ii < feature.size(); ii++) {
				cv::circle(image, cv::Point2f(feature[ii].uv.x(), feature[ii].uv.y()) * (1 << i), 3, cv::Scalar(0, 0, 255));
			}

			cv::imshow("debug" + std::to_string(i), image);
			cv::waitKey(1);

		}

	}     

}

void GridPixelExtractor::extractInOrigin(Frame* frame, int space, float thFactor) {

	std::vector<cv::Mat >& magImages = frame->getMagGradientPyramid()->images;
	std::vector<cv::Mat >& xImages = frame->getXGradPyramid()->images;
	std::vector<cv::Mat >& yImages = frame->getYGradPyramid()->images;

	std::vector<datastruct::Feature>& features = frame->features[0];


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
					features.push_back(datastruct::Feature(bestX0, bestY0, 0));
					n0++;
					bestN1 = 1e10;
				}

			}

			if (bestX1 > 0) {
				features.push_back(datastruct::Feature(bestX1, bestY1, 0));

				n1++;
				bestN2 = 1e10;
			}

		}

		if (bestX2 > 0) {
			features.push_back(datastruct::Feature(bestX2, bestY2, 0));

			n2++;
		}

	}

	std::cout << "extract num : " << n0 << " " << n1 << " " << n2 << std::endl;

}

void GridPixelExtractor::extractInPyramid(Frame* frame, int space, float thFactor) {

	std::vector<cv::Mat >& magImages = frame->getMagGradientPyramid()->images;
	std::vector<cv::Mat >& xImages = frame->getXGradPyramid()->images;
	std::vector<cv::Mat >& yImages = frame->getYGradPyramid()->images;

	std::vector<cv::Mat>& images = frame->getImagePyramid()->images;

	int pyrlvl = frame->getPyramidLevel();

	for (int i = 1; i < pyrlvl; i++) {
		float* mags = (float*)magImages[i].data;
		float* dIdxs = (float*)xImages[i].data;
		float* dIdys = (float*)yImages[i].data;

		int width = magImages[i].cols;
		int height = magImages[i].rows;

		int dw = space;
		int dh = space;

		cv::Mat image = images[i].clone();
		cv::cvtColor(image, image, CV_GRAY2BGR);

		std::vector<datastruct::Feature>& features = frame->features[i];

		for (int y0 = 0; y0 < height; y0 += dh) for (int x0 = 0; x0 < width; x0 += dw) {

			int bestXX = -1;
			int bestXY = -1;
			int bestYX = -1;
			int bestYY = -1;

			int bestXXidx = -1;
			int bestXYidx = -1;
			int bestYXidx = -1;
			int bestYYidx = -1;

			for (int y1 = 0; y1 < dh; y1++) for (int x1 = 0; x1 < dw; x1++) {

				int x01 = x0 + x1;
				int y01 = y0 + y1;

				if (x01 >= width || y01 >= height) {
					break;
				}

				int idx = x01 + y01 * width;

				float mag = mags[idx];
				float threshold = 7.5;

				if (mag > threshold * threshold) {
					float dIdx = fabs(dIdxs[idx]);
					float dIdy = fabs(dIdys[idx]);
					float dxdy = fabs(dIdx - dIdy);
					float dydx = fabs(dIdx + dIdy);

					if (dIdx > bestXX) { bestXX = dIdx; bestXXidx = idx; continue; }
					if (dIdy > bestYY) { bestYY = dIdy; bestYYidx = idx; continue; }
					if (dxdy > bestXY) { bestXY = dxdy; bestXYidx = idx; continue; }
					if (dydx > bestYX) { bestYX = dydx; bestYXidx = idx; continue; }

				}
			}


			if (bestXXidx != -1) {
				int y1 = bestXXidx / width;
				int x1 = bestXXidx - y1 * width;
				features.push_back(datastruct::Feature(x1, y1, i));
			}

			if (bestXXidx != -1) {
				int y2 = bestYYidx / width;
				int x2 = bestYYidx - y2 * width;
				features.push_back(datastruct::Feature(x2, y2, i));

			}

			if (bestXYidx != -1) {

				int y3 = bestXYidx / width;
				int x3 = bestXYidx - y3 * width;
				features.push_back(datastruct::Feature(x3, y3, i));
			}

			if (bestYXidx != -1) {

				int y4 = bestYXidx / width;
				int x4 = bestYXidx - y4 * width;
				features.push_back(datastruct::Feature(x4, y4, i));
			}

		}



	}


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

	if (SHOW_ORIGIN_FEATURE == 1) {

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

