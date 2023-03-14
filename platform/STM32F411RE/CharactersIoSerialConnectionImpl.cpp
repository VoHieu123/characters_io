/* Todo: License */


#include "stm32f4xx.h"

/* Todo: define USE_HAL_DRIVER */

#include "CharactersIoSerialConnection.h"
#include "CharactersIoSerialConnectionImpl.h"
#include "CharactersIoErrorCode.h"

/* Todo: each dma_buffer should be correspond with 1 chars instance, right now setting to 1 instance
 * so no need to seperate them */
#define DMA_BUFFER_SIZE 16u /* Todo: should be in config.h */
static uint16_t dmaBufferOldPos = 0;
static uint8_t dmaBuffer[DMA_BUFFER_SIZE] = {0};

/* Todo: Define in config file */
#define MAX_CHARSIO_NUM 1u /* Todo: This value equals the number of serial connection */

namespace CharactersIo {

namespace DeviceLayer {

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	switch (huart->ErrorCode)
	{
		case HAL_UART_ERROR_NONE:
			break;
		case HAL_UART_ERROR_ORE:
			/* Todo: this should also be platform-dependent */
			__attribute__((fallthrough));
		case HAL_UART_ERROR_FE:
			__attribute__((fallthrough));
		case HAL_UART_ERROR_NE:
			__attribute__((fallthrough));
		case HAL_UART_ERROR_PE:
			__attribute__((fallthrough));
		case HAL_UART_ERROR_DMA:
			__attribute__((fallthrough));
		/* Undefined errors are treated as HAL_UART_ERROR_DMA */
		default:
//      printf("Error: HAL_UART_ERROR: %d\n", huart->ErrorCode);
			dmaBufferOldPos = 0;

			HAL_UARTEx_ReceiveToIdle_DMA(huart, dmaBuffer, DMA_BUFFER_SIZE);
			break;
	}
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	CharactersIoSerialConnectionImpl &impl = GetCharactersIoSerialConnectionImpl();
	uint32_t index             = 0;
	uint32_t numReceivedChars  = 0;

	if (size != dmaBufferOldPos)
	{
		if (size > dmaBufferOldPos)
		{
			numReceivedChars = size - dmaBufferOldPos;
			impl.HandleReceivedData(huart, &dmaBuffer[dmaBufferOldPos + index], numReceivedChars);
		}
		else
		{
			numReceivedChars = DMA_BUFFER_SIZE - dmaBufferOldPos;
			impl.HandleReceivedData(huart, &dmaBuffer[dmaBufferOldPos + index], numReceivedChars);
			if (size > 0)
			{
				impl.HandleReceivedData(huart, &dmaBuffer[index], size);
			}
		}
		dmaBufferOldPos = size;
  }
}

/* Anonymous namespace prevents accessibility of outside files */
namespace {
alignas(alignof(CharactersIoInstance)) char sCharactersIoRaw[sizeof(CharactersIoInstance) * MAX_CHARSIO_NUM];
} // namespace

CharactersIoSerialConnectionImpl::CharactersIoSerialConnectionImpl(void)
{
	sInstanceCount = 0;

}

CharactersIoSerialConnectionImpl::~CharactersIoSerialConnectionImpl(void)
{

}

void CharactersIoSerialConnectionImpl::_Init(void)
{

}


void CharactersIoSerialConnectionImpl::_RemoveConnection(void *aPlatformHandle)
{

}

CharactersIoInstance
&CharactersIoSerialConnectionImpl::_CreateNewConnection(void *aPlatformHandle,
																												struct CharactersIoConfigs &aIoConfig)
{
	/* Todo: Check if mInstaneCount > MAX */
	CharactersIoInstance &newInstance =
		*(new (&sCharactersIoRaw[sInstanceCount])
			CharactersIoInstance(aPlatformHandle));

	sInstanceCount++;
	/* Todo: Should have a function to start the transaction instead */
	HAL_UARTEx_ReceiveToIdle_DMA((UART_HandleTypeDef *) aPlatformHandle, dmaBuffer, DMA_BUFFER_SIZE);

	return newInstance;
}

/* Todo: Prevent the initialization of CharactersIo instance outside this function */
CharactersIoInstance
&CharactersIoSerialConnectionImpl::_CreateNewConnection(void *aPlatformHandle)
{
	/* Todo: Set default config */
	struct CharactersIoConfigs config =
	{

	};

	return _CreateNewConnection(aPlatformHandle, config);
}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PushData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount)
{
	UART_HandleTypeDef *huart = static_cast<UART_HandleTypeDef *> (aPlatformHandle);

	return HAL_UART_Transmit(huart, aBuffer, aByteCount, HAL_MAX_DELAY) == HAL_OK ? ERROR_NONE : ERROR_TX_BUSY;
}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PushData(void *aPlatformHandle, uint8_t aByte)
{
	/* Todo: Check aPlatformHandle */
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *) aPlatformHandle;
	CharactersIOErrorCode error = ERROR_NONE;

	/* Check that a Tx process is not already ongoing */
	if (huart->gState == HAL_UART_STATE_READY)
	{
		if(huart->Lock == HAL_LOCKED)
		{
			 return ERROR_TX_BUSY;
		}
		else
		{
			 huart->Lock = HAL_LOCKED;
		}

		huart->ErrorCode = HAL_UART_ERROR_NONE;
		huart->TxXferSize = 1;
		huart->TxXferCount = 1;
		huart->gState = HAL_UART_STATE_BUSY_TX;

		huart->Lock = HAL_UNLOCKED;

		while (!(huart->Instance->SR & UART_FLAG_TXE)) {}
		huart->Instance->DR = (uint32_t) aByte;
		huart->TxXferCount--;

		huart->gState = HAL_UART_STATE_READY;

		error = ERROR_NONE;
	}
	else
	{
		error = ERROR_TX_BUSY;
	}

	return error;
}

CharactersIoInstance *CharactersIoSerialConnectionImpl::PlatformHandleToInstance(void *aPlatformHandle) const
{
	CharactersIoInstance *instance;

	for (size_t i = 0; i < sInstanceCount; i++)
	{
		instance = &(((CharactersIoInstance *)sCharactersIoRaw)[i]);
		if (instance->GetPlatformHandle() == aPlatformHandle)
		{
			break;
		}
	}

	return instance;
}

/* Todo: Apply code utils */
/* Todo: Think about class friendship */
CharactersIOErrorCode CharactersIoSerialConnectionImpl::HandleReceivedData(void *aPlatformHandle,
																																			     uint8_t *aBuffer,
																																				 	 uint16_t aByteCount) const
{
	CharactersIoInstance *instancePtr = nullptr;
	CharactersIOErrorCode errorCode = ERROR_NONE;

	instancePtr = PlatformHandleToInstance(aPlatformHandle);
	if (instancePtr != nullptr)
	{
		CharactersIoInstance &instance = *(instancePtr);

		for (size_t i = 0; i < aByteCount; i++)
		{
			(void) instance;
		}
		errorCode = ERROR_NONE;
	}
	else
	{
		errorCode = ERROR_FAILED;
	}

	return errorCode;
}

//CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PullData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount)
//{
//
//}
//
//CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PullData(void *aPlatformHandle, uint8_t aByte)
//{
//	return ERROR_NONE;
//}

} // CharactersIo
} // DeviceLayer
