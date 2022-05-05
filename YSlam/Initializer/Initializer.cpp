#include "Initializer/Initializer.h"
#include "Initializer/DSInitializer.h"

#include "Parameters/Parameters.h"
namespace dan {
	Initializer::Initializer() {}
	Initializer::~Initializer() {}

	Initializer* Initializer::createInitializer(TrackerType trackerType) {
		switch (trackerType) {

		case TrackerType::DENSE_SPARSE:
			return DSInitializer::createInitializer();
		}
		
		return nullptr;
	}

}