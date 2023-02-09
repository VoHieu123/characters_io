#include "charactersio_uart.hpp"

CharactersIOUart::CharactersIOUart(UartReceivedHandler aUartByteReceivedHandler, void* aContext)
	: mCharactersIOContext(aContext)
	, mByteReceivedHandler(aUartByteReceivedHandler)
{

}

CharactersIOUart::~CharactersIOUart(void)
{

}

inline CharactersIOErrorCode_t CharactersIOUart::PushDataToUartTx(uint8_t aByte)
{
	return static_cast<ImplClass *>(this)->_PushDataToUartTx(aByte);
}

inline CharactersIOErrorCode_t CharactersIOUart::Init(void* aPlatformUartHandle)
{
	return static_cast<ImplClass *>(this)->_Init(aPlatformUartHandle, this);
}

UartContext_t* CharactersIOUart::GetInstance(void* aPlatformUartHandle)
{
	UartContext_t* instance = nullptr;

	for (uint8_t i = 0; i < mInstanceCount; ++i)
	{
		if (CharactersIOUart::mCreatedInstances[i].mUartHandle == aPlatformUartHandle)
		{
			instance = &CharactersIOUart::mCreatedInstances[i];
			break;
		}
	}

	return instance;
}
