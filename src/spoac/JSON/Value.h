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

#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <stdint.h>
#include <string>
#include <boost/shared_ptr.hpp>

namespace JSON
{
	class Array;
	class Object;

	/**
	* An enum of all JSON types.
	*/
	typedef enum
	{
		NULLTYPE,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT
	} ValueType;

	/**
	* Base class for all JSON types.
	*
	* This class defines a basic interface that you can use to retrieve
	* type specific information from a value. By default they all throw
	* a ValueException so subclasses override the methods relevant for
	* their type to return useful data.
	*/
	class Value
	{
	public:
		Value(ValueType t);
		Value(const Value& v);
		Value();
		virtual ~Value() {};
		ValueType getType() const;
		virtual bool isNull() const;
		virtual int64_t toInt() const;
		virtual double toDouble() const;
		virtual bool isExactInt() const;
		virtual bool toBool() const;
		virtual std::string& toString();
		virtual const std::string& toString() const;
		virtual Array& toArray();
		virtual const Array& toArray() const;
		virtual Object& toObject();
		virtual const Object& toObject() const;
		virtual void _toJSON(std::string& json, const std::string& indent) const;
		std::string toJSON() const;

		virtual bool operator==(const Value& v) const;
		virtual bool operator!=(const Value& v) const;
		virtual bool operator>(const Value& v) const;
		virtual bool operator<(const Value& v) const;
		virtual bool operator>=(const Value& v) const;
		virtual bool operator<=(const Value& v) const;
	protected:
		ValueType type;
	};

    typedef boost::shared_ptr<Value> ValuePtr;
}

#endif
