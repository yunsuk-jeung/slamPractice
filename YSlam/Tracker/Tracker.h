#pragma once 
#include <iostream>
#include "datastruct/Image.hpp"

namespace dan {

	enum class TrackerType; 
	class Tracker {
	public:
		Tracker();
		virtual ~Tracker();

		static Tracker* createTracker(TrackerType trackerType);
		
		virtual void process(datastruct::ImagePtr image) = 0;

	private:

	};

}