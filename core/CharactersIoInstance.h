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

#include "utilities/CharactersIoErrorCode.h"
#include "CharactersIoSerialConnection.h"
#include "stdint.h"

namespace CharactersIo {

namespace DeviceLayer { /* Todo: It must not be in this namespace */

class CharactersIoSerialConnectionImpl;

class CharactersIoInstance
{
	friend CharactersIoSerialConnectionImpl;
public:
//    void Initialize(void);

	/* Todo: Implement features-enable function */
	int32_t Write(const void *aBuffer, size_t aCount);
	int32_t Read(void *aBuffer, size_t aCount);

private:

	enum {
		kCharsIoBufferSize = 256u
	};

	~CharactersIoInstance(void);
	CharactersIoInstance(void *aPlatformHandle);

	bool PushDataToBuffer(uint8_t aByte);
	bool PushDataToBuffer(const uint8_t *aBuffer, uint16_t aCount);
	bool PopDataFromBuffer(uint8_t *aOutputByte);

	void *mPlatformHandle;
	/* Todo: Constant values should be in config file */
	uint8_t   mBuffer[kCharsIoBufferSize];
	uint16_t  mBufferReadIndex;
	uint16_t  mBufferWriteIndex;
	uint16_t  mBufferCount;
	bool      mIsSoftwareFlowControlEnabled;
	bool      mIsRemoteXON;
	bool 	    mCrShouldBeCrlf;
	bool      mIsXON;

	void *GetPlatformHandle(void) const;

	/* Todo: Inherit from non-copyable instance */
	// No copy, move or assignment.
	CharactersIoInstance(const CharactersIoInstance &)  = delete;
	CharactersIoInstance(const CharactersIoInstance &&) = delete;
	CharactersIoInstance &operator=(const CharactersIoInstance &) = delete;
};

} // DeviceLayer
} // CharactersIo
