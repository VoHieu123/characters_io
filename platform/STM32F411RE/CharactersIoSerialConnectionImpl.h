/* Todo: Include MIT license */

#pragma once

#include "CharactersIoSerialConnection.h"
/* Todo: Think about the folder structure and names */
#include <GenericCharactersIoSerialConnectionImpl.h>
/* Todo: Must be included and must be inline-ed to overload "new" operator */
#include <cstddef>

inline void* operator new(size_t sz, void *p)
{
	return p;
}

namespace CharactersIo {
namespace DeviceLayer {

/* Todo: Define this in platform implementation */
class PlatformHandle {

};

class CharactersIoSerialConnection;
class CharactersIoSerialConnectionImpl;

class CharactersIoSerialConnectionImpl final: public CharactersIoSerialConnection,
public Internal::GenericCharactersIoSerialConnectionImpl<CharactersIoSerialConnectionImpl>
{
public:
	friend CharactersIoSerialConnection;
	friend CharactersIoInstance;
	friend Internal::GenericCharactersIoSerialConnectionImpl<CharactersIoSerialConnectionImpl>;

	/* Todo: Is there any differences between privately and publicly grand friend ? */
	friend CharactersIoSerialConnection & ::CharactersIo::DeviceLayer::GetCharactersIoSerialConnection(void);
	friend CharactersIoSerialConnectionImpl & ::CharactersIo::DeviceLayer::GetCharactersIoSerialConnectionImpl(void);

private:

	void _Init(void);

	CharactersIoInstance &_CreateNewConnection(void *aPlatformHandle, struct CharactersIoConfigs &aIoConfig);
	CharactersIoInstance &_CreateNewConnection(void *aPlatformHandle); // Default config

	void _RemoveConnection(void *aPlatformHandle);

	CharactersIOErrorCode _PushData(void *aPlatformHandle, uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode _PushData(void *aPlatformHandle, uint8_t aByte);
	CharactersIOErrorCode _HandleReceivedData(void *aPlatformHandle, uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode _HandleReceivedData(void *aPlatformHandle, uint8_t aByte);

	CharactersIoInstance *PlatformHandleToInstance(void *aPlatformHandle) const;

	/* Initialize these value */
	/* Todo: typedef uint8 */
	static uint8_t mInstanceCount;
	static CharactersIoSerialConnectionImpl sInstance;
	CharactersIoSerialConnectionImpl(void);
	~CharactersIoSerialConnectionImpl(void);

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
