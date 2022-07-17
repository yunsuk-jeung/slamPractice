#include "Graph/Graph.h"
#include "Graph/Frame.h"
#include "Graph/InvPoint.h"

Graph::Graph() {}
Graph::~Graph() {

	for (int i = 0; i < frames.size(); i++) {
		delete frames[i];
	}
	for (int i = 0; i < mapPoints.size(); i++) {
		delete mapPoints[i];
	}

}

void Graph::addFactor(Frame* frame, InvPoint* mapPoint) {
	//frames.push_back(frame);
	//frame->id = frames.size();

	//mapPoints.push_back(mapPoint);
	//mapPoint->id = mapPoints.size();
}


