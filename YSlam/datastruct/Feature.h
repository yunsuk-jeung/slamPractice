#pragma once

#include <iostream>
#include <Eigen/Eigen>

namespace dan {
namespace datastruct {
	class Feature {
	public:

		Feature();
		Feature(float u, float v, int lvl);

		~Feature();

		int lvl = 0;
		
		// uv coordinate in lvl 0
		Eigen::Vector2f uv;
		
	private:



	};
}
}