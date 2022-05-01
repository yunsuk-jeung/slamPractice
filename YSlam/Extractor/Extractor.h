#pragma once 

#include <iostream>


namespace dan {
	class Frame;

	enum class TrackerType;
	class Extractor{
	public:

		virtual ~Extractor();

		static Extractor* createExtractor(TrackerType trackerType);

		void setConfig();

		virtual void extract(Frame* frame) = 0;

	protected:
		Extractor();
		
		//image width, height
		int _width;
		int _height;

		// grid Num
		int _xGridNum = 0;
		int _yGridNum = 0;

		// grid Step
		int _xStep = 0;
		int _yStep = 0;

	private:

	};

}