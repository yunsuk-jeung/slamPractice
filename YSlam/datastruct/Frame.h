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
		void createMagGradientPyramid();

		int getPyramidLevel();


		datastruct::ImagePyramid* getImagePyramid();
		datastruct::ImagePyramid* getXGradPyramid();
		datastruct::ImagePyramid* getYGradPyramid();
		datastruct::ImagePyramid* getMagGradientPyramid();


	private:
		datastruct::ImagePyramid imagePyramid;
		datastruct::ImagePyramid xGradientPyramid;
		datastruct::ImagePyramid yGradientPyramid;
		datastruct::ImagePyramid magGradientPyramid;

		bool isKeyFrame = false;
	};
}