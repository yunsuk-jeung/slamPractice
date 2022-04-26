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

		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
				
				int u = i >> 5;
				int v = j >> 5;
			
				float dx = xgrad[i * _height + j];
				float dy = ygrad[i * _height + j];

				int sum = sqrt(dx * dx + dy * dy) - 1 ;

				if (sum < 0) {
					sum = 0;
				}
				else if (sum > 48 ){
					sum = 48;
				}



			}
		}



		for (int lvl = 0; lvl < prlvl; lvl++) {
			
		}

	}

}