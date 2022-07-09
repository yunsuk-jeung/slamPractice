#include "Graph/Graph.h"

Graph::Graph() {}
Graph::~Graph() {

	for (int i = 0; i < frames.size(); i++) {
		delete frames[i];
	}
	for (int i = 0; i < mapPoints.size(); i++) {
		delete mapPoints[i];
	}

}

void Graph::addFactor(Frame* frame, MapPoint* mapPoint) {
	frames.push_back(frame);
	mapPoints.push_back(mapPoint);

}


