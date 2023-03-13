/* Todo: License */
#pragma once

namespace CharactersIo {

namespace DeviceLayer {

class CharactersIoSerialConnectionImpl;

namespace Internal {

template <class ImplClass>
class GenericCharactersIoSerialConnectionImpl
{
private:
	inline ImplClass * Impl() { return static_cast<ImplClass *>(this); }
};

// Instruct the compiler to instantiate the template only when explicitly told to do so.
extern template class GenericCharactersIoSerialConnectionImpl<CharactersIoSerialConnectionImpl>;

} // namespace Internal
} // namespace DeviceLayer
} // namespace CharactersIo
