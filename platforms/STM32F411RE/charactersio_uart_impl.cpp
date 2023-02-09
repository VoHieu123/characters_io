#include "charactersio_uart_impl.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Todo: should define in the config file*/

static HAL_StatusTypeDef stm32UartTx(UART_HandleTypeDef *huart, uint8_t aByte);
static void pushDataToCharactersIOBuffer(UartContext_t* aUartContext, uint8_t aByte);

static void pushDataToCharactersIOBuffer(UartContext_t* aUartContext, uint8_t aByte)
{
	CharactersIOUart *charactersIOUart = static_cast<CharactersIOUart*>(aUartContext->mCharactersIOUart);

	if (charactersIOUart->mByteReceivedHandler != nullptr && aUartContext != nullptr)
	{
		charactersIOUart->mByteReceivedHandler(charactersIOUart->mCharactersIOContext, aByte);
	}
}

static HAL_StatusTypeDef stm32UartTx(UART_HandleTypeDef *huart, uint8_t aByte)
{
  /* Check that a Tx process is not already ongoing */
  if (huart->gState == HAL_UART_STATE_READY)
  {
    __HAL_LOCK(huart);

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->TxXferSize = 1;
    huart->TxXferCount = 1;
    huart->gState = HAL_UART_STATE_BUSY_TX;

     __HAL_UNLOCK(huart);

    while (!(huart->Instance->SR & UART_FLAG_TXE)) {}
    huart->Instance->DR = (uint32_t) aByte;
    huart->TxXferCount--;

    huart->gState = HAL_UART_STATE_READY;

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

CharactersIOErrorCode_t CharactersIOUartImpl::_PushDataToUartTx(uint8_t aByte)
{
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *) mUartHandle;
	CharactersIOErrorCode_t error = ERROR_NONE;

	error = stm32UartTx(huart, aByte) == HAL_OK ? ERROR_NONE : ERROR_UART_BUSY;

	return error;
}

CharactersIOErrorCode_t CharactersIOUartImpl::_Init(void* aPlatformUartHandle, IOStreamUart *aIOStreamUart)
{
	CharactersIOErrorCode_t error = ERROR_NONE;
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *) aPlatformUartHandle;

	mCreatedInstances[mInstanceCount].mCharactersIOUart = aCharactersIOUart;
	mCreatedInstances[mInstanceCount].mUartHandle = aPlatformUartHandle;
	mUartHandle = aPlatformUartHandle;
	mCreatedInstances[mInstanceCount].mDmaBufferOldPos = 0;
	mInstanceCount++;
	HAL_UARTEx_ReceiveToIdle_DMA(huart, mCreatedInstances[mInstanceCount].mDmaBuffer, DMA_BUFFER_SIZE);

	return error;
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	UartContext_t* iostreamUartContext = static_cast<UartContext_t*>(IOStreamUart::GetInstance(huart));

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
	UartContext_t* uartContext = CharactersIO::GetInstance(huart);
	uint32_t i                         = 0;
	uint32_t num_received_chars        = 0;
	uint16_t dma_buffer_old_pos        = uartContext->mDmaBufferOldPos;
	uint8_t  *dma_buffer               = uartContext->mDmaBuffer;


	/* Todo: each dma_buffer should be correspond with 1 platformUartHandle */
	if (size != dma_buffer_old_pos)
	{
		if (size > dma_buffer_old_pos)
		{
			num_received_chars = size - dma_buffer_old_pos;
			for (i = 0; i < num_received_chars; i++)
			{
					pushDataToCharactersIOBuffer(uartContext, dma_buffer[dma_buffer_old_pos + i]);
			}
		}
		else
		{
			num_received_chars = DMA_BUFFER_SIZE - dma_buffer_old_pos;
			for (i = 0; i < num_received_chars; i++)
			{
					pushDataToCharactersIOBuffer(uartContext, dma_buffer[dma_buffer_old_pos + i]);
			}
			if (size > 0)
			{
				for (i = 0; i < size; i++)
				{
						pushDataToCharactersIOBuffer(uartContext, dma_buffer[i]);
				}
			}
		}
		uartContext->mDmaBufferOldPos = size;
  }
}
