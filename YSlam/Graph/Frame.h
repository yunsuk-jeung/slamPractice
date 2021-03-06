#pragma once

#include <iostream>
#include "datastruct/Feature.h"
#include "datastruct/Image.hpp"
#include "datastruct/ImagePyramid.hpp"
#include "datastruct/Pose.h"
#include "Parameters/Parameters.h"
#include "Camera/Camera.h"



	class datastruct::Pose;
	class Frame {
	public:

		Frame();
		~Frame();

		void setCamera(Camera& _cam);
		Camera& getCamera();

		void createImagePyramid(datastruct::ImagePtr _imagePyramid);
		void createGradientPyramid();
		void createMagGradientPyramid();

		int getPyramidLevel();

		datastruct::Pose* getPwc();

		void getCeresParameter(double* pose);
		void setCeresParameter(double* pose);

		datastruct::ImagePyramid* getImagePyramid();
		datastruct::ImagePyramid* getXGradPyramid();
		datastruct::ImagePyramid* getYGradPyramid();
		datastruct::ImagePyramid* getMagGradientPyramid();

		std::vector<std::vector< datastruct::Feature >> features;

		int id = -1;
	private:
		Camera cam;
		datastruct::ImagePyramid imagePyramid;
		datastruct::ImagePyramid xGradientPyramid;
		datastruct::ImagePyramid yGradientPyramid;
		datastruct::ImagePyramid magGradientPyramid;

		datastruct::Pose Pwc;


		bool isKeyFrame = false;
	};

