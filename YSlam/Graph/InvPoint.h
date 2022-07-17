#pragma once

#include <iostream>
#include <Eigen/Eigen>
#include <Graph/Point.h>

class Frame;

template<typename T>
class InvPoint_ : Point_<T> {
public:

	InvPoint_() {
		invDepth = 1.0;
		Pwx.setZero();
	}
	~InvPoint_() {}

	// first
	void setPosition(Eigen::Vector2f uv, int _lvl) {

		Pwx.head<2>() = uv.cast<T>();
		Pwx.z() = 1.0;
		invDepth = 1.0;

		lvl = _lvl;
	}

	void addObserve(Eigen::Vector2i uv) {
	
	}

	double getInvDepth() {
		return 1.0 / Pwx.z();
	}


	Eigen::Matrix<T,3,1> Pwx;

	int id = -1;

private: 

	int lvl = -1;

	T invDepth;

	std::list<Frame* > frames;
	
};

typedef InvPoint_<double> InvPoint;
typedef InvPoint_<float> InvPointf;

