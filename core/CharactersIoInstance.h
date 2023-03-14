#pragma once

#include "CharactersIoErrorCode.h"
#include "CharactersIoSerialConnection.h"
#include "stdint.h"

namespace CharactersIo {

namespace DeviceLayer { /* Todo: It must not be in this namespace */

class CharactersIoSerialConnectionImpl;

class CharactersIoInstance
{
	friend CharactersIoSerialConnectionImpl;
public:
//    void Initialize(void);

	/* Todo: Implement features-enable function */
	int32_t Write(const void *aBuffer, size_t aCount);
	int32_t Read(void *aBuffer, size_t aCount);

private:

	enum {
		kCharsIoBufferSize = 256u
	};

	~CharactersIoInstance(void);
	CharactersIoInstance(void *aPlatformHandle);

	bool PushDataToBuffer(uint8_t aByte);
	bool PushDataToBuffer(const uint8_t *aBuffer, uint16_t aCount);
	bool PopDataFromBuffer(uint8_t *aOutputByte);

	void *mPlatformHandle;
	/* Todo: Constant values should be in config file */
	uint8_t   mBuffer[kCharsIoBufferSize];
	uint16_t  mBufferReadIndex;
	uint16_t  mBufferWriteIndex;
	uint16_t  mBufferCount;
	bool      mIsSoftwareFlowControlEnabled;
	bool      mIsRemoteXON;
	bool 	    mCrShouldBeCrlf;
	bool      mIsXON;

	void *GetPlatformHandle(void) const;

	/* Todo: Inherit from non-copyable instance */
	// No copy, move or assignment.
	CharactersIoInstance(const CharactersIoInstance &)  = delete;
	CharactersIoInstance(const CharactersIoInstance &&) = delete;
	CharactersIoInstance &operator=(const CharactersIoInstance &) = delete;
};

} // DeviceLayer
} // CharactersIo
