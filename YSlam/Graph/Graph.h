#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Parameters/Parameters.h"


class Frame;
class InvPoint;
class Factor {
public:

	Factor() {}
	~Factor() {}

	Frame* frame;
	InvPoint* mapPoint;

private:

};



class Frame;
class InvPoint;
class Graph {

public:

	Graph();
	~Graph();
	void addFactor(Frame* frame, InvPoint* mapPoint);



	//node
	std::vector<Frame*> frames;
	std::vector<InvPoint*> mapPoints;
	
	std::vector<Factor*> factors;

	//factor
	//std::map<Frame*, std::list<Factor*>> kfFactors;
	//std::map<InvPoint*, std::vector<Factor*>> mpFactor;

private:

	int newMpId = 0;
	int newFId = 0;


};
