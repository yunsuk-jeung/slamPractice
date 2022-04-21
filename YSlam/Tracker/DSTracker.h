#pragma once 

#include <iostream>
#include "Tracker/Tracker.h"
#include "datastruct/Image.hpp"

namespace dan {

	enum class ProcessState {
		INITIALIZE,

	};
	
	class Extractor;
	class DSTracker : public Tracker {
	public:
		DSTracker();
		virtual ~DSTracker();

		virtual void process(datastruct::ImagePtr image);

	private:
		Extractor* extractor = nullptr;
	};

}