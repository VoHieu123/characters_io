#pragma once
#include <stdint.h>
#include "CharactersIo.h"
#include "../utilities/charactersio_error_code.hpp"

class CharactersIoSerialConnection
{
	using ImplClass = CharactersIoSerialConnectionImpl;

public:
	CharactersIOErrorCode_t PushData(uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode_t PushData(uint8_t aByte);
	CharactersIOErrorCode_t HandleReceivedData(uint8_t const *aByte, uint16_t aByteCount);
	CharactersIOErrorCode_t HandleReceivedData(uint8_t aByte);

	bool IsConnectedWithCharactersIo(void);
	bool IsConnectedWithPlatform(void);
	CharactersIOErrorCode_t ConnectWithCharactersIo(CharactersIo const &aCharactersIo);
	CharactersIOErrorCode_t ConnectWithPlatform(void *aPlatform);

	CharactersIOErrorCode_t StartDma(void);
	CharactersIOErrorCode_t StopDma(void);

	static CharactersIoSerialConnection &GetInstance(void *aPlatform);

protected:
    // Construction/destruction limited to subclasses.
		CharactersIoSerialConnection()  = default;
    ~CharactersIoSerialConnection() = default;

    // No copy, move or assignment.
    CharactersIoSerialConnection(const CharactersIoSerialConnection &)  = delete;
    CharactersIoSerialConnection(const CharactersIoSerialConnection &&) = delete;
    CharactersIoSerialConnection &operator=(const CharactersIoSerialConnection &) = delete;
};

inline CharactersIoSerialConnection &CharactersIoSerialConnection::GetInstance(void *aPlatform)
{
	return ImplClass::_GetInstance(aPlatform);
}

inline bool CharactersIoSerialConnection::IsConnectedWithCharactersIo(void)
{
	return static_cast<ImplClass *>(this)->_IsConnectedWithCharactersIo();
}

inline bool CharactersIoSerialConnection::IsConnectedWithPlatform(void)
{
	return static_cast<ImplClass *>(this)->_IsConnectedWithPlatform();
}

inline CharactersIOErrorCode_t CharactersIoSerialConnection::PushData(uint8_t const *aByte, uint16_t aByteCount)
{
	return static_cast<ImplClass *>(this)->_PushData(aByte, aByteCount);
}


inline CharactersIOErrorCode_t CharactersIoSerialConnection::HandleReceivedData(uint8_t const *aByte, uint16_t aByteCount)
{
	return static_cast<ImplClass *>(this)->_HandleReceivedData(aByte, aByteCount);
}

inline CharactersIOErrorCode_t CharactersIoSerialConnection::PushData(uint8_t aByte)
{
	return static_cast<ImplClass *>(this)->_PushData(aByte);
}


inline CharactersIOErrorCode_t CharactersIoSerialConnection::HandleReceivedData(uint8_t aByte)
{
	return static_cast<ImplClass *>(this)->_HandleReceivedData(aByte);
}
