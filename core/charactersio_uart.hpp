#pragma once
#include <stdint.h>

#include "../utilities/charactersio_error_code.hpp"

#define MAX_INSTANCE_NUM 5u
#define DMA_BUFFER_SIZE       8u

class CharactersIOUart;

typedef void (*UartReceivedHandler)(void *aContext, uint8_t aReceivedByte);

/* Todo: this struct should be replaced because it's not platform-independent */
typedef struct UartContext
{
	CharactersIOUart *mCharactersIOUart;
	void         *mUartHandle;
	uint8_t      mDmaBuffer[DMA_BUFFER_SIZE];
	uint16_t     mDmaBufferOldPos;
} UartContext_t;

class CharactersIOUart
{
	using ImplClass = CharactersIOUartImpl;

public:
	CharactersIOUart(UartReceivedHandler aUartByteReceivedHandler, void* aContext);
	~CharactersIOUart(void);

	CharactersIOErrorCode_t PushDataToUartTx(uint8_t aByte);
	CharactersIOErrorCode_t Init(void* aPlatformUartHandle);

	static UartContext_t *GetInstance(void* aPlatformUartHandle);
	void                          *mCharactersIOContext;
	UartReceivedHandler           mByteReceivedHandler;

private:

protected:
	static uint8_t       mInstanceCount;
	static UartContext_t mCreatedInstances[MAX_INSTANCE_NUM];
	void                 *mUartHandle;
};
