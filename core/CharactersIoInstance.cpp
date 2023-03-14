//#include <assert.h>
#include <CharactersIoInstance.h>
#include <assert.h>
#include "CharactersIoSerialConnection.h"
////#include "interrupt_handling.h"
//#define DECLARE_IRQ_STATE
//#define	ENTER_NO_INTERRUPTS_SECTION()
//#define	EXIT_NO_INTERRUPTS_SECTION()
///* Todo: RTOS Kernel persents? */
//
//#define XON  0x11u
//#define XOFF 0x13u
//

namespace CharactersIo {

namespace DeviceLayer {

CharactersIoInstance::CharactersIoInstance(void *aPlatformHandle)
	: mPlatformHandle(aPlatformHandle)
	, mBufferReadIndex(0)
	, mBufferWriteIndex(0)
	, mBufferCount(0)
	, mIsSoftwareFlowControlEnabled(false)
	,	mIsRemoteXON(false)
	, mCrShouldBeCrlf(false)
	, mIsXON(false)
{

}

CharactersIoInstance::~CharactersIoInstance(void)
{
}

void *CharactersIoInstance::GetPlatformHandle(void) const
{
	return mPlatformHandle;
}

/* Todo: Implement macros for debuging */
int32_t CharactersIoInstance::Write(const void *aBuffer, size_t aCount)
{
	CharactersIoSerialConnection &serial = GetCharactersIoSerialConnection();
	size_t         sentCount  = 0;
	const uint8_t *aBufferUnsigned = static_cast <const uint8_t*> (aBuffer);

	/* Sanity check */
	if (aBuffer == nullptr || aCount == 0)
	{
			return -1;
	}

	if (mCrShouldBeCrlf == false)
	{
		while (sentCount < aCount)
		{
			if (mIsXON)
			{
				/* Todo: Implement error ? */
				serial.PushData(this, aBufferUnsigned[sentCount++]);
			}
		}
	}
	else
	{
		while (sentCount < aCount)
		{
			if (mIsXON)
			{
				if (aBufferUnsigned[sentCount] == '\n')
				{
					serial.PushData(this, '\r');
				}
				serial.PushData(this, aBufferUnsigned[sentCount++]);
			}
		}
	}

	return sentCount;
}

bool CharactersIoInstance::PopDataFromBuffer(uint8_t *aOutputByte)
{
	if (mBufferCount < 0)
	{
		assert(false);
	}

	if (aOutputByte == nullptr || mBufferCount == 0)
	{
		return false;
	}

	/* Todo: Thread safe ultilities? */
	*aOutputByte = mBuffer[mBufferReadIndex];
	mBufferCount--;
	mBufferReadIndex++;
	if (mBufferReadIndex == kCharsIoBufferSize)
	{
			mBufferReadIndex = 0;
	}

	/* Todo: later */
//	if (mIsRemoteXON == false)
//	{
//			uint8_t xon = XON;
//			error = mUartHandle.PushDataToUartTx(xon);
//			mIsRemoteXON = true;
//	}

	return true;
}


/* Todo: Write documentations and UML to explain the algorithms */
int32_t CharactersIoInstance::Read(void *aBuffer, size_t aCount)
{
	uint16_t byteRead = 0;
	uint8_t *aBufferUnsigned = nullptr;

	aBufferUnsigned = static_cast <uint8_t *> (aBuffer);
	while (byteRead < aCount)
	{
		if(PopDataFromBuffer(&aBufferUnsigned[byteRead]))
		{
			byteRead++;
		}
		else
		{
			break;
		}
	}

	return byteRead;
}

/* Todo: Remove this or not? */
bool CharactersIoInstance::PushDataToBuffer(const uint8_t *aBuffer, uint16_t aCount)
{
	for (size_t i = 0; i < aCount; i++)
	{
		if (!PushDataToBuffer(aBuffer[i]))
		{
			return false;
		}
	}

	return true;
}

bool CharactersIoInstance::PushDataToBuffer(uint8_t aByte)
{
	if (mBufferCount > kCharsIoBufferSize)
	{
			assert(false);
	}

	/* Return immediately and abort all received data
	 * if we don't have enough space for new data */
	if (mBufferCount == kCharsIoBufferSize)
	{
			return false;
	}

	/* Todo: Uncomment these */
//	DECLARE_IRQ_STATE;
//	ENTER_NO_INTERRUPTS_SECTION();

//	if (mIsSoftwareFlowControlEnabled)
//	{
//		if (aByte == XOFF)
//		{
//			mIsXON = false;
////			EXIT_NO_INTERRUPTS_SECTION();
//			return;
//		}
//		else if (aByte == XON)
//		{
//			mIsXON = true;
////			EXIT_NO_INTERRUPTS_SECTION();
//			return;
//		}
//	}

	mBuffer[mBufferWriteIndex] = aByte;
	mBufferWriteIndex++;
	mBufferCount++;
	if (mBufferWriteIndex == kCharsIoBufferSize)
	{
			mBufferWriteIndex = 0;
	}
	//    signalUartReceived();
//	EXIT_NO_INTERRUPTS_SECTION();

//	if (mIsSoftwareFlowControlEnabled)
//	{
//		/* Stream's buffer is full of unread data, so we disable partner's transmitter */
//		if (mBufferCount == kStreamBufferSize)
//		{
//			uint8_t xoff = XOFF;
//			error = mUartHandle.PushDataToUartTx(xoff);
//			mIsRemoteXON = false;
//		}
//	}
	return true;
}

} // namespace CharactersIo

} // namespace DeviceLayer

