#pragma once
#include <CharactersIoSerialConnection.h>
#include "CharactersIo.h"
#include <stdint.h>

#define MAX_SERIAL_CONNECTION_COUNT 20u
#define DMA_BUFFER_SIZE             8u

class CharactersIoSerialConnectionImpl final: public CharactersIoSerialConnection
{
	friend CharactersIoSerialConnection;

public:
	CharactersIoSerialConnectionImpl(void);
	~CharactersIoSerialConnectionImpl(void);

private:

	CharactersIOErrorCode_t _IsConnectedWithCharactersIo(void) const {return mCharactersIo != nullptr; }
	CharactersIOErrorCode_t _IsConnectedWithPlatform(void) const {return mPlatformIndex != -1; }
	CharactersIOErrorCode_t _ConnectWithCharactersIo(CharactersIo const &aCharactersIo);
	CharactersIOErrorCode_t _ConnectWithPlatform(void *aPlatform);

	void* _GetPlatform(void) const { return mPlatformIndex != -1 ? instanceInfo[mPlatformIndex].mPlatform : nullptr; }
	CharactersIo *_GetCharactersIo(void) const { return mCharactersIo; }

	CharactersIOErrorCode_t _StartDma(void);
	CharactersIOErrorCode_t _StopDma(void);

	static CharactersIoSerialConnection &_GetInstance(void *aPlatform);
	static CharactersIoSerialConnection &_GetInstance(CharactersIo const &aCharactersIo);

	CharactersIOErrorCode_t _PushData(uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode_t _PushData(uint8_t aByte);
	CharactersIOErrorCode_t _HandleReceivedData(uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode_t _HandleReceivedData(uint8_t aByte);

	CharactersIOErrorCode_t _Deinit(void);
	CharactersIOErrorCode_t _Init(void);

	typedef struct InstanceInfo
	{
		CharactersIoSerialConnection &mInstance;
		void *mPlatform;
	} InstanceInfo_t;

	static InstanceInfo_t instanceInfo[MAX_SERIAL_CONNECTION_COUNT];
	static uint8_t mPlatformCount = 0;

	int16_t               mPlatformIndex = -1;
	CharactersIo          *mCharactersIo;
	uint8_t               mDmaBuffer[DMA_BUFFER_SIZE];
	uint16_t              mDmaBufferOldPos = 0;

	ImplClass * Impl() { return static_cast<ImplClass *>(this); }
};
