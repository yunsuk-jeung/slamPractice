#pragma once

#include <vector>

namespace dan {

class Frame;
class MapPoint;
class Optimizer {
public:
	void static InitialBA(Frame* currF, std::vector<MapPoint*>& vMapPoints);

};

}