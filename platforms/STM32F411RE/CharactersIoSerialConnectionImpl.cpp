#include <CharactersIoSerialConnectionImpl.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Todo: should define in the config file*/

static void PushDataToCharactersIOBuffer(CharactersIoSerialConnectionImpl &aImplInstance, uint8_t aByte);

static void PushDataToCharactersIOBuffer(CharactersIoSerialConnectionImpl &aImplInstance, uint8_t aByte)
{
	if (charactersIOUart->mByteReceivedHandler != nullptr && aUartContext != nullptr)
	{
		charactersIOUart->mByteReceivedHandler(charactersIOUart->mCharactersIOContext, aByte);
	}
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_StartDma(void)
{
	CharactersIOErrorCode_t error = ERROR_NONE;
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *) aPlatformUartHandle;

	HAL_UARTEx_ReceiveToIdle_DMA(huart, mCreatedInstances[mInstanceCount].mDmaBuffer, DMA_BUFFER_SIZE);

	return error;
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_Init(CharactersIo const &aCharactersIo, void *aPlatform)
{
	if (_ConnectWithPlatform(aPlatform) == ERROR_NONE && _ConnectWithCharactersIo(aCharactersIo))
	{
		return ERROR_NONE;
	}
	else
	{
		return ERROR_FAILED;
	}
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_Deinit(void)
{
	for (uint8_t i = 0; i < mPlatformCount; ++i)
	{
		if (instanceInfo[i].mPlatform == mPlatformSerialConnectionHandler)
		{
			mPlatformCount--;
			while (i < mPlatformCount)
			{
				instanceInfo[i].mPlatform = instanceInfo[i + 1].mPlatform;
			}
			break;
		}
	}

	return ERROR_NONE;
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_ConnectWithCharactersIo(CharactersIo const &aCharactersIo)
{
	CharactersIOErrorCode_t isSuccessful = ERROR_NONE;

	mCharactersIo = &aCharactersIo;

	return isSuccessful;
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_ConnectWithPlatform(void *aPlatform)
{
	CharactersIOErrorCode_t isSuccessful = ERROR_NONE;

	for (uint8_t i = 0; i < mPlatformCount; ++i)
	{
		if (aPlatform == instanceInfo[i].mPlatform)
		{
			isSuccessful = ERROR_FAILED;
			break;
		}
	}

	if (isSuccessful == ERROR_NONE)
	{
		instanceInfo[mPlatformCount].mPlatform = aPlatform;
		instanceInfo[mPlatformCount].mInstance = this;
		mPlatformIndex = mPlatformCount;
		mPlatformCount++;
	}

	return isSuccessful;
}

static CharactersIoSerialConnection &CharactersIoSerialConnectionImpl::_GetInstance(void *aPlatform)
{
	CharactersIoSerialConnection &foundInstance;

	for (uint8_t i = 0; i < mPlatformCount; ++i)
	{
		if (aPlatform == instanceInfo[i].mPlatform)
		{
			foundInstance = instanceInfo[i].mInstance;
			break;
		}
	}

	return foundInstance;
}


extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	UartContext_t* iostreamUartContext = static_cast<UartContext_t*>(CharactersIOUart::GetInstance(huart));

	switch (huart->ErrorCode)
	{
		case HAL_UART_ERROR_NONE:
			break;
		case HAL_UART_ERROR_ORE:
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
			iostreamUartContext->mDmaBufferOldPos = 0;

			HAL_UARTEx_ReceiveToIdle_DMA(huart, iostreamUartContext->mDmaBuffer, DMA_BUFFER_SIZE);
			break;
	}
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	CharactersIoSerialConnectionImpl &implInstance = CharactersIoSerialConnection::GetInstance((void*) huart).Impl();
	uint32_t index             = 0;
	uint32_t numReceivedChars  = 0;
	uint16_t dmaBufferOldPos   = implInstance.mDmaBufferOldPos;
	uint8_t  *dmaBuffer        = implInstance.mDmaBuffer;


	/* Todo: each dma_buffer should be correspond with 1 platformUartHandle */
	if (size != dmaBufferOldPos)
	{
		if (size > dmaBufferOldPos)
		{
			numReceivedChars = size - dmaBufferOldPos;
			for (index = 0; index < numReceivedChars; index++)
			{
					PushDataToCharactersIOBuffer(implInstance, dmaBuffer[dmaBufferOldPos + index]);
			}
		}
		else
		{
			numReceivedChars = DMA_BUFFER_SIZE - dmaBufferOldPos;
			for (index = 0; index < numReceivedChars; index++)
			{
					PushDataToCharactersIOBuffer(implInstance, dmaBuffer[dmaBufferOldPos + index]);
			}
			if (size > 0)
			{
				for (index = 0; index < size; index++)
				{
						PushDataToCharactersIOBuffer(implInstance, dmaBuffer[index]);
				}
			}
		}
		implInstance.mDmaBufferOldPos = size;
  }
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_PushData(uint8_t const *aByte, uint16_t aByteCount)
{
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *) instanceInfo[mPlatformIndex].mPlatform;

	return HAL_UART_Transmit(huart, aByte, aByteCount, HAL_MAX_DELAY) == HAL_OK ? ERROR_NONE : ERROR_TX_BUSY;
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_PushData(uint8_t aByte)
{
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *) instanceInfo[mPlatformIndex].mPlatform;
	CharactersIOErrorCode_t error = ERROR_NONE;

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

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_HandleReceivedData(uint8_t const *aByte, uint16_t aByteCount)
{
	return ERROR_NONE;
}

CharactersIOErrorCode_t CharactersIoSerialConnectionImpl::_HandleReceivedData(uint8_t aByte)
{
	return ERROR_NONE;
}
