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

#include "Value.h"
#include "ValueException.h"

using namespace JSON;

Value::Value(ValueType t) :
	type(t)
{
}

Value::Value(const Value& v)
{
	type = v.getType();
}

Value::Value() :
	type(NULLTYPE)
{
}

ValueType Value::getType() const
{
	return type;
}

bool Value::isNull() const
{
	return false;
}

bool Value::toBool() const
{
	throw ValueException(getType(), BOOL);
}

int64_t Value::toInt() const
{
	throw ValueException(getType(), NUMBER);
}

double Value::toDouble() const
{
	throw ValueException(getType(), NUMBER);
}

bool Value::isExactInt() const
{
	throw ValueException(getType(), NUMBER);
}

std::string& Value::toString()
{
	throw ValueException(getType(), STRING);
}

const std::string& Value::toString() const
{
	throw ValueException(getType(), STRING);
}

Array& Value::toArray()
{
	throw ValueException(getType(), ARRAY);
}

const Array& Value::toArray() const
{
	throw ValueException(getType(), ARRAY);
}

Object& Value::toObject()
{
	throw ValueException(getType(), OBJECT);
}

const Object& Value::toObject() const
{
	throw ValueException(getType(), OBJECT);
}

void Value::_toJSON(std::string& json, const std::string& indent) const
{
	json.append("undefined");
}

std::string Value::toJSON() const
{
	std::string json;
	_toJSON(json, "");
	return json;
}

bool Value::operator==(const Value& v) const
{
	return getType() == v.getType();
}

bool Value::operator!=(const Value& v) const 
{
	return getType() != v.getType();
}

bool Value::operator>(const Value& v) const
{
	return getType() > v.getType();
}

bool Value::operator<(const Value& v) const
{
	return getType() < v.getType();
}

bool Value::operator>=(const Value& v) const
{
	return getType() >= v.getType();
}

bool Value::operator<=(const Value& v) const
{
	return getType() <= v.getType();
}
