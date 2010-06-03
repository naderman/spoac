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

#include "Number.h"
#include <sstream>
#include <limits>

using namespace JSON;

Number::Number() :
	Value(NUMBER)
{
	intValue = 0;
	doubleValue = 0.0;
	exactInt = true;
}

Number::Number(int32_t iVal) :
	Value(NUMBER)
{
	intValue = iVal;
	doubleValue = iVal;
	exactInt = true;
}

Number::Number(int64_t iVal) :
	Value(NUMBER)
{
	intValue = iVal;
	doubleValue = iVal;
	exactInt = true;
}

Number::Number(double dVal) :
	Value(NUMBER)
{
	intValue = (int64_t) dVal;
	doubleValue = dVal;
	exactInt = ((double) intValue == dVal) ? true : false;
}

Number::Number(const std::string& number) :
	Value(NUMBER)
{

	if (number == "NaN")
	{
		doubleValue = std::numeric_limits<double>::quiet_NaN();
		intValue = (int64_t) doubleValue;
		exactInt = false;
		return;
	}

	// check whether it's a double
	bool isDouble = false;
	for (std::string::const_iterator it = number.begin(); !isDouble && it != number.end(); ++it)
	{
		switch (*it)
		{
			case 'e':
			case 'E':
			case '.':
				isDouble = true;
			break;
		}
	}
	// parse the number
	std::istringstream in(number);

	if (isDouble)
	{
		in >> doubleValue;
		intValue = (int64_t) doubleValue;
	}
	else
	{
		in >> intValue;
		doubleValue = intValue;
	}
	exactInt = ((double) intValue == doubleValue) ? true : false;
}

Number::Number(const Number& n) :
	Value(NUMBER)
{
	intValue = n.toInt();
	doubleValue = n.toDouble();
	exactInt = n.isExactInt();
}

int64_t Number::toInt() const
{
	return intValue;
}

double Number::toDouble() const
{
	return doubleValue;
}

bool Number::isExactInt() const
{
	return exactInt;
}

void Number::_toJSON(std::string& json, const std::string& indent) const
{
	// use a stringstream to read in the number and then write it into a
	// tring
	std::stringstream ss;
	std::string num;

	if (exactInt)
	{
		ss << intValue;
	}
	else
	{
		ss << doubleValue;
	}

	ss >> num;
	
	json.append(num);
}

bool Number::operator==(const Value& v) const
{
	if (getType() == v.getType())
	{
		return ((exactInt) ? intValue : doubleValue) == ((v.isExactInt()) ? v.toInt() : v.toDouble()); 
	}

	return false;
}

bool Number::operator!=(const Value& v) const
{
	if (getType() == v.getType())
	{
		return ((exactInt) ? intValue : doubleValue) != ((v.isExactInt()) ? v.toInt() : v.toDouble()); 
	}

	return true;
}

bool Number::operator>(const Value& v) const
{
	if (getType() == v.getType())
	{
		return ((exactInt) ? intValue : doubleValue) > ((v.isExactInt()) ? v.toInt() : v.toDouble()); 
	}

	return getType() > v.getType();
}

bool Number::operator<(const Value& v) const
{
	if (getType() == v.getType())
	{
		return ((exactInt) ? intValue : doubleValue) < ((v.isExactInt()) ? v.toInt() : v.toDouble()); 
	}

	return getType() < v.getType();
}

bool Number::operator>=(const Value& v) const
{
	if (getType() == v.getType())
	{
		return ((exactInt) ? intValue : doubleValue) >= ((v.isExactInt()) ? v.toInt() : v.toDouble()); 
	}

	return getType() >= v.getType();
}

bool Number::operator<=(const Value& v) const
{
	if (getType() == v.getType())
	{
		return ((exactInt) ? intValue : doubleValue) <= ((v.isExactInt()) ? v.toInt() : v.toDouble()); 
	}

	return getType() <= v.getType();
}

