#include "Graph/InvPoint.h"
#include "Graph/Frame.h"

#include "Optimizer/Optimizer.h"
#include "Optimizer/Parameterization/PoseLocalParameterization.h"
#include "Optimizer/Factor/Auto_Direct_InvDepth_Frame.h"

#include "ceres/ceres.h"


void Optimizer::InitialBA(Frame* prevF, Frame* currF,std::vector<InvPoint*>& vMapPoints) {

	Camera& cam = currF->getCamera();

	float* I0 = (float*)prevF->getImagePyramid()->images[0].data;

	float* I1 = (float*)currF->getImagePyramid()->images[0].data;
	
	float* dIx = (float*)currF->getXGradPyramid()->images[0].data;
	float* dIy = (float*)currF->getYGradPyramid()->images[0].data;

	int imWidth = currF->getImagePyramid()->images[0].cols;
	int imHeight = currF->getImagePyramid()->images[0].rows;
	


}



