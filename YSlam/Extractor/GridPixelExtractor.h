#pragma once 

#include <iostream>
#include <vector>
#include "Extractor/Extractor.h"



	class Frame;
	class GridPixelExtractor : public Extractor {
	public:

		virtual ~GridPixelExtractor();
		virtual void process(Frame* frame);
		virtual void allocate();

		void extract_scath(Frame* frame);

		void extractInOrigin(Frame* frame, int space = 12, float thFactor = 2.0f);
		void extractInPyramid(Frame* frame, int space = 12, float thFactor = 2.0f);

		static GridPixelExtractor* createGridPixelExtractor();

	protected:

	private:
		void makeHistogram(Frame* mag);
		GridPixelExtractor();

		std::vector<std::vector<int>> histBins;
		std::vector<int> histThreshold;
		
		unsigned char* randUC = nullptr;

		int xPixelSearchNum = 0;
		int yPixelSearchNum = 0;
	};

