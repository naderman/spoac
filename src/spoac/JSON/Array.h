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

#ifndef ARRAY_H
#define ARRAY_H

#include "Value.h"
#include <vector>

namespace JSON
{
	/**
	* Represents a JSON Array containing a list of values.
	*
	* This class implements a JSON Array using a STL vector. Its interface
	* is similar to that of a standard vector. Alternatively you can still
	* retrieve the vector itself and work with that directly using
	* getVector().
	*
	* The array stores pointers to the values. On destruction all values
	* stored by the array are deleted. Following this behaviour erase()
	* deletes the value as well. So make sure you copy data if you wish to
	* continue using it after destructing the array.
	*/
	class Array : public Value
	{
	public:
		typedef ValuePtr value_type;
		typedef boost::shared_ptr<const Value> const_value_type;
		typedef std::vector<value_type> vector_type;
		typedef vector_type::iterator iterator;
		typedef vector_type::const_iterator const_iterator;

		Array();
		Array(const Array& a);

		size_t size() const;
		bool empty() const;
		value_type& at(size_t pos);
		const_value_type at(size_t pos) const;
		value_type& operator[](const size_t pos);
		Array& operator=(const Array& a);
		value_type& front();
		value_type& back();
		const_value_type front() const;
		const_value_type back() const;
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		void push_back(value_type value);
		void pop_back();
		void insert(size_t index, value_type value);
		void insert(iterator it, value_type value);
		void erase(size_t index);
		void erase(iterator it);
		Array& toArray();
		const Array& toArray() const;
		void _toJSON(std::string& json, const std::string& indent) const;
	private:
		vector_type vector;
	};

    typedef boost::shared_ptr<Array> ArrayPtr;
}

#endif
