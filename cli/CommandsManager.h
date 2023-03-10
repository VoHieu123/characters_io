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

namespace CharactersIo {

class CommandsManager {

public:

	static CommandsManager & GetInstance(void);
	/* Todo: Try state machine */
	/* Todo: Functions to
	 *           + Add command and handler
	 *           + Configure features
	 */

private:

	CommandsManager() = default;
	~CommandsManager() = default;

	static CommandsManager sInstance;

	/* Todo: check if below lines are effected by private, public, protected keyword */
	// No copy, move or assignment.
	CommandsManager(const CommandsManager &) = delete;
	CommandsManager(const CommandsManager &&) = delete;
	CommandsManager &operator=(const CommandsManager &) = delete;
};

} // CharactersIo
