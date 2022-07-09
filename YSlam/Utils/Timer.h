#pragma once

#include <ctime>
#include <cstdlib>
#include <chrono>
#include <map>
#include <vector>

#include <float.h>


	class Timer
	{
	public:
		Timer() {
			start();
		}

		void start() {
			startTime = std::chrono::system_clock::now();
		}

		double end() {
			endTime = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = endTime - startTime;

			return elapsed_seconds.count() * 1000;
		}

	private:
		std::chrono::time_point<std::chrono::system_clock> startTime;
		std::chrono::time_point<std::chrono::system_clock> endTime;
	};

	enum TIME_PERIOD
	{
		IMPYRAMID,
		GRADIENT_PYRAMID,
		MAG_PYRAMID,
		HISTOGRAM,
		EXTRACTION
	};

	struct TimerElement {
		Timer timer;
		double totalTime;
		double minTime = DBL_MAX;
		double maxTime = DBL_MIN;
		int callCount = 0;
	};

	class Statistics
	{
	public:
		static void startTimer(TIME_PERIOD id);
		static double stopTimer(TIME_PERIOD id);
		static double meanTime(TIME_PERIOD id);
		static void report(std::vector<TIME_PERIOD> ids);

	private:
		Statistics() {};
		~Statistics() {};
	};

