#include "Extractor/Extractor.h"
#include "Extractor/GridPixelExtractor.h"
#include "Parameters/Parameters.h"

namespace dan {
	Extractor::Extractor() {}
	Extractor::~Extractor() {}

	Extractor* Extractor::createExtractor(TrackerType trackerType) {
		switch (trackerType) {

		case TrackerType::DENSE_SPARSE:
			return new GirdPixelExtractor();;
		}
		return nullptr;
	}

	void Extractor::setConfig() {
		_width = 736;
		_height = 480;

		_xGridNum = 1 <<  X_GRID_NUM;
		_yGridNum = 1 <<  Y_GRID_NUM;

		_xStep = _width >> X_GRID_NUM;
		_yStep = _height >> X_GRID_NUM;

	}

}