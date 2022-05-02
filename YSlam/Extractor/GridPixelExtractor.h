#pragma once 

#include <iostream>
#include <vector>
#include "Extractor/Extractor.h"

namespace dan {

	class Frame;
	class GridPixelExtractor : public Extractor {
	public:

		virtual ~GridPixelExtractor();
		virtual void process(Frame* frame);
		virtual void allocate();

		static GridPixelExtractor* createGridPixelExtractor();

	protected:

	private:
		void makeHistogram(Frame* mag);
		GridPixelExtractor();
		//std::vector< std::vector <int> > histBins;
		std::vector<std::vector<int>> histBins;
		std::vector<int> histThreshold;

		int xPixelSearchNum = 0;
		int yPixelSearchNum = 0;
	};

}