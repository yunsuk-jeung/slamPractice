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

	std::list<Frame*> frames;
	std::map<int, MapPoint*> mapPoints;
private:


};
}