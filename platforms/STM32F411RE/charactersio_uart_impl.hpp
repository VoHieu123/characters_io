#pragma once
#include <stdint.h>
#include "charactersio_uart.hpp"

/* Good practice: Interface class and Implementation class */
class CharactersIOUartImpl final: public CharactersIOUart
{
	friend CharactersIOUart;

public:

private:
	CharactersIOErrorCode_t _PushDataToUartTx(uint8_t aByte);
	CharactersIOErrorCode_t _Init(void* aPlatformUartHandle, CharactersIOUart *aIOStreamUartHandle);
};
