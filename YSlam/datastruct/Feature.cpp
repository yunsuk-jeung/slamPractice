#include "datastruct/Feature.h"

namespace dan {
namespace datastruct{
Feature::Feature() {}
Feature::Feature(float u, float v, int _lvl) {
	uv.x() = u;
	uv.y() = v;
	lvl = _lvl;
}
Feature::~Feature() {}
	
}
}