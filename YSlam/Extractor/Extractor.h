#pragma once 

#include <iostream>


namespace dan {
	class Frame;

	enum class TrackerType;
	class Extractor{
	public:

		Extractor();
		virtual ~Extractor();

		static Extractor* createExtractor(TrackerType trackerType);

		void setImageInfo(int width, int height);

		virtual void extract(Frame* frame) = 0;

		int _width;
		int _height;

		int _xStep = 0;
		int _yStep = 0;

	protected:

	};

}