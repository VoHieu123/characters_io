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

#pragma once

#include "core/CharactersIoSerialConnection.h"
/* Todo: Think about the folder structure and names */
#include "core/internal/GenericCharactersIoSerialConnectionImpl.h"
/* Todo: Must be included and must be inline-ed to overload "new" operator */
#include <cstddef>

inline void* operator new(size_t sz, void *p)
{
	return p;
}

namespace CharactersIo {
namespace DeviceLayer {

class CharactersIoSerialConnection;
class CharactersIoSerialConnectionImpl;

class CharactersIoSerialConnectionImpl final: public CharactersIoSerialConnection,
public Internal::GenericCharactersIoSerialConnectionImpl<CharactersIoSerialConnectionImpl>
{
public:
	friend CharactersIoSerialConnection;
	friend CharactersIoInstance;
	friend Internal::GenericCharactersIoSerialConnectionImpl<CharactersIoSerialConnectionImpl>;

	friend CharactersIoSerialConnection & ::CharactersIo::DeviceLayer::GetCharactersIoSerialConnection(void);
	friend CharactersIoSerialConnectionImpl & ::CharactersIo::DeviceLayer::GetCharactersIoSerialConnectionImpl(void);

	CharactersIoInstance *PlatformHandleToInstance(void *aPlatformHandle) const;
	CharactersIOErrorCode HandleReceivedData(void *aPlatformHandle, uint8_t *aBuffer, uint16_t aByteCount) const;

private:
	CharactersIoSerialConnectionImpl(void);
	~CharactersIoSerialConnectionImpl(void);
	void _Init(void);

	CharactersIoInstance &_CreateNewConnection(void *aPlatformHandle, struct CharactersIoConfigs &aIoConfig);
	CharactersIoInstance &_CreateNewConnection(void *aPlatformHandle); // Default config

	void _RemoveConnection(void *aPlatformHandle);

	/* Todo: Add const */
	CharactersIOErrorCode _PushData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount);
	CharactersIOErrorCode _PushData(void *aPlatformHandle, uint8_t aByte);
//	CharactersIOErrorCode _PullData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount);
//	CharactersIOErrorCode _PullData(void *aPlatformHandle, uint8_t aByte);

	/* Todo: Typedef uint8 to specific use of our service */
	static uint8_t sInstanceCount;
	static CharactersIoSerialConnectionImpl sInstance;

};

/**
 * Returns the public interface of the CharactersIoSerialConnection singleton object.
 *
 * Applications should use this to access features of the CharactersIoSerialConnection object
 * that are common to all platforms.
 */
inline CharactersIoSerialConnection & GetCharactersIoSerialConnection(void)
{
	return CharactersIoSerialConnectionImpl::sInstance; /* Todo: ? */
}

/**
 * Returns the platform-specific implementation of the CharactersIoSerialConnectionImpl singleton object.
 *
 * Applications can use this to gain access to features of the CharactersIoSerialConnection
 * that are specific to the selected platform.
 */
inline CharactersIoSerialConnectionImpl & GetCharactersIoSerialConnectionImpl(void)
{
	return CharactersIoSerialConnectionImpl::sInstance;
}

} // namespace DeviceLayer
} // namespace CharactersIo
