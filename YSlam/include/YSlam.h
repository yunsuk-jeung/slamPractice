#pragma once

#include <iostream>


#if defined WIN32
#define YSLAM_API __declspec(dllexport)
#else
#define YSLAM_API
#endif

namespace YSlam {
	class YSLAM_API YSlam {

	public:
		
		YSlam();
		~YSlam();

		static YSlam* getInstance();
		static void deleteInstance();

		bool init();

	};
}