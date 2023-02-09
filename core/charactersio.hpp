#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "../utilities/charactersio_config.hpp"
#include "../utilities/charactersio_error_code.hpp"
#include "charactersio_uart.hpp"

class CharactersIO
{
public:
    void Initialize(void);
    CharactersIO(void);
    int32_t Write(const void *aBuffer, size_t aCount);
    int32_t Read(void *aBuffer, size_t aCount);
    ~CharactersIO(void);

private:
    enum {
        kStreamBufferSize = CHARACTERSIO_BUFFER_SIZE
    };

    static inline void HandleReceivedByte(void *aContext, uint8_t aReceivedByte);
    void               HandleReceivedByte(uint8_t aReceivedByte);

    CharactersIOUart mUartHandle;
    uint8_t            mBuffer[kStreamBufferSize];
    uint16_t           mBufferReadIndex;
    uint16_t           mBufferWriteIndex;
    uint16_t           mBufferCount;
    bool               mIsSoftwareFlowControlEnabled;
    bool               mIsRemoteXON;
    bool 						   mCrShouldBeCrlf;
    bool               mIsXON;
};