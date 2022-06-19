#pragma once

#include <iostream>
#include "datastruct/Feature.h"
#include "datastruct/Image.hpp"
#include "datastruct/ImagePyramid.hpp"
#include "Parameters/Parameters.h"
#include "Camera/Camera.h"

namespace dan {


	class Frame {
	public:

		Frame();
		~Frame();

		void setCamere(Camera& _cam);

		void createImagePyramid(datastruct::ImagePtr _imagePyramid);
		void createGradientPyramid();
		void createMagGradientPyramid();

		int getPyramidLevel();


		datastruct::ImagePyramid* getImagePyramid();
		datastruct::ImagePyramid* getXGradPyramid();
		datastruct::ImagePyramid* getYGradPyramid();
		datastruct::ImagePyramid* getMagGradientPyramid();

		std::vector<std::vector< datastruct::Feature >> features;
	private:
		Camera cam;
		datastruct::ImagePyramid imagePyramid;
		datastruct::ImagePyramid xGradientPyramid;
		datastruct::ImagePyramid yGradientPyramid;
		datastruct::ImagePyramid magGradientPyramid;


		bool isKeyFrame = false;
	};

}