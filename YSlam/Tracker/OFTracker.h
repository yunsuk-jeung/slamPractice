#pragma once 

#include <iostream>
#include "Tracker/Tracker.h"
#include "datastruct/Image.hpp"

namespace dan {

	class OFTracker : public Tracker {
	public:
		OFTracker();
		virtual ~OFTracker();

		virtual void process(datastruct::ImagePtr image);

	private:

	};

}