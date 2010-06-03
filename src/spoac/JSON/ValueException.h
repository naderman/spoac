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

#ifndef JSON_VALUEEXCEPTION_H
#define JSON_VALUEEXCEPTION_H

#include "Exception.h"
#include "Value.h"

namespace JSON
{
	class ValueException : public Exception
	{
	public:
		ValueException(const ValueType from, const ValueType to);
		virtual ~ValueException() throw() {};
		ValueType getFrom() const;
		ValueType getTo() const;
	private:
		ValueType from;
		ValueType to;

		std::string typeString(const ValueType t);
	};
}

#endif
