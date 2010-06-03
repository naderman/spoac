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

#include "Identifier.h"
#include "String.h"
#include "Number.h"

using namespace JSON;

Identifier::Identifier(NumberPtr n) :
	type(NUMBER),
	value(n)
{
}

Identifier::Identifier(const Number& n) :
	type(NUMBER),
	value(new Number(n))
{
}

Identifier::Identifier(StringPtr s) :
	type(STRING),
	value(s)
{
}

Identifier::Identifier(const String& s) :
	type(STRING),
	value(new String(s))
{
}

Identifier::Identifier(const Identifier& i) :
	type(i.type)
{
	switch (i.getType())
	{
		case STRING:
			value.reset(new String(*(i.getString())));
		break;

		case NUMBER:
			value.reset(new Number(*(i.getNumber())));
		break;

		default:
		break;
	}
}

ValueType Identifier::getType() const
{
	return type;
}

StringPtr Identifier::getString() const
{
	if (getType() != STRING)
	{
		return StringPtr();
	}
	return boost::dynamic_pointer_cast<String>(value);
}

NumberPtr Identifier::getNumber() const
{
	if (getType() != NUMBER)
	{
		return NumberPtr();
	}
	return boost::dynamic_pointer_cast<Number>(value);
}

void Identifier::_toJSON(std::string& json, const std::string& indent) const
{
	value->_toJSON(json, indent);
}

Identifier& Identifier::operator=(const Identifier& i)
{
	if (&i != this)
	{
		switch (i.getType())
		{
			case STRING:
				value.reset(new String(*(i.getString())));
			break;

			case NUMBER:
				value.reset(new Number(*(i.getNumber())));
			break;

			default:
			break;
		}
	}
	return *this;
}

#define COMPARE(operator) \
	if (getType() == i.getType()) \
		switch (getType()) \
		{ \
			case STRING: \
				return *getString() operator *(i.getString()); \
			case NUMBER: \
				return *getNumber() operator *(i.getNumber()); \
			default: \
				return (getType() operator i.getType()); \
		} \
	else return (getType() operator i.getType());

bool Identifier::operator==(const Identifier& i) const
{
	COMPARE(==)
}

bool Identifier::operator!=(const Identifier& i) const
{
	COMPARE(!=)
}

bool Identifier::operator>(const Identifier& i) const
{
	COMPARE(>)
}

bool Identifier::operator<(const Identifier& i) const
{
	COMPARE(<)
}

bool Identifier::operator>=(const Identifier& i) const
{
	COMPARE(>=)
}

bool Identifier::operator<=(const Identifier& i) const
{
	COMPARE(<=)
}
