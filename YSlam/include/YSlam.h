#pragma once


#include <iostream>

#include "Parameters/Parameters.h"

#if defined _WIN32
#define ESLAM_API __declspec(dllexport)
#else
#define ESLAM_API
#endif




	enum class ColorFormat;
	class ESLAM_API Eslam {

	public:

		Eslam();
		~Eslam();

		static Eslam* getInstance();
		static void deleteInstance();

		bool init(std::string dataPath, std::string parameterPath);
		void setNewFrame(Byte* data, int length, int width, int height, datastruct::ColorFormat format, unsigned long long int timestmap);

	private:
		

	};
