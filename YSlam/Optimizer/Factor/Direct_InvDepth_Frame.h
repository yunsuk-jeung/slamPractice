#pragma once

#include <ceres/ceres.h>
#include <Eigen/Dense>
#include "Utility/utility.h"
#include "Utility/Timer.h"

#include "Config/Config.h"

namespace maxstMapper
{

	class InverseDepthProjectionFactor : public ceres::SizedCostFunction<2, 7, 7, 1>
	{
	public:
		InverseDepthProjectionFactor(const Eigen::Vector3d &_pts_i, const Eigen::Vector3d &_pts_j, const OptimizeState _optimizeState);
		virtual bool Evaluate(double const *const *parameters, double *residuals, double **jacobians) const;
		void check(double **parameters);

		Eigen::Vector3d pts_i, pts_j;
		Eigen::Matrix<double, 2, 3> tangent_base;

		static Eigen::Matrix2d sw_sqrt_info;
		static Eigen::Matrix2d loop_sqrt_info;
		static Eigen::Matrix2d fba_sqrt_info;

		static double sum_t;

		OptimizeState optimizeState;

	};

}