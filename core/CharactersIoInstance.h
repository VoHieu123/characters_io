#pragma once

#include "CharactersIoErrorCode.h"
#include "CharactersIoSerialConnection.h"

namespace CharactersIo {

namespace DeviceLayer {

class CharactersIoSerialConnectionImpl;

class CharactersIoInstance
{
	friend CharactersIoSerialConnectionImpl;
//public:
//    void Initialize(void);
//
//    int32_t Write(const void *aBuffer, size_t aCount);
//    int32_t Read(void *aBuffer, size_t aCount);
//
//private:
//    enum {
//        kStreamBufferSize = CHARACTERSIO_BUFFER_SIZE
//    };
//
//    static inline void HandleReceivedByte(void *aContext, uint8_t aReceivedByte);
//    void               HandleReceivedByte(uint8_t aReceivedByte);
//
//    CharactersIoSerialConnection mUartHandle;
//    uint8_t            mBuffer[kStreamBufferSize];
//    uint16_t           mBufferReadIndex;
//    uint16_t           mBufferWriteIndex;
//    uint16_t           mBufferCount;
//    bool               mIsSoftwareFlowControlEnabled;
//    bool               mIsRemoteXON;
//    bool 						   mCrShouldBeCrlf;
//    bool               mIsXON;
private:
		~CharactersIoInstance(void);
		/* Todo: Add configs */
		CharactersIoInstance(void *aPlatformHandle);

		void *mPlatformhandle;
};

} // CharactersIo
} // DeviceLayer
