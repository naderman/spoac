/*
 * Copyright 2008 Nils Adermann
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JSON_STRING_H
#define JSON_STRING_H

#include "Value.h"
#include <string>

namespace JSON
{
	/**
	* Represents a JSON String.
	*
	* This class wraps a UnicodeString with only minimal functionality for
	* appending, assignment and comparison. All further operations should
	* be performed on the string itself which can be retrieved using
	* toString(). The string is automatically escaped correctly when
	* converted to JSON.
	*/
	class String : public Value
	{
	public:
		String();
		String(const char& c);
		String(const std::string& s);
		String(const char* data);
		String(const char* data, size_t len);
		String(const String& s);

		void append(const char& c);
		void append(const std::string& s);
		void append(const char* data);
		void append(const char* data, size_t len);
		void append(const String& s);
		std::string& toString();
		const std::string& toString() const;
		void _toJSON(std::string& json, const std::string& indent) const;

		String& operator=(const String& s);
		bool operator==(const Value& v) const;
		bool operator!=(const Value& v) const;
		bool operator>(const Value& v) const;
		bool operator<(const Value& v) const;
		bool operator>=(const Value& v) const;
		bool operator<=(const Value& v) const;
	private:
		std::string string;
	};
}

#endif
