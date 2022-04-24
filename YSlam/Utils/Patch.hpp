#pragma once 

#include <iostream>


namespace dan {
	class Patch;

	class Patch {
	public:

		Patch();
		~Patch();

	private:

		int size = 0;

		int x = 0;
		int y = 0;


	};

}