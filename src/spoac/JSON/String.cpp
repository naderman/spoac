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

#include <iostream>
#include "String.h"

using namespace JSON;

String::String() :
	Value(STRING)
{
}

String::String(const char& c) :
	Value(STRING), string(1, c)
{
}

String::String(const std::string& s) :
	Value(STRING), string(s)
{
}

String::String(const char* data) :
	Value(STRING), string(data)
{
}

String::String(const char* data, size_t len) :
	Value(STRING), string(data, len)
{
}

String::String(const String& s) :
	Value(s), string(s.string)
{
}

void String::append(const char& c)
{
	string.append(1, c);
}

void String::append(const std::string& s)
{
	string.append(s);
}

void String::append(const char* data)
{
	string.append(data);
}

void String::append(const char* data, size_t len)
{
	string.append(data, len);
}

void String::append(const String& s)
{
	string.append(s.string);
}

std::string& String::toString()
{
	return string;
}

const std::string& String::toString() const
{
	return string;
}

void String::_toJSON(std::string& json, const std::string& indent) const
{
	// escape the string
	char c;
	std::string escaped;
	std::string::const_iterator it;
	short noDigit = 0;

	for(it = string.begin(); it < string.end(); ++it)
	{
		c = *it;
		
		if (noDigit) noDigit--;

		switch (c)
		{
			case '\\':
				escaped.append("\\\\");
			break;

			case '"':
				escaped.append("\\\"");
			break;

			case 0:
				escaped.append("\\0");
				noDigit = 2;
			break;

			case '\n':
				escaped.append("\\n");
			break;

			case '\r':
				escaped.append("\\r");
			break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (noDigit)
				{
					escaped.append("\\u003"); // 0 is U+0030, 1 is U+0031 etc.
					escaped.push_back(c);
				}
				else
				{
					escaped.push_back(c);
				}
			break;

			default:
				escaped.push_back(c);
			break;
		}
	}

	json.append("\"");
	json.append(escaped);
	json.append("\"");
}

String& String::operator=(const String& s)
{
	if (&s != this)
	{
		string = s.toString();
	}
	return *this;
}

bool String::operator==(const Value& v) const
{
	if (getType() == v.getType())
	{
		return string == v.toString();
	}

	return false;
}

bool String::operator!=(const Value& v) const
{
	if (getType() == v.getType())
	{
		return string != v.toString();
	}

	return true;
}

bool String::operator>(const Value& v) const
{
	if (getType() == v.getType())
	{
		return string > v.toString();
	}

	return getType() > v.getType();
}

bool String::operator<(const Value& v) const
{
	if (getType() == v.getType())
	{
		return string < v.toString();
	}

	return getType() < v.getType();
}

bool String::operator>=(const Value& v) const
{
	if (getType() == v.getType())
	{
		return string >= v.toString();
	}

	return getType() >= v.getType();
}

bool String::operator<=(const Value& v) const
{
	if (getType() == v.getType())
	{
		return string <= v.toString();
	}

	return getType() <= v.getType();
}

