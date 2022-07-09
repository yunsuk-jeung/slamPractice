#pragma once

#include <iostream>
#include <Eigen/Eigen>

namespace datastruct {

class Feature {
public:

	Feature() {}
	Feature(int x, int y, int _lvl) {
		uv.x() = x;
		uv.y() = y;
		lvl = _lvl;
	}

	Feature(float x, float y, float _lvl) {
		uv.x() = x;
		uv.y() = y;
		lvl = _lvl;
	}

	//Feature(float u, float v, int lvl);

	~Feature() {}

	int lvl = 0;


	Eigen::Vector2f uv;

private:



};


}
