#include "datastruct/Frame.h"

namespace dan{
	Frame::Frame() {}
	Frame::~Frame() {}

	void Frame::setImagePyramid(std::shared_ptr<datastruct::ImagePyramid> _imagePyramid) {
		imagePyramids = _imagePyramid;
	}

}