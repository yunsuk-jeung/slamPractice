#pragma once 

#include <iostream>
#include "Extractor/Extractor.h"

namespace dan {
	class Frame;
	class GirdPixelExtractor : public Extractor {
	public:
		GirdPixelExtractor();
		virtual ~GirdPixelExtractor();

		virtual void extract(Frame* frame);

	private:

	};

}