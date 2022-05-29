#include "Tracker/Tracker.h"
#include "Tracker/DSTracker.h"
#include "Parameters/Parameters.h"

#include "Initializer/Initializer.h"


namespace dan {
	Tracker::Tracker() {}
	Tracker::~Tracker() {}

	void Tracker::setGraph(Graph* _graph) {
			graph = _graph;
			initializer->setGraph(graph);
	}

	Tracker* Tracker::createTracker(TrackerType trackerType) {
		switch (trackerType) {

		case TrackerType::DENSE_SPARSE:
			return DSTracker::createTracker();
			
		}

		return nullptr;
	}

}