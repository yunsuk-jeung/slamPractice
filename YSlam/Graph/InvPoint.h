#pragma once

#include <iostream>
#include <Eigen/Eigen>
#include <Graph/Point.h>

class Frame;

class InvPoint : Point {
public:

	InvPoint() {
		invDepth = 1.0;
		Pwx.setZero();
	}
	~InvPoint() {}

	// first
	void setPosition(Eigen::Matrix<float,2,1> uv, int _lvl) {

		Pwx.head<2>() = uv;
		Pwx.z() = 1.0;
		invDepth = 1.0;

		lvl = _lvl;
	}

	void addObserve(Eigen::Vector2i uv) {
	
	}

	double getInvDepth() {
		return 1.0 / Pwx.z();
	}


	Eigen::Vector3f Pwx;

	int id = -1;

private: 

	int lvl = -1;

	float invDepth;

	std::list<Frame* > frames;
	
};

