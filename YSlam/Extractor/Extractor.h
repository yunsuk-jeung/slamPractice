#pragma once 

#include <iostream>


namespace dan {
	class Frame;
	class Graph;
	enum class TrackerType;
	class Extractor{
	public:

		virtual ~Extractor();

		static Extractor* createExtractor(TrackerType trackerType);

		void setConfig();

		virtual void allocate() = 0;
		virtual void process(Frame* frame) = 0;

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

		Graph* graph = nullptr;

	private:

	};

}