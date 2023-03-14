/* Todo: Insert MIT license */

#pragma once

/* Todo: Include needed libraries */
#include <CharactersIoErrorCode.h>
#include <CharactersIoInstance.h>
#include <stdint.h>

namespace CharactersIo {

namespace DeviceLayer {

/* Todo: Declare involves classes */
class CharactersIoSerialConnectionImpl;
class CharactersIoInstance;
class PlatformHandle;

/* Todo: Include needed internal templates */
namespace Internal {
template <class>
class GenericCharactersIoSerialConnectionImpl;
} // namespace Internal

class CharactersIoSerialConnection
{
	using ImplClass = CharactersIoSerialConnectionImpl;

protected:
	// Construction/destruction limited to subclasses.
	CharactersIoSerialConnection()  = default;
	~CharactersIoSerialConnection() = default;

	// No copy, move or assignment.
	CharactersIoSerialConnection(const CharactersIoSerialConnection &)  = delete;
	CharactersIoSerialConnection(const CharactersIoSerialConnection &&) = delete;
	CharactersIoSerialConnection &operator=(const CharactersIoSerialConnection &) = delete;

public:

	/* Todo: HDLC? Implement decoder and encoder for this. A config struct is needed in order
	 * to specify whether HDLC is enabled or not */
	CharactersIOErrorCode PushData(void *aPlatformHandle, uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode PushData(void *aPlatformHandle, uint8_t aByte);
//	CharactersIOErrorCode PullData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount);
//	CharactersIOErrorCode PullData(void *aPlatformHandle, uint8_t aByte);

	/* Todo: Dedicated struct for configuration */
	struct CharactersIoConfigs
	{

	};

	void Init(void);

	/* Todo: Implementation of this function should be in template file */
	CharactersIoInstance &CreateNewConnection(void *aPlatformHandle, struct CharactersIoConfigs &aIoConfig);
	CharactersIoInstance &CreateNewConnection(void *aPlatformHandle); // Default config

	void RemoveConnection(void *aPlatformHandle);

private:

	/* Todo: Grand friend to all generic classes */
//  template <class>
//  friend class Internal::CharactersIo:DeviceLayer::GenericCharactersIoSerialConnectionImpl;

};

/**
 * Returns the public interface of the CharactersIoSerialConnection singleton object.
 *
 * Applications should use this to access features of the CharactersIoSerialConnection object
 * that are common to all platforms.
 */
extern CharactersIoSerialConnection & GetCharactersIoSerialConnection(void);

/**
 * Returns the platform-specific implementation of the CharactersIoSerialConnectionImpl singleton object.
 *
 * Applications can use this to gain access to features of the CharactersIoSerialConnection
 * that are specific to the selected platform.
 */
extern CharactersIoSerialConnectionImpl & GetCharactersIoSerialConnectionImpl(void);

} // namespace DeviceLayer
} // namespace CharactersIo

/* Todo: Include a header file containing the implementation of the object for the selected platform using Makefile. */

#define CHARACTERSIO_DEVICE_LAYER_TARGET STM32F411RE /* Todo: Remove this and specify all required symbols like this */

#ifdef EXTERNAL_CHARACTERSIOSERIALCONNECTIONIMPL_HEADER
#include EXTERNAL_CHARACTERSIOSERIALCONNECTIONIMPL_HEADER
#elif defined(CHARACTERSIO_DEVICE_LAYER_TARGET)
#define CHARACTERSIOSERIALCONNECTION_HEADER <platform/CHARACTERSIO_DEVICE_LAYER_TARGET/CharactersIoSerialConnectionImpl.h>
#include CHARACTERSIOSERIALCONNECTION_HEADER
#endif // defined(CHARACTERSIO_DEVICE_LAYER_TARGET)

namespace CharactersIo {

namespace DeviceLayer {

inline void CharactersIoSerialConnection::Init(void)
{
	static_cast<ImplClass *>(this)->_Init();
}

inline void CharactersIoSerialConnection::RemoveConnection(void *aPlatformHandle)
{
	static_cast<ImplClass *>(this)->_RemoveConnection(aPlatformHandle);
}


inline CharactersIoInstance &CharactersIoSerialConnection::CreateNewConnection(void *aPlatformHandle, struct CharactersIoConfigs &aIoConfig)
{
	return static_cast<ImplClass *>(this)->_CreateNewConnection(aPlatformHandle, aIoConfig);
}

inline CharactersIoInstance &CharactersIoSerialConnection::CreateNewConnection(void *aPlatformHandle)
{
	return static_cast<ImplClass *>(this)->_CreateNewConnection(aPlatformHandle);
}

inline CharactersIOErrorCode CharactersIoSerialConnection::PushData(void *aPlatformHandle, uint8_t const *aByte, uint16_t aByteCount)
{
	return static_cast<ImplClass *>(this)->_PushData(aPlatformHandle, aByte, aByteCount);
}

inline CharactersIOErrorCode CharactersIoSerialConnection::PushData(void *aPlatformHandle, uint8_t aByte)
{
	return static_cast<ImplClass *>(this)->_PushData(aPlatformHandle, aByte);
}

//inline CharactersIOErrorCode CharactersIoSerialConnection::PullData(void *aPlatformHandle, uint8_t const *aBuffer, uint16_t aByteCount)
//{
//	return static_cast<ImplClass *>(this)->_PullData(aPlatformHandle, aBuffer, aByteCount);
//}
//
//inline CharactersIOErrorCode CharactersIoSerialConnection::PullData(void *aPlatformHandle, uint8_t aByte)
//{
//	return static_cast<ImplClass *>(this)->_PullData(aPlatformHandle, aByte);
//}

} // namespace DeviceLayer
} // namespace CharactersIo
