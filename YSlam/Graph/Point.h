#pragma once

#include <iostream>
#include <Eigen/Eigen>


class Frame;

class Point {
public:

	Point() {
		//invDepth = (T)1.0;
		//Pwx.setZero();
	}

	virtual ~Point() {}

	//// first
	//void setPosition(Eigen::Vector2f uv, int _lvl) {

	//	Pwx.head<2>() = uv.cast<T>();
	//	Pwx.z() = 1.0;
	//	invDepth = 1.0;

	//	lvl = _lvl;
	//}

	//void addObserve(Eigen::Vector2i uv) {
	//
	//}

	//double getInvDepth() {
	//	return 1.0 / Pwx.z();
	//}



	//int id = -1;
	//Eigen::Matrix<T,3,1> Pwx;
	//Eigen::Matrix3d Pwx;
protected: 

	//int lvl = -1;

	//T invDepth;

	std::vector<Frame* > frames;
	
};

