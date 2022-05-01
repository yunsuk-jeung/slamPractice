#pragma once 

#include <iostream>
#include "datastruct/Types.h"


typedef unsigned char Byte;

namespace dan {

enum class DataType {
	EUROC = 0,
	ANDROID = 1,
	IOS = 2,
};

enum class TrackerType {
	OPTICAL_FLOW = 0,
	SEMI_DENSE = 1,
	DENSE_SPARSE = 2,
};

class Parameters {
public:
	Parameters();
	~Parameters();

	static Parameters* getInstance();
	static void deleteInstance();

	bool setParameters(std::string parameterpath);

};
extern DataType DATA_TYPE;
extern TrackerType TRACKER_TYPE;
extern int PYRAMID_LEVEL;
extern int X_GRID_NUM;
extern int Y_GRID_NUM;

/* Debug Obtion */
extern int SHOW_THRESHOLD_IMAGE;

}