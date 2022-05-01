#pragma once 

#include <iostream>
#include <vector>
#include "Extractor/Extractor.h"

namespace dan {

	class Frame;
	class GirdPixelExtractor : public Extractor {
	public:
		GirdPixelExtractor();

		virtual ~GirdPixelExtractor();
		virtual void extract(Frame* frame);


	protected:

	private:

		//std::vector< std::vector <int> > histBins;
		std::vector<std::vector<int>> histBins;
		std::vector<int> histThreshold;
		
	};

}