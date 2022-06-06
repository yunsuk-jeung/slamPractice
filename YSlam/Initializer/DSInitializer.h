#pragma once 

#include <iostream>
#include "Initializer/Initializer.h"
#include "Initializer/DSInitializer.h"
#include "datastruct/Image.hpp"

namespace dan {
class MapPoint;
class DSInitializer : public Initializer {
public:

	static DSInitializer* createInitializer();
	virtual ~DSInitializer();
	virtual bool process(Frame* frame);
	
	void makeNN();

private:
	DSInitializer();
	std::vector< Frame* > frames;
	std::vector< MapPoint* > mapPoints;

};

}