#pragma once

#include <iostream>
#include <Eigen/Eigen>


class Frame;

template<typename T>
class Point_ {
public:

	Point_() {
		//invDepth = (T)1.0;
		//Pwx.setZero();
	}

	virtual ~Point_() {}

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

protected: 

	//int lvl = -1;

	//T invDepth;

	//std::list<Frame* > frames;
	
};

typedef Point_<double> Point;
typedef Point_<float> Pointf;

