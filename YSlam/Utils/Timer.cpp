#pragma once

#include "Timer.h"
#include "Logger.h"

namespace dan
{

	const int REPORT_PERIOD = 10;

	std::map<TIME_PERIOD, TimerElement> elementList;
	int reportCount = 10;

	const char* TIME_PERIOD_NAME[100]
	{
		"IMPYRAMID",
		"GRADIENT_PYRAMID",
		"MAG_PYRAMID",
		"HISTOGRAM",
		"EXTRACTION"
	};

	void Statistics::startTimer(TIME_PERIOD id) {
		std::map<TIME_PERIOD, TimerElement>::iterator it = elementList.find(id);
		if (it == elementList.end()) {
			elementList[id] = TimerElement();
			it = elementList.find(id);
		}

		it->second.timer.start();
		it->second.callCount++;
	}

	double Statistics::stopTimer(TIME_PERIOD id) {
		std::map<TIME_PERIOD, TimerElement>::iterator it = elementList.find(id);

		double time = it->second.timer.end();
		it->second.totalTime += time;

		if (it->second.minTime > time) {
			it->second.minTime = time;
		}

		if (it->second.maxTime < time) {
			it->second.maxTime = time;
		}

		return time;
	}

	double Statistics::meanTime(TIME_PERIOD id) {
		std::map<TIME_PERIOD, TimerElement>::iterator it = elementList.find(id);

		return it->second.totalTime / it->second.callCount;
	}

	void Statistics::report(std::vector<TIME_PERIOD> ids) {

		if (reportCount != REPORT_PERIOD) {
			reportCount++;
			return;
		}

		reportCount = 0;

		printf("--------------------------------------------------------\n");
		for (int i = 0; i < ids.size(); i++) {
			std::map<TIME_PERIOD, TimerElement>::iterator it = elementList.find(ids[i]);

			if (it == elementList.end()) {
				continue;
			}

			double mean = it->second.totalTime / it->second.callCount;

			LogD("%s Mean time : %lf ms", TIME_PERIOD_NAME[ids[i]], mean);
			LogD("%s Max  time : %lf ms", TIME_PERIOD_NAME[ids[i]], it->second.maxTime);
			LogD("%s Min  time : %lf ms", TIME_PERIOD_NAME[ids[i]], it->second.minTime);
			printf("\n");
		}
		printf("--------------------------------------------------------\n");
	}

}