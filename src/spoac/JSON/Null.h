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

#ifndef JSON_NULL_H
#define JSON_NULL_H

#include "Value.h"

namespace JSON
{
	/**
	* Represents a JSON Null.
	*
	* This class is a simple subclass of Value which encodes as "null"
	* in JSON.
	*/
	class Null : public Value
	{
	public:
		Null();

		bool isNull() const;
		void _toJSON(std::string& json, const std::string& indent) const;
	private:
	};

    typedef boost::shared_ptr<Null> NullPtr;
}

#endif
