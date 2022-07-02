#pragma once 
#include <iostream>

namespace dan {

enum class TrackerType;
	class Frame;
	class Extractor;
	class Graph;
	class Initializer{
	public:
		virtual ~Initializer();

		static Initializer* createInitializer(TrackerType trackerType);


		void setGraph(Graph* _graph);
		virtual bool process(Frame* frame) = 0;

	protected:
		Initializer();
		Frame* prevF = nullptr;
		Frame* currF = nullptr;
		Extractor* extractor = nullptr;
		Graph* graph = nullptr;

	private:
	};

}