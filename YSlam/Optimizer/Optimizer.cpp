#include "Graph/InvPoint.h"
#include "Graph/Frame.h"

#include "Optimizer/Optimizer.h"
#include "Optimizer/Parameterization/PoseLocalParameterization.h"
#include "Optimizer/Factor/Auto_Direct_InvDepth_Frame.h"

#include "ceres/ceres.h"


void Optimizer::InitialBA(Frame* prevF, Frame* currF,std::vector<InvPoint*>& vMapPoints) {

	double** pose = new double*[1];
	pose[0] = new double[7];

	auto mapPointSize = vMapPoints.size();
	double** invDepth = new double*[mapPointSize];
	for (auto i = 0; i < mapPointSize; i++) {
		invDepth[i][0] = vMapPoints[i]->getInvDepth();
	}

	currF->getCeresParameter(pose[0]);
	Camera& cam = currF->getCamera();


	ceres::Problem problem;
	ceres::LocalParameterization* localParameterization = new EigenQuatParameterization();
	
	problem.AddParameterBlock(pose[0], 7, localParameterization);
	
	float* I0 = (float*)prevF->getImagePyramid()->images[0].data;
	float* I1 = (float*)currF->getImagePyramid()->images[0].data;
	float* dIx = (float*)currF->getXGradPyramid()->images[0].data;
	float* dIy = (float*)currF->getYGradPyramid()->images[0].data;


	int imWidth = currF->getImagePyramid()->images[0].cols;
	int imHeight = currF->getImagePyramid()->images[0].rows;


	for (auto i = 0; i < mapPointSize; i++) {
		problem.AddParameterBlock(invDepth[i], 1);

		ceres::CostFunction* cost = Auto_Direct_InvDepth_Frame::create(cam, imWidth, imHeight, prevF->getPwc()->toSE3() ,vMapPoints[i]->Pwx, I0, I1, dIx, dIy);
		problem.AddResidualBlock(cost, NULL, pose[0], invDepth[i]);

	}

	


	delete[] pose[0];
	delete[] pose;
	

	for (auto i = 0; i < mapPointSize; i++) {
		delete[] invDepth[i];
	}
	delete[] invDepth;
}



