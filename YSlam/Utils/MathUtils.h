#pragma once

#include <Eigen/Dense>

class MathUtils
{
public:
	template <typename Derived>
	static Eigen::Quaternion<typename Derived::Scalar> dAxisTodQ(const Eigen::MatrixBase<Derived>& theta)
	{
		typedef typename Derived::Scalar Scalar_t;

		Eigen::Quaternion<Scalar_t> dq;
		Eigen::Matrix<Scalar_t, 3, 1> half_theta = theta;
		half_theta /= static_cast<Scalar_t>(2.0);
		dq.w() = static_cast<Scalar_t>(1.0);
		dq.x() = half_theta.x();
		dq.y() = half_theta.y();
		dq.z() = half_theta.z();
		return dq;
	}

};


