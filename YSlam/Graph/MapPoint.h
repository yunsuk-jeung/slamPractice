#pragma once

#include <iostream>
#include <Eigen/Eigen>


class Frame;
class MapPoint {
public:

	MapPoint() {
		invDepth = 1.0;
		Pcx.setZero();
	}
	~MapPoint() {}

	// first
	void setPosition(Eigen::Vector2f uv, int _lvl) {

		Pcx.head<2>() = uv.cast<double>();
		Pcx.z() = 1.0;
		invDepth = 1.0;

		lvl = _lvl;
	}

private: 

	int lvl = -1;

	double invDepth;
	Eigen::Vector3d Pcx;

	std::list<Frame* > frames;
};



