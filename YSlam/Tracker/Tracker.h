#pragma once 
#include <iostream>
#include "Parameters/Parameters.h"


class Frame;
class Initializer;
class Graph;
class Tracker {
public:
	

	static Tracker* createTracker(TrackerType trackerType);

	void setGraph(Graph* _graph);

	virtual ~Tracker();
	virtual void process(Frame* frame) = 0;

protected:
	Tracker();
	Initializer* initializer = nullptr;

	SystemState systemState = SystemState::INITIALIZE;
	Graph* graph;
private:
};

