#include "InverseDepthProjectionFactor.h"

namespace maxstMapper
{

	Eigen::Matrix2d InverseDepthProjectionFactor::sw_sqrt_info = Eigen::Matrix2d::Identity();
	Eigen::Matrix2d InverseDepthProjectionFactor::loop_sqrt_info;
	Eigen::Matrix2d InverseDepthProjectionFactor::fba_sqrt_info;

	double InverseDepthProjectionFactor::sum_t;

	InverseDepthProjectionFactor::InverseDepthProjectionFactor(const Eigen::Vector3d &_pts_i, const Eigen::Vector3d &_pts_j, const OptimizeState _optimizeState) : pts_i(_pts_i), pts_j(_pts_j), optimizeState(_optimizeState)
	{
#ifdef UNIT_SPHERE_ERROR
		Eigen::Vector3d b1, b2;
		Eigen::Vector3d a = pts_j.normalized();
		Eigen::Vector3d tmp(0, 0, 1);
		if (a == tmp)
			tmp << 1, 0, 0;
		b1 = (tmp - a * (a.transpose() * tmp)).normalized();
		b2 = a.cross(b1);
		tangent_base.block<1, 3>(0, 0) = b1.transpose();
		tangent_base.block<1, 3>(1, 0) = b2.transpose();
#endif

		//sqrt_info *= 10;
	};

	bool InverseDepthProjectionFactor::Evaluate(double const *const *parameters, double *residuals, double **jacobians) const
	{
		Eigen::Matrix2d sqrt_info;

		if (optimizeState == OptimizeState::SLIDING_WINDOW_OPTIMIZE) {
			sqrt_info = sw_sqrt_info;
		}
		else if (optimizeState == OptimizeState::LOOP_OPTIMIZE) {
			sqrt_info = loop_sqrt_info;

		}
		else {
			sqrt_info = fba_sqrt_info;
		}

		Timer tic_toc;
		Eigen::Vector3d Pi(parameters[0][0], parameters[0][1], parameters[0][2]);
		Eigen::Quaterniond Qi(parameters[0][6], parameters[0][3], parameters[0][4], parameters[0][5]);

		Eigen::Vector3d Pj(parameters[1][0], parameters[1][1], parameters[1][2]);
		Eigen::Quaterniond Qj(parameters[1][6], parameters[1][3], parameters[1][4], parameters[1][5]);

		double inv_dep_i = parameters[2][0];

		Eigen::Vector3d pts_camera_i = pts_i / inv_dep_i;
		Eigen::Vector3d pts_w = Qi * pts_camera_i + Pi;
		Eigen::Vector3d pts_camera_j = Qj.inverse() * (pts_w - Pj);
		Eigen::Map<Eigen::Vector2d> residual(residuals);

#ifdef UNIT_SPHERE_ERROR 
		residual = tangent_base * (pts_camera_j.normalized() - pts_j.normalized());
#else
		double dep_j = pts_camera_j.z();
		residual = (pts_camera_j / dep_j).head<2>() - pts_j.head<2>();
#endif

		residual = sqrt_info * residual;

		if (jacobians)
		{
			Eigen::Matrix3d Ri = Qi.toRotationMatrix();
			Eigen::Matrix3d Rj = Qj.toRotationMatrix();
			Eigen::Matrix3d RjTranspose = Rj.transpose();
			Eigen::Matrix<double, 2, 3> reduce(2, 3);
#ifdef UNIT_SPHERE_ERROR
			double norm = pts_camera_j.norm();
			Eigen::Matrix3d norm_jaco;
			double x1, x2, x3;
			x1 = pts_camera_j(0);
			x2 = pts_camera_j(1);
			x3 = pts_camera_j(2);
			norm_jaco << 1.0 / norm - x1 * x1 / pow(norm, 3), -x1 * x2 / pow(norm, 3), -x1 * x3 / pow(norm, 3),
				-x1 * x2 / pow(norm, 3), 1.0 / norm - x2 * x2 / pow(norm, 3), -x2 * x3 / pow(norm, 3),
				-x1 * x3 / pow(norm, 3), -x2 * x3 / pow(norm, 3), 1.0 / norm - x3 * x3 / pow(norm, 3);
			reduce = tangent_base * norm_jaco;
#else
			reduce << 1. / dep_j, 0, -pts_camera_j(0) / (dep_j * dep_j),
				0, 1. / dep_j, -pts_camera_j(1) / (dep_j * dep_j);
#endif
			reduce = sqrt_info * reduce;

			if (jacobians[0])
			{
				Eigen::Map<Eigen::Matrix<double, 2, 7, Eigen::RowMajor>> jacobian_pose_i(jacobians[0]);

				Eigen::Matrix<double, 3, 6> jaco_i;
				jaco_i.leftCols<3>() = RjTranspose;
				jaco_i.rightCols<3>() = RjTranspose * Ri * -Utility::skewSymmetric(pts_camera_i);

				jacobian_pose_i.leftCols<6>() = reduce * jaco_i;
				jacobian_pose_i.rightCols<1>().setZero();
			}

			if (jacobians[1])
			{
				Eigen::Map<Eigen::Matrix<double, 2, 7, Eigen::RowMajor>> jacobian_pose_j(jacobians[1]);

				Eigen::Matrix<double, 3, 6> jaco_j;
				jaco_j.leftCols<3>() = -RjTranspose;
				jaco_j.rightCols<3>() = Utility::skewSymmetric(pts_camera_j);

				jacobian_pose_j.leftCols<6>() = reduce * jaco_j;
				jacobian_pose_j.rightCols<1>().setZero();
			}
			if (jacobians[2])
			{
				Eigen::Map<Eigen::Vector2d> jacobian_feature(jacobians[2]);
				jacobian_feature = reduce * RjTranspose * Ri * pts_i * -1.0 / (inv_dep_i * inv_dep_i);
			}
		}
		sum_t += tic_toc.end();

		return true;
	}

	void InverseDepthProjectionFactor::check(double **parameters)
	{
		Eigen::Matrix2d sqrt_info;

		if (optimizeState == OptimizeState::SLIDING_WINDOW_OPTIMIZE) {
			sqrt_info = sw_sqrt_info;
		}
		else if (optimizeState == OptimizeState::LOOP_OPTIMIZE) {
			sqrt_info = loop_sqrt_info;

		}
		else {
			sqrt_info = fba_sqrt_info;
		}
		double *res = new double[15];
		double **jaco = new double *[4];
		jaco[0] = new double[2 * 7];
		jaco[1] = new double[2 * 7];
		jaco[2] = new double[2 * 7];
		jaco[3] = new double[2 * 1];
		Evaluate(parameters, res, jaco);

		Eigen::Vector3d Pi(parameters[0][0], parameters[0][1], parameters[0][2]);
		Eigen::Quaterniond Qi(parameters[0][6], parameters[0][3], parameters[0][4], parameters[0][5]);

		Eigen::Vector3d Pj(parameters[1][0], parameters[1][1], parameters[1][2]);
		Eigen::Quaterniond Qj(parameters[1][6], parameters[1][3], parameters[1][4], parameters[1][5]);

		Eigen::Vector3d tic(parameters[2][0], parameters[2][1], parameters[2][2]);
		Eigen::Quaterniond qic(parameters[2][6], parameters[2][3], parameters[2][4], parameters[2][5]);
		double inv_dep_i = parameters[3][0];

		Eigen::Vector3d pts_camera_i = pts_i / inv_dep_i;
		Eigen::Vector3d pts_imu_i = qic * pts_camera_i + tic;
		Eigen::Vector3d pts_w = Qi * pts_imu_i + Pi;
		Eigen::Vector3d pts_imu_j = Qj.inverse() * (pts_w - Pj);
		Eigen::Vector3d pts_camera_j = qic.inverse() * (pts_imu_j - tic);


		Eigen::Vector2d residual;
#ifdef UNIT_SPHERE_ERROR 
		residual = tangent_base * (pts_camera_j.normalized() - pts_j.normalized());
#else
		double dep_j = pts_camera_j.z();
		residual = (pts_camera_j / dep_j).head<2>() - pts_j.head<2>();
#endif
		residual = sqrt_info * residual;

		puts("num");
		std::cout << residual.transpose() << std::endl;

		const double eps = 1e-6;
		Eigen::Matrix<double, 2, 19> num_jacobian;
		for (int k = 0; k < 19; k++)
		{
			Eigen::Vector3d Pi(parameters[0][0], parameters[0][1], parameters[0][2]);
			Eigen::Quaterniond Qi(parameters[0][6], parameters[0][3], parameters[0][4], parameters[0][5]);

			Eigen::Vector3d Pj(parameters[1][0], parameters[1][1], parameters[1][2]);
			Eigen::Quaterniond Qj(parameters[1][6], parameters[1][3], parameters[1][4], parameters[1][5]);

			Eigen::Vector3d tic(parameters[2][0], parameters[2][1], parameters[2][2]);
			Eigen::Quaterniond qic(parameters[2][6], parameters[2][3], parameters[2][4], parameters[2][5]);
			double inv_dep_i = parameters[3][0];

			int a = k / 3, b = k % 3;
			Eigen::Vector3d delta = Eigen::Vector3d(b == 0, b == 1, b == 2) * eps;

			if (a == 0)
				Pi += delta;
			else if (a == 1)
				Qi = Qi * Utility::deltaQ(delta);
			else if (a == 2)
				Pj += delta;
			else if (a == 3)
				Qj = Qj * Utility::deltaQ(delta);
			else if (a == 4)
				tic += delta;
			else if (a == 5)
				qic = qic * Utility::deltaQ(delta);
			else if (a == 6)
				inv_dep_i += delta.x();

			Eigen::Vector3d pts_camera_i = pts_i / inv_dep_i;
			Eigen::Vector3d pts_imu_i = qic * pts_camera_i + tic;
			Eigen::Vector3d pts_w = Qi * pts_imu_i + Pi;
			Eigen::Vector3d pts_imu_j = Qj.inverse() * (pts_w - Pj);
			Eigen::Vector3d pts_camera_j = qic.inverse() * (pts_imu_j - tic);

			Eigen::Vector2d tmp_residual;
#ifdef UNIT_SPHERE_ERROR 
			tmp_residual = tangent_base * (pts_camera_j.normalized() - pts_j.normalized());
#else
			double dep_j = pts_camera_j.z();
			tmp_residual = (pts_camera_j / dep_j).head<2>() - pts_j.head<2>();
#endif
			tmp_residual = sqrt_info * tmp_residual;
			num_jacobian.col(k) = (tmp_residual - residual) / eps;
		}
		std::cout << num_jacobian << std::endl;
	}

}
