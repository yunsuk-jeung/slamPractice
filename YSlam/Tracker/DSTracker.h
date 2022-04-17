#pragma once 

#include <iostream>
#include "Tracker/Tracker.h"
#include "datastruct/Image.hpp"

namespace dan {
	
	class DSTracker : public Tracker {
	public:
		DSTracker();
		virtual ~DSTracker();

		virtual void process(datastruct::ImagePtr image);

	private:

	};

}