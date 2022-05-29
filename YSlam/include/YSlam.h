#pragma once

#include <iostream>

#include "Parameters/Parameters.h"

#if defined _WIN32
#define YSLAM_API __declspec(dllexport)
#else
#define YSLAM_API
#endif


namespace dan {


	enum class ColorFormat;
	class YSLAM_API YSlam {

	public:

		YSlam();
		~YSlam();

		static YSlam* getInstance();
		static void deleteInstance();

		bool init(std::string dataPath, std::string parameterPath);
		void setNewFrame(Byte* data, int length, int width, int height, datastruct::ColorFormat format, unsigned long long int timestmap);

	private:
		

	};
}