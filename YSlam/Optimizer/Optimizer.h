#pragma once

#include <vector>


class Frame;
class MapPoint;
class Optimizer {
public:
	void static InitialBA(Frame* currF, std::vector<MapPoint*>& vMapPoints);

};

