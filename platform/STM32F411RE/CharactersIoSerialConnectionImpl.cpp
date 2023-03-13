/* Todo: License */

#include "stm32f4xx.h"

/* Todo: define USE_HAL_DRIVER */

#include "CharactersIoSerialConnection.h"
#include "CharactersIoSerialConnectionImpl.h"
#include "CharactersIoErrorCode.h"

/* Todo: Define in config file */
#define MAX_CHARSIO_NUM 1u

namespace CharactersIo {

namespace DeviceLayer {


extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
//	UartContext_t* iostreamUartContext = static_cast<UartContext_t*>(CharactersIOUart::GetInstance(huart));
//
//	switch (huart->ErrorCode)
//	{
//		case HAL_UART_ERROR_NONE:
//			break;
//		case HAL_UART_ERROR_ORE:
//			/* Todo: this should also be platform-dependent */
//			__attribute__((fallthrough));
//		case HAL_UART_ERROR_FE:
//			__attribute__((fallthrough));
//		case HAL_UART_ERROR_NE:
//			__attribute__((fallthrough));
//		case HAL_UART_ERROR_PE:
//			__attribute__((fallthrough));
//		case HAL_UART_ERROR_DMA:
//			__attribute__((fallthrough));
//		/* Undefined errors are treated as HAL_UART_ERROR_DMA */
//		default:
////      printf("Error: HAL_UART_ERROR: %d\n", huart->ErrorCode);
//			iostreamUartContext->mDmaBufferOldPos = 0;
//
//			HAL_UARTEx_ReceiveToIdle_DMA(huart, iostreamUartContext->mDmaBuffer, DMA_BUFFER_SIZE);
//			break;
//	}
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
//	CharactersIoSerialConnectionImpl &implInstance = CharactersIoSerialConnection::GetInstance((void*) huart).Impl();
//	uint32_t index             = 0;
//	uint32_t numReceivedChars  = 0;
//	uint16_t dmaBufferOldPos   = implInstance.mDmaBufferOldPos;
//	uint8_t  *dmaBuffer        = implInstance.mDmaBuffer;
//
//
//	/* Todo: each dma_buffer should be correspond with 1 platformUartHandle */
//	if (size != dmaBufferOldPos)
//	{
//		if (size > dmaBufferOldPos)
//		{
//			numReceivedChars = size - dmaBufferOldPos;
//			for (index = 0; index < numReceivedChars; index++)
//			{
//					PushDataToCharactersIOBuffer(implInstance, dmaBuffer[dmaBufferOldPos + index]);
//			}
//		}
//		else
//		{
//			numReceivedChars = DMA_BUFFER_SIZE - dmaBufferOldPos;
//			for (index = 0; index < numReceivedChars; index++)
//			{
//					PushDataToCharactersIOBuffer(implInstance, dmaBuffer[dmaBufferOldPos + index]);
//			}
//			if (size > 0)
//			{
//				for (index = 0; index < size; index++)
//				{
//						PushDataToCharactersIOBuffer(implInstance, dmaBuffer[index]);
//				}
//			}
//		}
//		implInstance.mDmaBufferOldPos = size;
//  }
}

/* Anonymous namespace prevents accessibility of outside files */
namespace {
alignas(alignof(CharactersIoInstance)) char sCharactersIoRaw[sizeof(CharactersIoInstance) * MAX_CHARSIO_NUM];
} // namespace

CharactersIoSerialConnectionImpl::CharactersIoSerialConnectionImpl(void)
{
	mInstanceCount = 0;

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
		*(new (&sCharactersIoRaw[mInstanceCount])
			CharactersIoInstance(aPlatformHandle));

	mInstanceCount++;

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

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PushData(void *aPlatformHandle, uint8_t const *aByte, uint16_t aByteCount)
{
	UART_HandleTypeDef *huart = static_cast<UART_HandleTypeDef *> (aPlatformHandle);

	return HAL_UART_Transmit(huart, aByte, aByteCount, HAL_MAX_DELAY) == HAL_OK ? ERROR_NONE : ERROR_TX_BUSY;
}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PushData(void *aPlatformHandle, uint8_t aByte)
{
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


CharactersIOErrorCode CharactersIoSerialConnectionImpl::_HandleReceivedData(void *aPlatformHandle, uint8_t const *aByte, uint16_t aByteCount)
{
	return ERROR_NONE;
}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_HandleReceivedData(void *aPlatformHandle, uint8_t aByte)
{
	return ERROR_NONE;
}

} // CharactersIo
} // DeviceLayer
