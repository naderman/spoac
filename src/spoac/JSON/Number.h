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

#ifndef JSON_NUMBER_H
#define JSON_NUMBER_H

#include "Value.h"

namespace JSON
{
	/**
	* Represents a JSON Number.
	*
	* A number is either an integer or a double. It can be constructed from
	* a string containing the number in JSON format. To find out whether it
	* is an exact integer value you can use the isExactInt() method.
	*/
	class Number : public Value
	{
	public:
		Number();
		Number(int32_t iVal);
		Number(int64_t iVal);
		Number(double dVal);
		Number(const std::string& number);
		Number(const Number& n);

		int64_t toInt() const;
		double toDouble() const;
		void _toJSON(std::string& json, const std::string& indent) const;
		bool isExactInt() const;

		bool operator==(const Value& v) const;
		bool operator!=(const Value& v) const;
		bool operator>(const Value& v) const;
		bool operator<(const Value& v) const;
		bool operator>=(const Value& v) const;
		bool operator<=(const Value& v) const;
	private:
		int64_t intValue;
		double doubleValue;
		bool exactInt;
	};

    typedef boost::shared_ptr<Number> NumberPtr;
}

#endif
