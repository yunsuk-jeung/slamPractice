/*==============================================================================
Copyright 2017 Maxst, Inc. All Rights Reserved.
==============================================================================*/


#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>
using namespace std;

#define MAX_MESSAGE 10000

#ifdef _WIN32
#include <windows.h>
#endif

#define _DEBUG

#ifdef _PRINT_LOG

void Log(const char *format, ...)
{
	char szTempBuf[MAX_MESSAGE];
	va_list marker;

	va_start(marker, format);
	vsprintf(szTempBuf, format, marker);

	va_end(marker);

#if defined(_WIN32)
	sprintf_s(szTempBuf, MAX_MESSAGE, "%s\n", szTempBuf);
	printf_s("SensorFusion-Native: %s\n", szTempBuf);

	OutputDebugStringA(szTempBuf);
	//
	//    //printf("%s\n", szTempBuf);
	//    ofstream myfile;
	//    myfile.open("./log.txt", std::ofstream::app);
	//    myfile << szTempBuf << endl;
	//    myfile.close();

#elif defined(MAXSTAR_LINUX)
	printf("SensorFusion-Native: %s\n", szTempBuf);
	std::cout << szTempBuf << std::endl;
#else
	printf("SensorFusion-Native: %s\n", szTempBuf);
//    ofstream myfile;
//    myfile.open("./log.txt", std::ofstream::app);
//    myfile << szTempBuf << endl;
//    myfile.close();
#endif
}

void LogD(const char *format, ...)
{

	char szTempBuf[MAX_MESSAGE];
	va_list marker;

	va_start(marker, format);
	vsprintf(szTempBuf, format, marker);

	va_end(marker);

#if defined(_WIN32)
	sprintf_s(szTempBuf, MAX_MESSAGE, "%s\n", szTempBuf);
	printf("SensorFusion-Native: %s\n", szTempBuf);

	OutputDebugStringA(szTempBuf);
#elif defined(__linux__)
	printf("SensorFusion-Native: %s\n", szTempBuf);
#else
//    printf("MaxstAR-Native: %s\n", szTempBuf);
//    ofstream myfile;
//    myfile.open("./log.txt", std::ofstream::app);
//    myfile << szTempBuf << endl;
//    myfile.close();
#endif

}

void LogE(const char *format, ...)
{

#ifdef _DEBUG
	char szTempBuf[MAX_MESSAGE];
	va_list marker;

	va_start(marker, format);
	vsprintf(szTempBuf, format, marker);

	va_end(marker);

#if defined(_WIN32)
	sprintf_s(szTempBuf, MAX_MESSAGE, "%s\n", szTempBuf);
	printf("SensorFusion-Native: %s\n", szTempBuf);

	OutputDebugStringA(szTempBuf);
#elif defined(__linux__)
	printf("SensorFusion-Native: %s\n", szTempBuf);
#else
	//    printf("MaxstAR-Native: %s\n", szTempBuf);
	//    ofstream myfile;
	//    myfile.open("./log.txt", std::ofstream::app);
	//    myfile << szTempBuf << endl;
	//    myfile.close();
#endif

#endif
}
#endif
