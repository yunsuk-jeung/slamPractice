#pragma once 
#include <iostream>

namespace dan {
	class Tracker {
	public:
		Tracker();
		virtual ~Tracker();

		virtual void process() = 0;

	private:

	};

}