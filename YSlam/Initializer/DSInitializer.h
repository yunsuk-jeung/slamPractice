#pragma once 

#include <iostream>
#include "Initializer/Initializer.h"
#include "Initializer/DSInitializer.h"
#include "datastruct/Image.hpp"


class InvPoint_;
class DSInitializer : public Initializer {
public:

	static DSInitializer* createInitializer();
	virtual ~DSInitializer();
	virtual bool process(Frame* frame);
	
	bool initialize();

	void makeNN();

private:
	DSInitializer();
	//std::vector< Frame* > frames;
	//std::vector< InvPoint* > mapPoints;

};

