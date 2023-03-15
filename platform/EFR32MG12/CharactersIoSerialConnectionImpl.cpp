/* MIT License

	Copyright (c) [2023] [Vo Tran Trung Hieu]

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/


#include "PlatformSerialConnectionConfig.h"

namespace CharactersIo {

namespace DeviceLayer {

CharactersIoSerialConnectionImpl::CharactersIoSerialConnectionImpl(void)
{
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

}

CharactersIoInstance
&CharactersIoSerialConnectionImpl::_CreateNewConnection(void *aPlatformHandle)
{
	struct CharactersIoConfigs config =
	{

	};

	return _CreateNewConnection(aPlatformHandle, config);
}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PushData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount)
{

}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::_PushData(void *aPlatformHandle, uint8_t aByte)
{

}

CharactersIoInstance *CharactersIoSerialConnectionImpl::PlatformHandleToInstance(void *aPlatformHandle) const
{

}

CharactersIOErrorCode CharactersIoSerialConnectionImpl::HandleReceivedData(void *aPlatformHandle,
																																			     uint8_t *aBuffer,
																																				 	 uint16_t aByteCount) const
{

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
