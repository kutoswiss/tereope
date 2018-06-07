#include "stdafx.h"
#include "CraneSettings.h"

namespace CraneSettings
{
	// Device names
	char *kAioDeviceName = "AIO000";
	char *kCntDeviceName = "CNT000";

	// AIO Channels
	const int kXCoarseAxisAioChannel = 3;
	const int kYCoarseAxisAioChannel = 2;
	const int kXFineAxisAioChannel = 1;
	const int kYFineAxisAioChannel = 0;
	const int kZRopeAxisAioChannel = 4;

	// CNT Channels
	const int kXCoarseAxisCntChannel = 3;
	const int kYCoarseAxisCntChannel = 2;
	const int kXFineAxisCntChannel = 0;
	const int kYFineAxisCntChannel = 1;
	const int kZRopeAxisCntChannel = 4;

	// Cameras settings
	const char *kGuppyCameraPID = "DEV_0xA47010F086149";
	const char *kPikeXCameraPID = "DEV_0xA470110075023";
	const char *kPikeYCameraPID = "DEV_0xA470110075024";

	// Preset values
	const int kCntPresetValue = 2000000;
}