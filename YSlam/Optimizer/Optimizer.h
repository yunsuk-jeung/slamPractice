#pragma once

#include <vector>


class Frame;
class InvPoint;
class Optimizer {
public:
	void static InitialBA(Frame* prev, Frame* currF, std::vector<InvPoint*>& vMapPoints);

};

