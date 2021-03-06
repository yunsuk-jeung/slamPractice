#include "Initializer/Initializer.h"
#include "Initializer/DSInitializer.h"

#include "Parameters/Parameters.h"

	Initializer::Initializer() {}
	Initializer::~Initializer() {}

	Initializer* Initializer::createInitializer(TrackerType trackerType) {
		switch (trackerType) {

		case TrackerType::DENSE_SPARSE:
			return DSInitializer::createInitializer();
		}
		
		return nullptr;
	}
	void Initializer::setGraph(Graph* _graph) {
		graph = _graph;
	}

