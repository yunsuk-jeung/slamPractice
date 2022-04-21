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

		virtual void extract(Frame* frame) = 0;

	private:

	};

}