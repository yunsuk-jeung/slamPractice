#include "Tracker/Tracker.h"
#include "Tracker/DSTracker.h"
#include "Parameters/Parameters.h"

namespace dan {
	Tracker::Tracker() {}
	Tracker::~Tracker() {}

	Tracker* Tracker::createTracker(TrackerType trackerType) {
		switch (trackerType) {

		case TrackerType::DENSE_SPARSE:
			return new DSTracker();;
			
		}

		return nullptr;
	}

}