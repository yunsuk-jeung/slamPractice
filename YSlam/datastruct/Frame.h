#pragma once

#include <iostream>
#include "datastruct/Image.hpp"
#include "datastruct/ImagePyramid.hpp"
#include "Parameters/Parameters.h"

namespace dan {

	class Frame {
	public:

		Frame();
		~Frame();

		void createImagePyramid(datastruct::ImagePtr _imagePyramid);
		void createGradientPyramid();
		int getPyramidLevel();
		datastruct::ImagePyramid* getImagePyramid();
		datastruct::ImagePyramid* getXGradPyramid();
		datastruct::ImagePyramid* getYGradPyramid();


	private:
		datastruct::ImagePyramid imagePyramid;
		datastruct::ImagePyramid xGradientPyramid;
		datastruct::ImagePyramid yGradientPyramid;

		bool isKeyFrame = false;
	};
}