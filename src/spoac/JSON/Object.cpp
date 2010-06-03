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

#include "Object.h"
#include "Identifier.h"
#include "String.h"
#include "Number.h"

using namespace JSON;

Object::Object() :
	Value(OBJECT)
{
}

Object::Object(const Object& o) :
	Value(o),
	map(o.map)
{
}

bool Object::empty() const
{
	return map.empty();
}

size_t Object::size() const
{
	return map.size();
}


Object::iterator Object::begin()
{
	return map.begin();
}

Object::iterator Object::end()
{
	return map.end();
}

Object::const_iterator Object::begin() const
{
	return map.begin();
}

Object::const_iterator Object::end() const
{
	return map.end();
}


Object& Object::operator=(const Object& o)
{
	if (&o != this)
	{
		map = o.map;
	}
	return *this;
}


Object::value_type& Object::operator[](const Identifier& key)
{
	Object::value_type& value = map[key];
	
	if (value == Object::value_type()) // make sure it's a valid pointer
	{
		value = Object::value_type(new Value);
	}
	
	return map[key];
}

Object::value_type& Object::operator[](const Number& key)
{
	Identifier i(key);
	return (*this)[i];
}

Object::value_type& Object::operator[](const String& key)
{
	Identifier i(key);
	return (*this)[i];
}

Object::value_type& Object::operator[](const char* key)
{
	String i(key);
	return (*this)[i];
}

Object::value_type& Object::operator[](const std::string& key)
{
	String i(key);
	return (*this)[i];
}

Object::value_type& Object::operator[](int32_t key)
{
	Number i(key);
	return (*this)[i];
}

Object::value_type& Object::operator[](int64_t key)
{
	Number i(key);
	return (*this)[i];
}

Object::value_type& Object::operator[](double key)
{
	Number i(key);
	return (*this)[i];
}


Object::iterator Object::find(const Identifier& key)
{
	return map.find(key);
}

Object::iterator Object::find(const String& key)
{
	Identifier i(key);
	return find(i);
}

Object::iterator Object::find(const Number& key)
{
	Identifier i(key);
	return find(i);
}

Object::iterator Object::find(const char* key)
{
	String i(key);
	return find(i);
}

Object::iterator Object::find(const std::string& key)
{
	String i(key);
	return find(i);
}

Object::iterator Object::find(int32_t key)
{
	Number i(key);
	return find(i);
}

Object::iterator Object::find(int64_t key)
{
	Number i(key);
	return find(i);
}

Object::iterator Object::find(double key)
{
	Number i(key);
	return find(i);
}


Object::const_iterator Object::find(const Identifier& key) const
{
	return map.find(key);
}

Object::const_iterator Object::find(const String& key) const
{
	Identifier i(key);
	return find(i);
}

Object::const_iterator Object::find(const Number& key) const
{
	Identifier i(key);
	return find(i);
}

Object::const_iterator Object::find(const char* key) const
{
	String i(key);
	return find(i);
}

Object::const_iterator Object::find(int32_t key) const
{
	Number i(key);
	return find(i);
}

Object::const_iterator Object::find(int64_t key) const
{
	Number i(key);
	return find(i);
}

Object::const_iterator Object::find(double key) const
{
	Number i(key);
	return find(i);
}


std::pair<Object::iterator, bool> Object::insert(const Identifier& key, value_type val)
{
	return map.insert(std::pair<Identifier, value_type>(key, val));
}

std::pair<Object::iterator, bool> Object::insert(const String& key, value_type val)
{
	Identifier i(key);
	return insert(i, val);
}

std::pair<Object::iterator, bool> Object::insert(const Number& key, value_type val)
{
	Identifier i(key);
	return insert(i, val);
}

std::pair<Object::iterator, bool> Object::insert(const char* key, value_type val)
{
	String i(key);
	return insert(i, val);
}

std::pair<Object::iterator, bool> Object::insert(const std::string& key, value_type val)
{
	String i(key);
	return insert(i, val);
}

std::pair<Object::iterator, bool> Object::insert(int32_t key, value_type val)
{
	Number i(key);
	return insert(i, val);
}

std::pair<Object::iterator, bool> Object::insert(int64_t key, value_type val)
{
	Number i(key);
	return insert(i, val);
}

std::pair<Object::iterator, bool> Object::insert(double key, value_type val)
{
	Number i(key);
	return insert(i, val);
}


void Object::erase(const Identifier& key)
{
	iterator position = find(key);
	map.erase(position);
}

void Object::erase(const String& key)
{
	Identifier i(key);
	erase(i);
}

void Object::erase(const Number& key)
{
	Identifier i(key);
	erase(i);
}

void Object::erase(const char* key)
{
	String i(key);
	erase(i);
}

void Object::erase(const std::string& key)
{
	String i(key);
	erase(i);
}

void Object::erase(int32_t key)
{
	Number i(key);
	erase(i);
}

void Object::erase(int64_t key)
{
	Number i(key);
	erase(i);
}

void Object::erase(double key)
{
	Number i(key);
	erase(i);
}

Object& Object::toObject()
{
	return *this;
}

const Object& Object::toObject() const
{
	return *this;
}

void Object::_toJSON(std::string& json, const std::string& indent) const
{
	std::string subindent = indent;
	subindent.append("\t");

	if (!empty())
	{
		json.append("{\n");
		json.append(subindent);
	
		for (Object::const_iterator it = begin(); it != end(); ++it)
		{
			if (it != begin())
			{
				json.append(",\n");
				json.append(subindent);
			}
			
			it->first._toJSON(json, subindent);
			json.append(": ");
			if (it->second)
			{
				it->second->_toJSON(json, subindent);
			}
			else
			{
				json.append("null");
			}
		}
	
		json.append("\n");
		json.append(indent);
		json.append("}");
	}
	else
	{
		json.append("{}");
	}
}
