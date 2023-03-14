//#include <assert.h>
#include <CharactersIoInstance.h>
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
	: mPlatformhandle(aPlatformHandle)
//	, mUartHandle(HandleReceivedByte, this)
//	, mBufferReadIndex(0)
//	, mBufferWriteIndex(0)
//	, mBufferCount(0)

{

}

CharactersIoInstance::~CharactersIoInstance(void)
{
}

void *CharactersIoInstance::GetPlatformHandle(void) const
{
	return mPlatformhandle;
}
//
///* Todo: Implement macros for debuging */
//int32_t CharactersIo::Write(const void *aBuffer, size_t aCount)
//{
//		size_t            sentCount  = 0;
//    const uint8_t*    tempBuffer = nullptr;
//    CharactersIOErrorCode_t error      = CharactersIOErrorCode_t::ERROR_NONE;
//
//    /* Sanity check */
//    if (aBuffer == nullptr || aCount == 0)
//    {
//        return -1;
//    }
//
//    tempBuffer = reinterpret_cast<const uint8_t*>(aBuffer);
//    if (mCrShouldBeCrlf == false)
//    {
//			while (sentCount < aCount && error == ERROR_NONE)
//			{
//				if (mIsXON)
//				{
//	        error = mUartHandle.PushDataToUartTx(tempBuffer[sentCount++]);
//				}
//			}
//    }
//    else
//    {
//    	while (sentCount < aCount && error == ERROR_NONE)
//			{
//				if (mIsXON)
//				{
//					if (tempBuffer[sentCount] == '\n')
//					{
//						error = mUartHandle.PushDataToUartTx('\r');
//					}
//					error = mUartHandle.PushDataToUartTx(tempBuffer[sentCount++]);
//				}
//			}
//    }
//
//    return sentCount;
//}
//
///* Todo: Write documentations and UML to explain the algorithms */
//int32_t CharactersIo::Read(void *aBuffer, size_t aCount)
//{
//    size_t              readCount  = 0;
//    uint8_t*            tempBuffer = nullptr;
//    CharactersIOErrorCode_t error      = CharactersIOErrorCode_t::ERROR_NONE;
//
//    (void) error;
//    if (aBuffer == nullptr || aCount == 0)
//    {
//        return -1;
//    }
//
//    tempBuffer = reinterpret_cast<uint8_t*>(aBuffer);
//    while (readCount < aCount && mBufferCount != 0)
//    {
//        tempBuffer[readCount++] = mBuffer[mBufferReadIndex];
//        mBufferCount--;
//        mBufferReadIndex++;
//        if (mBufferReadIndex == kStreamBufferSize)
//        {
//            mBufferReadIndex = 0;
//        }
//    }
//
//    if (mIsRemoteXON == false)
//    {
//        uint8_t xon = XON;
//				error = mUartHandle.PushDataToUartTx(xon);
//        mIsRemoteXON = true;
//		}
//
//    return readCount;
//}
//
///* Todo: Deep copy */
///* Todo: Implement functions to change hardware port */
//void CharactersIo::Initialize()
//{
//}
//

void CharactersIoInstance::HandleReceivedByte(uint8_t *aReceivedBuffer, uint16_t aByteCount)
{

}

void CharactersIoInstance::HandleReceivedByte(uint8_t aReceivedByte)
{
//	CharactersIOErrorCode_t error    = CharactersIOErrorCode_t::ERROR_NONE;
//
//	(void) error;
//	if (mBufferCount > kStreamBufferSize)
//	{
//			assert(false);
//	}
//
//	/* Return immediately and abort all received data
//	 * if we don't have enough space for new data */
//	if (mBufferCount == kStreamBufferSize)
//	{
//			return;
//	}
//
//	DECLARE_IRQ_STATE;
//	ENTER_NO_INTERRUPTS_SECTION();
//	if (mIsSoftwareFlowControlEnabled)
//	{
//		if (aReceivedByte == XOFF)
//		{
//			mIsXON = false;
//			EXIT_NO_INTERRUPTS_SECTION();
//			return;
//		}
//		else if (aReceivedByte == XON)
//		{
//			mIsXON = true;
//			EXIT_NO_INTERRUPTS_SECTION();
//			return;
//		}
//	}
//
//	mBuffer[mBufferWriteIndex] = aReceivedByte;
//	mBufferWriteIndex++;
//	mBufferCount++;
//	if (mBufferWriteIndex == kStreamBufferSize)
//	{
//			mBufferWriteIndex = 0;
//	}
//	//    signalUartReceived();
//	EXIT_NO_INTERRUPTS_SECTION();
//
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
}
//
///* Method-forwarding to lower layer */
//inline void CharactersIo::HandleReceivedByte(void *aContext, uint8_t aReceivedByte)
//{
//    static_cast<CharactersIo *>(aContext)->HandleReceivedByte(aReceivedByte);
//}
//


} // namespace CharactersIo

} // namespace DeviceLayer

