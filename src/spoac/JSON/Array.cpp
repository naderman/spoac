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

#include "Array.h"

using namespace JSON;

Array::Array() :
	Value(ARRAY),
	vector()
{
}

Array::Array(const Array& a) :
	Value(a),
	vector(a.vector)
{
}

bool Array::empty() const
{
	return vector.empty();
}

size_t Array::size() const
{
	return vector.size();
}

Array::value_type& Array::at(size_t pos)
{
    Array::value_type& value = vector.at(pos);

    if (value == Array::value_type()) // make sure it's a valid pointer
    {
        value = Array::value_type(new Value);
    }

    return value;
}

Array::const_value_type Array::at(size_t pos) const
{
    Array::const_value_type value = vector.at(pos);

    if (value == Array::const_value_type()) // make sure it's a valid pointer
    {
        value = Array::const_value_type(new Value);
    }

    return value;
}

Array::value_type& Array::operator[](const size_t pos)
{
    Array::value_type& value = vector[pos];

    if (value == Array::value_type()) // make sure it's a valid pointer
    {
        value = Array::value_type(new Value);
    }

	return value;
}

Array& Array::operator=(const Array& a)
{
	if (&a != this)
	{
		vector = a.vector;
	}
	return *this;
}

Array::value_type& Array::front()
{
	return vector.front();
}

Array::value_type& Array::back()
{
	return vector.back();
}

Array::const_value_type Array::front() const
{
	return vector.front();
}

Array::const_value_type Array::back() const
{
	return vector.back();
}

Array::iterator Array::begin()
{
	return vector.begin();
}

Array::iterator Array::end()
{
	return vector.end();
}

Array::const_iterator Array::begin() const
{
	return vector.begin();
}

Array::const_iterator Array::end() const
{
	return vector.end();
}

void Array::push_back(Array::value_type value)
{
	vector.push_back(value);
}

void Array::pop_back()
{
	vector.pop_back();
}

void Array::insert(size_t index, Array::value_type value)
{
	insert(begin() + index, value);
}

void Array::insert(Array::iterator it, Array::value_type value)
{
	vector.insert(it, value);
}

void Array::erase(size_t index)
{
	Array::iterator position = begin() + index;
	erase(begin() + index);
}

void Array::erase(Array::iterator it)
{
	vector.erase(it);
}

Array& Array::toArray()
{
	return *this;
}

const Array& Array::toArray() const
{
	return *this;
}

void Array::_toJSON(std::string& json, const std::string& indent) const
{
	std::string subindent = indent;
	subindent.append("\t");

	if (!empty())
	{
		json.append("[\n");
		json.append(subindent);

		for (Array::const_iterator it = begin(); it != end(); ++it)
		{
			if (it != begin())
			{
				json.append(",\n");
				json.append(subindent);
			}

			(*it)->_toJSON(json, subindent);
		}

		json.append("\n");
		json.append(indent);
		json.append("]");
	}
	else
	{
		json.append("[]");
	}
}
