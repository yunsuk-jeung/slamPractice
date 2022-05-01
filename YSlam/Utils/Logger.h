/*==============================================================================
 Copyright 2017 Maxst, Inc. All Rights Reserved.
 ==============================================================================*/


#pragma once


#define _PRINT_LOG
#define DEBUG
#define DEBUG2
//#define __IOS__
//#define MAXSTAR_LINUX
#ifdef __ANDROID__
#include <android/log.h>
#ifndef LOG_TAG
#define  LOG_TAG "MappingModule-Native"
#endif


#ifdef _PRINT_LOG

#ifdef DEBUG
//#error DEBUG is defined
#define LogD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
//#error DEBUG is not defined
#define LogD(...)
#endif
#define Log(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LogE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else
#define Log(...)
#define LogD(...)

//#define LogE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LogE(...)
#endif
#elif defined(_WIN32) || defined(__MacOS__) || defined(__linux__)

#ifdef _PRINT_LOG
extern void Log(const char* szFormat, ...);
extern void LogD(const char* szFormat, ...);
extern void LogE(const char* szFormat, ...);
#else
#define Log(...)
#define LogD(...)
#define LogE(...)
//extern void LogE(const char* szFormat, ...);
#endif

#elif defined(__IOS__)
    #ifdef _PRINT_LOG

    #ifdef DEBUG
    #define LogD(...) {printf("SensorFusion-Native: "); printf(__VA_ARGS__);  printf("\n");}
    #else
    #define LogD(...)
    #endif
    #define Log(...) {printf("SensorFusion-Native: "); printf(__VA_ARGS__);  printf("\n");}
    #define LogE(...) {printf("SensorFusion-Native: "); printf(__VA_ARGS__);  printf("\n");}

#else
#define Log(...)
#define LogD(...)
#define LogE(...)
#endif

#else
#define LogD(...)
#define Log(...) //printf(__VA_ARGS__);
#define LogE(...)
#endif


#ifdef __IOS__

#include <stdio.h>

#ifdef DEBUG
#define LogD(...) {printf("MappingModule-Native: "); printf(__VA_ARGS__);  printf("\n");}
#else
#define LogD(...)
#endif

#define Log(...) {printf("MappingModule-Native: "); printf(__VA_ARGS__);  printf("\n");}
#define LogE(...) {printf("MappingModule-Native: "); printf(__VA_ARGS__);  printf("\n");}

#endif

static void LogMatrix4x4(double* matrix, const char* name) {
	LogE("Matrix  %s", name);

	LogE("%lf  %lf  %lf  %lf ", matrix[0], matrix[4], matrix[8], matrix[12]);
	LogE("%lf  %lf  %lf  %lf ", matrix[1], matrix[5], matrix[9], matrix[13]);
	LogE("%lf  %lf  %lf  %lf ", matrix[2], matrix[6], matrix[10], matrix[14]);
	LogE("%lf  %lf  %lf  %lf ", matrix[3], matrix[7], matrix[11], matrix[15]);
}

static void LogMatrix4x4f(float* matrix, const char* name) {
	LogE("Matrix  %s", name);

	LogE("%lf  %lf  %lf  %lf ", matrix[0], matrix[4], matrix[8], matrix[12]);
	LogE("%lf  %lf  %lf  %lf ", matrix[1], matrix[5], matrix[9], matrix[13]);
	LogE("%lf  %lf  %lf  %lf ", matrix[2], matrix[6], matrix[10], matrix[14]);
	LogE("%lf  %lf  %lf  %lf ", matrix[3], matrix[7], matrix[11], matrix[15]);
}
