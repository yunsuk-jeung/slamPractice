#pragma once 
#include <iostream>

namespace dan {

enum class TrackerType;
	class Frame;
	class Initializer{
	public:
		virtual ~Initializer();

		static Initializer* createInitializer(TrackerType trackerType);
		virtual bool process(Frame* frame) = 0;

	protected:
		Initializer();
		Frame* prev = nullptr;
		Frame* curr = nullptr;

	private:
	};

}