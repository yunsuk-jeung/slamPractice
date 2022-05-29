#pragma once

#include <iostream>
#include <Eigen/Eigen>

namespace dan {
namespace datastruct {
class Frame;
class MapPoint {
public:

	MapPoint() {}
	~MapPoint() {}

private: 

	int lvl = 0;

	double invDepth;
	Eigen::Vector3d point;

	std::list<Frame* > frames;
};


}
}