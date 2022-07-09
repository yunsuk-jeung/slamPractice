#pragma once

namespace datastruct {

	enum class ColorFormat
	{
		RGB888 = 1,
		YUV420sp = 2,
		YUV420 = 3,
		YUV420_888 = 4,
		GRAY8 = 5,
		RGBA8888 = 6,
		YUV420sp_iOS = 7,
	};

	enum class SensorFormat
	{
		GYR_RADIAN = 1,
		GYR_DEGREE = 2,
		ACC_G = 3,
		ACC_METRIC = 4,
		ACC_G_IOS = 5,
	};

	enum class FrameSize
	{
		FRAME_2560 = 2560,
		FRAME_1920 = 1920,
		FRAME_1440 = 1440,
		FRAME_1280 = 1280,
		FRAME_640 = 640,
	};

	enum class TrackingState
	{
		NONE = 0,
		FAIL = 1,
		DANGEROUS = 2,
		TRACKING = 3,
		INITIALIZE = 4,
	};

	enum class FeatureVisible {
		LEFT = 1,
		RIGHT = 2,
		STEREO = 3,
	};

	enum class ScreenOrientation //
	{
		UNKNOWN = 0,
		PORTRAIT = 1,
		PORTRAIT_UP = 1,
		PORTRAIT_DOWN = 2,
		LANDSCAPE = 3,
		LANDSCAPE_LEFT = 3,
		LANDSCAPE_RIGHT = 4,
	};
}

