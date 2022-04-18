#pragma once

#include <iostream>
#include "datastruct/Image.hpp"
#include "datastruct/ImagePyramid.hpp"

namespace dan {

	class Frame {
	public:

		Frame();
		~Frame();

		void createImagePyramid(datastruct::ImagePtr _imagePyramid);
		void createGradientPyramid();

	private:
		datastruct::ImagePyramid imagePyramid;
		datastruct::ImagePyramid xGradientPyramid;
		datastruct::ImagePyramid yGradientPyramid;

		bool isKeyFrame = false;
	};
}