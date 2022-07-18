#include "Graph/InvPoint.h"
#include "Graph/Frame.h"

#include "Optimizer/Optimizer.h"
#include "Optimizer/Parameterization/PoseLocalParameterization.h"
#include "Optimizer/Factor/Auto_Direct_InvDepth_Frame.h"

#include "ceres/ceres.h"

typedef Eigen::Matrix<float, 10, 1> Vector10f;
typedef Eigen::Matrix<float, 8, 1> Vector8f;

int patch[8][2] = { {0,-2},	  {-1,-1},	   {1,-1},		{-2,0},		 {0,0},		  {2,0},	   {-1,1},		{0,2}
};


void Optimizer::InitialBA(Frame * prevF, Frame * currF,std::vector<InvPoint*>&vMapPoints) {

	Camera& cam = currF->getCamera();

	float* I0 = (float*)prevF->getImagePyramid()->images[0].data;

	float* I1 = (float*)currF->getImagePyramid()->images[0].data;

	float* dIx = (float*)currF->getXGradPyramid()->images[0].data;
	float* dIy = (float*)currF->getYGradPyramid()->images[0].data;

	int imWidth = currF->getImagePyramid()->images[0].cols;
	int imHeight = currF->getImagePyramid()->images[0].rows;

	Vector10f* JBuffer = new Vector10f[imWidth * imHeight];
	Vector10f* JBufferC = new Vector10f[imWidth * imHeight]; // cache


	int mapPointSize = vMapPoints.size();

	for (int i = 0; i < mapPointSize; i++) {

		Vector8f dp0;
		Vector8f dp0;
		Vector8f dp1;
		Vector8f dp2;
		Vector8f dp3;
		Vector8f dp4;
		Vector8f dp5;
		Vector8f dp6;
		Vector8f dp7;
		Vector8f dd;
		Vector8f r;

		JBufferC[i].setZero();

	}


	delete[] JBuffer;
	delete[] JBufferC;

}



