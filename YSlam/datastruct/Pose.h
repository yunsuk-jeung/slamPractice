#pragma once

#include <Eigen/Eigen>

namespace dan {
namespace datastruct {

class Pose {
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		Pose() {
		position.setZero();
		rotation.setIdentity();
	}

	Pose(const Eigen::Matrix<double, 4, 4>& M) {
		this->fromMatrix4d(M);
	}

	void fromMatrix4d(const Eigen::Matrix4d& M) {

		rotation = Eigen::Quaterniond(M.block<3, 3>(0, 0));
		position = M.block<3, 1>(0, 3);

	}

	Pose operator * (const Pose& rhs) {

		return multiply(*this, rhs);
	}

	Pose& operator *= (const Pose& rhs) {

		*this = multiply(*this, rhs);
		return *this;
	}

	Pose multiply(const Pose& a, const Pose& b) {
		
		Eigen::Quaterniond rotation_ = a.rotation * b.rotation;
		Eigen::Vector3d position_ = a.rotation * b.position + a.position;

		Pose out;
		out.rotation = rotation_;
		out.position = position_;

		return out;
	}

	Pose inverse() {
		Pose out;
		out.rotation = rotation.inverse();
		out.position = -(out.rotation * position);
	}

	Pose inverseInPlace() {
		rotation = rotation.inverse();
		position = -(rotation * position);
	}

	Eigen::Matrix4d toSE3() {
		Eigen::Matrix4d out;
		out.setZero();
		out.block<3, 3>(0, 0) = rotation.toRotationMatrix();
		out.block<3, 1>(0, 3) = position;
	
	}


	~Pose() {}


	Eigen::Vector3d position;
	Eigen::Quaterniond rotation;

};

}
}
