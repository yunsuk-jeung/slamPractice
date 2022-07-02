#include "Optimizer/Optimizer.h"
#include "Graph/MapPoint.h"
#include "Graph/Frame.h"

#include "ceres/ceres.h"

namespace dan {



void Optimizer::InitialBA(Frame* currF,std::vector<MapPoint*>& vMapPoints) {

	double** pose = new double*[1];
	pose[0] = new double[7];

	currF->getCeresParameter(pose[0]);

	delete[] pose[0];
	delete[] pose;
}



}