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

#ifndef JSON_IDENTIFIER_H
#define JSON_IDENTIFIER_H

#include "Value.h"

namespace JSON
{
	class String;
    typedef boost::shared_ptr<String> StringPtr;
	class Number;
    typedef boost::shared_ptr<Number> NumberPtr;

	class Identifier
	{
	public:
		Identifier(NumberPtr n);
		Identifier(const Number& n);
		Identifier(StringPtr s);
		Identifier(const String& s);
		Identifier(const Identifier& i);

		ValueType getType() const;
		StringPtr getString() const;
		NumberPtr getNumber() const;
		void _toJSON(std::string& json, const std::string& indent) const;

		Identifier& operator=(const Identifier& i);
		bool operator==(const Identifier& i) const;
		bool operator!=(const Identifier& i) const;
		bool operator>(const Identifier& i) const;
		bool operator<(const Identifier& i) const;
		bool operator>=(const Identifier& i) const;
		bool operator<=(const Identifier& i) const;
	private:
		ValueType type;
		ValuePtr value;
	};

    typedef boost::shared_ptr<Identifier> IdentifierPtr;
}

#endif
