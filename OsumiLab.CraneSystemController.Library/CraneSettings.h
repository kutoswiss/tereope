#pragma once

namespace CraneSettings 
{
	// Device names
	extern char *kAioDeviceName;
	extern char *kCntDeviceName;

	// AIO Channels
	extern const int kXCoarseAxisAioChannel;
	extern const int kYCoarseAxisAioChannel;
	extern const int kXFineAxisAioChannel;
	extern const int kYFineAxisAioChannel;
	extern const int kZRopeAxisAioChannel;

	// CNT Channels
	extern const int kXCoarseAxisCntChannel;
	extern const int kYCoarseAxisCntChannel;
	extern const int kXFineAxisCntChannel;
	extern const int kYFineAxisCntChannel;
	extern const int kZRopeAxisCntChannel;

	// Cameras settings
	extern const char *kGuppyCameraPID;
	extern const char *kPikeXCameraPID;
	extern const char *kPikeYCameraPID;

	// Preset values
	extern const int kCntPresetValue;
}