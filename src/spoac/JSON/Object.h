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

#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "Value.h"
#include <map>

namespace JSON
{
	class Identifier;
	class Number;
	class String;

	/**
	* Represents a JSON Object containing key value pairs.
	*
	* This class implements a JSON Object using a STL map. Its interface
	* is similar to that of a standard map. Alternatively you can still
	* retrieve the map itself and work with that directly using getMap().
	*
	* Properties are identified by an Identifier. That means you can access
	* them by either a String or a Number. The object stores pointers to
	* the values. On destruction all values stored by the object are
	* deleted. Following this behaviour erase() deletes the value as well.
	* So make sure you copy data if you wish to continue using it after
	* destructing the object.
	*/
	class Object : public Value
	{
	public:
		typedef ValuePtr value_type;
		typedef boost::shared_ptr<const Value> const_value_type;
		typedef std::map<Identifier, value_type> map_type;
		typedef map_type::iterator iterator;
		typedef map_type::const_iterator const_iterator;

		Object();
		Object(const Object& o);

		bool empty() const;
		size_t size() const;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		Object& operator=(const Object& o);

		value_type& operator[](const Identifier& key);
		value_type& operator[](const String& key);
		value_type& operator[](const Number& key);
		value_type& operator[](const char* key);
		value_type& operator[](const std::string& key);
		value_type& operator[](int32_t key);
		value_type& operator[](int64_t key);
		value_type& operator[](double key);

		iterator find(const Identifier& key);
		iterator find(const String& key);
		iterator find(const Number& key);
		iterator find(const char* key);
		iterator find(const std::string& key);
		iterator find(int32_t key);
		iterator find(int64_t key);
		iterator find(double key);

		const_iterator find(const Identifier&) const;
		const_iterator find(const String&) const;
		const_iterator find(const Number&) const;
		const_iterator find(const char* key) const;
		const_iterator find(const std::string& key) const;
		const_iterator find(int32_t key) const;
		const_iterator find(int64_t key) const;
		const_iterator find(double key) const;

		std::pair<iterator, bool> insert(const Identifier& key, value_type val);
		std::pair<iterator, bool> insert(const String& key, value_type val);
		std::pair<iterator, bool> insert(const Number& key, value_type val);
		std::pair<iterator, bool> insert(const char* key, value_type val);
		std::pair<iterator, bool> insert(const std::string& key, value_type val);
		std::pair<iterator, bool> insert(int32_t key, value_type val);
		std::pair<iterator, bool> insert(int64_t key, value_type val);
		std::pair<iterator, bool> insert(double key, value_type val);

		void erase(const Identifier& key);
		void erase(const String& key);
		void erase(const Number& key);
		void erase(const char* key);
		void erase(const std::string& key);
		void erase(int32_t key);
		void erase(int64_t key);
		void erase(double key);

		Object& toObject();
		const Object& toObject() const;
		void _toJSON(std::string& json, const std::string& indent) const;
	private:
		map_type map;
	};

    typedef boost::shared_ptr<Object> ObjectPtr;
}

#endif
