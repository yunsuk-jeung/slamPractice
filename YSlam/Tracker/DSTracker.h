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

		static DSTracker* createTracker();
		virtual ~DSTracker();
		virtual void process(Frame* frame);

	private:
		DSTracker();
		
	};

}