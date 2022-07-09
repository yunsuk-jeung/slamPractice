#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Parameters/Parameters.h"


class Frame;
class MapPoint;
class Factor {
public:

	Factor() {}
	~Factor() {}

	Frame* frame;
	MapPoint* mapPoint;

private:

};



class Frame;
class MapPoint;
class Graph {

public:

	Graph();
	~Graph();
	void addFactor(Frame* frame, MapPoint* mapPoint);



	//node
	std::vector<Frame*> frames;
	std::vector<MapPoint*> mapPoints;
	
	std::vector<Factor*> factors;

	//factor
	//std::map<Frame*, std::list<Factor*>> kfFactors;
	//std::map<MapPoint*, std::vector<Factor*>> mpFactor;

private:


};
