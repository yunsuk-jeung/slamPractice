#include "Extractor/Extractor.h"
#include "Extractor/GridPixelExtractor.h"
#include "Parameters/Parameters.h"

namespace dan {
	Extractor::Extractor() {}
	Extractor::~Extractor() {}

	Extractor* Extractor::createExtractor(TrackerType trackerType) {
		switch (trackerType) {

		case TrackerType::DENSE_SPARSE:
			return new GirdPixelExtractor();
		}
		return nullptr;
	}


	//todo 나중에 image config 에서 받아오게하기
	void Extractor::setImageInfo(int width, int height) {
		_width = width;
		_height = height;

		_xStep = (_width + 31 ) >> 5;
		_yStep = (_height + 31) >> 5;

		std::cout << _xStep << " " << _yStep << std::endl;;
	}

}