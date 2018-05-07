#include "stdafx.h"
#include "Crane.h"

/// <summary>
/// 
/// </summary>
Crane::Crane() : Crane(AIO_DEVICE_NAME, CNT_DEVICE_NAME) {
	// No code
}

/// <summary>
/// 
/// </summary>
/// <param name="device_name"></param>
Crane::Crane(char *aio_name, char *cnt_name) {
	this->InitAio(aio_name);
	this->InitCnt(cnt_name);
	this->_coarse_axis.SetAioCntIDs(this->_aio_id, this->_cnt_id);
}

/// <summary>
/// 
/// </summary>
Crane::~Crane() {
	this->Exit();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CraneCoarseAxis Crane::CoarseAxis() const {
	return this->_coarse_axis;
}

/// <summary>
/// 
/// </summary>
void Crane::Exit() {
	memset(this->_aio_data, 0.0, sizeof(float) * kAioMaxChannel);
	AioMultiAoEx(this->_aio_id, kAioMaxChannel, this->_aio_data);

	AioExit(this->_aio_id);
	CntExit(this->_cnt_id);
}

/// <summary>
/// 
/// </summary>
/// <param name="device_name"></param>
void Crane::InitAio(char *device_name) {
	AioInit(device_name, &this->_aio_id);
	AioResetDevice(this->_aio_id);
	AioSetAoRangeAll(this->_aio_id, PM10);

	memset(this->_aio_data, 0.0, sizeof(float) * kAioMaxChannel);
	AioMultiAoEx(this->_aio_id, kAioMaxChannel, this->_aio_data);

	for (int i = 7; i <= 11; i++)
		this->_aio_data[i] = 5.0;

	AioMultiAoEx(this->_aio_id, kAioMaxChannel, this->_aio_data);
}

/// <summary>
/// 
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
