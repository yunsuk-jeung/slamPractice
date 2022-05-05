#pragma once 
#include <iostream>
#include "Parameters/Parameters.h"

namespace dan {

class Frame;
class Initializer;
class Tracker {
public:
	virtual ~Tracker();

	static Tracker* createTracker(TrackerType trackerType);

	virtual void process(Frame* frame) = 0;

protected:
	Tracker();
	Initializer* initializer = nullptr;

	SystemState systemState = SystemState::INITIALIZE;

private:
};

}