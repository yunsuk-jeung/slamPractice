#pragma once

#include <iostream>
#include <list>
#include <map>
#include "Parameters/Parameters.h"

namespace dan {

class Frame;
class MapPoint;
class Graph {

public:

	Graph();
	~Graph();

private:
	std::list<Frame*> Frames;
	std::map<int, MapPoint*> MapPoints;

};
}