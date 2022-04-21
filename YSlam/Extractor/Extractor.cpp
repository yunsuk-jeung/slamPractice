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

}