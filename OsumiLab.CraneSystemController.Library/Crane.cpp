#include "stdafx.h"
#include "Crane.h"

/// <summary>
/// Ctor
/// </summary>
Crane::Crane() : Crane(CraneSettings::kAioDeviceName, CraneSettings::kCntDeviceName) {
	// No code
}

/// <summary>
/// Delegate Ctor
/// </summary>
/// <param name="device_name"></param>
Crane::Crane(char *aio_name, char *cnt_name) {
	// Initialize CNT-3208M-PE device
	this->InitAio(aio_name);
	this->InitCnt(cnt_name);

	// Initialize all axis
	this->SetCoarseAxis();
	this->SetFineAxis();
	this->SetRopeAxis();

	// Initialize Vimba system and cameras
	this->VimbaSystemStartup();
	this->InitCameras();
}

/// <summary>
/// Dtor
/// </summary>
Crane::~Crane() {
	this->Exit();

	this->_cam_scene_right.reset();
	this->_cam_rope_x.reset();
	this->_cam_rope_y.reset();

	this->VimbaSystemShutdown();
}

/// <summary>
/// Method that returns the coarse axis object
/// </summary>
/// <returns></returns>
CraneCoarseAxis& Crane::CoarseAxis() {
	return std::ref(this->_coarse_axis);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CraneFineAxis Crane::FineAxis() const {
	return this->_fine_axis;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CraneRopeAxis Crane::Rope() const {
	return this->_rope;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CraneSceneCamera& Crane::RightSceneCamera() const {
	return (*this->_cam_scene_right);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CraneRopeCamera& Crane::XRopeCamera() const {
	return (*this->_cam_rope_x);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CraneRopeCamera& Crane::YRopeCamera() const {
	return (*this->_cam_rope_y);
}

/// <summary>
/// Method to exit properly the crane system
/// </summary>
void Crane::Exit() {
	// Set 0[V] to all analog IO channels
	memset(this->_aio_data, 0.0, sizeof(float) * kAioMaxChannel);
	AioMultiAoEx(this->_aio_id, kAioMaxChannel, this->_aio_data);

	AioExit(this->_aio_id);
	CntExit(this->_cnt_id);
}

/// <summary>
/// Method to initialize the analog IO device on CNT-3208M-PE
/// </summary>
/// <param name="device_name"></param>
void Crane::InitAio(char *device_name) {
	AioInit(device_name, &this->_aio_id);
	AioResetDevice(this->_aio_id);
	AioSetAoRangeAll(this->_aio_id, PM10);

	// 0[V] on all AIO mosules
	memset(this->_aio_data, 0.0, sizeof(float) * kAioMaxChannel);
	AioMultiAoEx(this->_aio_id, kAioMaxChannel, this->_aio_data);

	this->_aio_data[7] = 5.0;		// Voltage for fine axis Y
	this->_aio_data[8] = 5.0;		// Voltage for fine axis X
	//this->_aio_data[9] = 5.0;		// Voltage for Coarse axis Y (makes noise)
	this->_aio_data[10] = 5.0;		// Voltage for Coarse axis X 
	this->_aio_data[11] = 5.0;		// Voltage for rope

	AioMultiAoEx(this->_aio_id, kAioMaxChannel, this->_aio_data);

	// Set all Axis channels to 0[V]
	for (size_t i = 0; i <= 4; i++)
		AioSingleAoEx(this->_aio_id, i, 0);
}

/// <summary>
/// Method to initialize the counter device on CNT-3208M-PE
/// </summary>
/// <param name="device_name"></param>
void Crane::InitCnt(char *device_name) {
	CntInit(device_name, &this->_cnt_id);

	for (int channel = 0; channel < kCntMaxChannel; channel++) {
		CntSetZMode(this->_cnt_id, channel, CNT_ZPHASE_NOT_USE);
		CntSetZLogic(this->_cnt_id, channel, CNT_ZLOGIC_POSITIVE);
		CntSelectChannelSignal(this->_cnt_id, channel, CNT_ZLOGIC_POSITIVE);
		CntSetCountDirection(this->_cnt_id, channel, CNT_DIR_UP);
		CntSetOperationMode(this->_cnt_id, channel, CNT_MODE_2PHASE, CNT_MUL_X4, CNT_CLR_ASYNC);
		CntSetDigitalFilter(this->_cnt_id, channel, 0);
	}
}

/// <summary>
/// 
/// </summary>
void Crane::InitCameras(void) {
	this->_cam_scene_right = std::make_unique<CraneSceneCamera>
		(this->_vimbasystem, CraneSettings::kGuppyCameraPID);

	this->_cam_rope_x = std::make_unique<CraneRopeCamera>
		(this->_vimbasystem, CraneSettings::kPikeXCameraPID);

	this->_cam_rope_y = std::make_unique<CraneRopeCamera>
		(this->_vimbasystem, CraneSettings::kPikeYCameraPID);
}

/// <summary>
/// Method to setup the coarse axis
/// </summary>
void Crane::SetCoarseAxis(void) {
	this->_coarse_axis.SetAioCntIDs(this->_aio_id, this->_cnt_id);
	this->_coarse_axis.SetAioChannels(
		CraneSettings::kXCoarseAxisAioChannel,
		CraneSettings::kYCoarseAxisAioChannel,
		-1);
	this->_coarse_axis.SetCntChannels(
		CraneSettings::kXCoarseAxisCntChannel,
		CraneSettings::kYCoarseAxisCntChannel,
		-1);
}

/// <summary>
/// Method to setup the fine axis
/// </summary>
void Crane::SetFineAxis(void) {
	this->_fine_axis.SetAioCntIDs(this->_aio_id, this->_cnt_id);
	this->_fine_axis.SetAioChannels(
		CraneSettings::kXFineAxisAioChannel,
		CraneSettings::kYFineAxisAioChannel,
		-1);
	this->_fine_axis.SetCntChannels(
		CraneSettings::kXFineAxisCntChannel,
		CraneSettings::kYFineAxisCntChannel,
		-1);
}

/// <summary>
/// Method to setup the rope axis
/// </summary>
void Crane::SetRopeAxis(void) {
	this->_rope.SetAioCntIDs(this->_aio_id, this->_cnt_id);
	this->_rope.SetAioChannels(-1, -1, CraneSettings::kZRopeAxisAioChannel);
	this->_rope.SetCntChannels(-1, -1, CraneSettings::kZRopeAxisCntChannel);
}

/// <summary>
/// 
/// </summary>
void Crane::VimbaSystemStartup(void) {
	if (this->_vimbasystem.Startup() != VmbErrorSuccess)
		std::cout << "ERROR: Unable to startup Vimba System" << std::endl;
}

/// <summary>
/// 
/// </summary>
void Crane::VimbaSystemShutdown(void) {
	if (this->_vimbasystem.Shutdown() != VmbErrorSuccess)
		std::cout << "ERROR: Unable to shutdown Vimba System" << std::endl;
}
