#pragma once

#include <iostream>

namespace dan {
	namespace datastruct {
		class ImagePyramid;
	}
	class Frame {
	public:

		Frame();
		~Frame();

		void setImagePyramid(std::shared_ptr<datastruct::ImagePyramid> _imagePyramid);
	private:
		std::shared_ptr<datastruct::ImagePyramid> imagePyramids;
		bool isKeyFrame = false;
	};
}