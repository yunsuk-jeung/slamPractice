#pragma once

#include <ceres/ceres.h>
#include <Eigen/Dense>

#include <Camera/Camera.h>

class Auto_Direct_InvDepth_Frame
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		Auto_Direct_InvDepth_Frame(Camera& _cam, int _width, int _height, const Eigen::Matrix4d& _Mcw, Eigen::Matrix<double, 3, 1>& Pwx, const float* _I0, const float* _I1, const float* dIx, const float* dIy) :I0(_I0), I1(_I1) {
		cam = _cam;
			fx = cam.fx;
			fy = cam.fy;
			cx = cam.cx;
			cy = cam.cy;
			k1 = cam.k1;
			k2 = cam.k2;
			p1 = cam.p1;
			p2 = cam.p2;
		width = _width;
		height = _height;
		Rcw = _Mcw.block<3, 3>(0, 0);
		Tcw = _Mcw.block<3, 1>(0, 3);

		Eigen::Vector3d Pcx = Rcw * Pwx + Tcw;
		n_uv0 = Pcx / Pcx.z();

		cam.projection(n_uv0, uv0);

		int u = uv0.x();
		int v = uv0.y();

		i0 = I0[v * width + u];

	}


	template <typename T>
	bool operator()(const T* const Pwc, const T* invDepth, T* residuals) const {
		Eigen::Matrix<T, 3, 1> Twc(Pwc[0], Pwc[1], Pwc[2]);
		Eigen::Quaternion<T> Qwc(Pwc[6], Pwc[3], Pwc[4], Pwc[5]);

		T idepth = invDepth[0];

		Eigen::Matrix<T, 3, 1> n_uv_T = n_uv0.cast<T>();


		Eigen::Matrix<T, 3, 1> Pc0x = n_uv_T * idepth;
		Eigen::Matrix<T, 3, 1> Pc1x = Qwc.inverse() * (Pc0x - Twc);


		//for (auto i = 0; i < 8; i++) {
		Eigen::Vector2d uv1;
		Eigen::Vector3d Pc1xd;// = Pc1x.cast<double>();
		int a = 1;
		//Pc1xd.x() = (double)Pc1x.x();
		//Pc1xd.y() = (double)Pc1x.y();
		//Pc1xd.z() = (double)Pc1x.z();

		//cam.constProjection(Pc1xd, uv1);
		//int u = uv1.x();
		//int v = uv1.y();

		//int idx = v * width + u;

		//if (idx < 0 || idx > width * height) {
		//	residuals[0] = (T)0;
		//	residuals[1] = (T)0;
		//	residuals[2] = (T)0;
		//	residuals[3] = (T)0;
		//	residuals[4] = (T)0;
		//	residuals[5] = (T)0;
		//	residuals[6] = (T)0;
		//	residuals[7] = (T)0;


		//}
		//else {
		//	float i1 = I1[idx];
		//	residuals[0] = (T)i0 - (T)i1;
		//	residuals[1] = (T)i0 - (T)i1;
		//	residuals[2] = (T)i0 - (T)i1;
		//	residuals[3] = (T)i0 - (T)i1;
		//	residuals[4] = (T)i0 - (T)i1;
		//	residuals[5] = (T)i0 - (T)i1;
		//	residuals[6] = (T)i0 - (T)i1;
		//	residuals[7] = (T)i0 - (T)i1;
		//}


		//}



		//Eigen::Map<Eigen::Matrix<T, 8, 1>> residual(residuals);

		//residual = (pts_camera_j / dep_j).head<2>() - pts_j.head<2>();


		return true;
	}

	static ceres::CostFunction* create(Camera& _cam, int width, int height, const Eigen::Matrix4d& Mcw, Eigen::Matrix<double, 3, 1>& Pwx, const float* I0, const float* I1, const float* dIx, const float* dIy) {
		return (new ceres::AutoDiffCostFunction<Auto_Direct_InvDepth_Frame, 8, 7, 1>(new Auto_Direct_InvDepth_Frame(_cam, width, height, Mcw, Pwx, I0, I1, dIx, dIy)));
	}

	Camera cam;

	double fx;
	double fy;
	double cx;
	double cy;
	double k1;
	double k2;
	double p1;
	double p2;

	int width = 0;
	int height = 0;

	Eigen::Matrix3d Rcw;
	Eigen::Vector3d Tcw;
	Eigen::Matrix<double, 3, 1> n_uv0;
	Eigen::Vector2d uv0;
	float i0;
	const float* I0;
	const float* I1;

};
