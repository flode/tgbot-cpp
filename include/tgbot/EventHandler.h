/*
 * Copyright (c) 2015 Oleg Morozenkov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TGBOT_EVENTHANDLER_H
#define TGBOT_EVENTHANDLER_H

#include "tgbot/EventBroadcaster.h"
#include "tgbot/types/Update.h"
#include "tgbot/tools/StringTools.h"

namespace TgBot {

class EventHandler {

public:
	explicit EventHandler(const EventBroadcaster* broadcaster) : _broadcaster(broadcaster) {
	}

	inline void handleUpdate(const Update::Ptr& update) const {
		_broadcaster->broadcastAnyMessage(update->message);
		if (StringTools::startsWith(update->message->text, "/")) {
			unsigned long splitPosition;
			unsigned long spacePosition = update->message->text.find(' ');
			unsigned long atSymbolPosition = update->message->text.find('@');
			if (spacePosition == update->message->text.npos) {
				if (atSymbolPosition == update->message->text.npos) {
					splitPosition = update->message->text.size();
				} else {
					splitPosition = atSymbolPosition;
				}
			} else if (atSymbolPosition == update->message->text.npos) {
				splitPosition = spacePosition;
			} else {
				splitPosition = std::min(spacePosition, atSymbolPosition);
			}
			std::string command = update->message->text.substr(1, splitPosition - 1);
			if (!_broadcaster->broadcastCommand(command, update->message)) {
				_broadcaster->broadcastUnknownCommand(update->message);
			}
		} else {
			_broadcaster->broadcastNonCommandMessage(update->message);
		}
	}

private:
	const EventBroadcaster* _broadcaster;
};

}

#endif //TGBOT_EVENTHANDLER_H
