#include "Extractor/GridPixelExtractor.h"
#include "datastruct/Frame.h"

namespace dan {
	GirdPixelExtractor::GirdPixelExtractor() {
		std::cout << "Extractor : GridPixelExtractor" << std::endl;

		bin = new int[32 * 32 * 50]();
		

	}
	GirdPixelExtractor::~GirdPixelExtractor() {
		
	}


	void GirdPixelExtractor::extract(Frame* frame) {
		
		int prlvl = frame->getPyramidLevel();

		float* xgrad = (float*)frame->getXGradPyramid()->images[0].data;
		float* ygrad = (float*)frame->getYGradPyramid()->images[0].data;

		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _height; j++) {
				
			
				float dx = xgrad[i * _width + j];
				float dy = ygrad[i * _width + j];

				float sum = dx * dx + dy * dy;

				if (sum / 2500 > 1) {
					//bin[ i / _xStep * 32 * 50 + j / _yStep * 50 + 49] += 1;
				}
				else {
					//bin[ i / _xStep * 32 * 50 + j / _yStep * 50 + (int)sum - 1];
				}

			}
		}



		for (int lvl = 0; lvl < prlvl; lvl++) {
			
		}

	}

}