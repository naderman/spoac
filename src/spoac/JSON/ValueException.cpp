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

#include "ValueException.h"

using namespace JSON;

ValueException::ValueException(const ValueType from, const ValueType to) :
	Exception("ValueException: Cannot convert Value from "), from(from), to(to)
{
	errorMessage.append(typeString(from));
	errorMessage.append(" to ");
	errorMessage.append(typeString(to));
}

ValueType ValueException::getFrom() const
{
	return from;
}

ValueType ValueException::getTo() const
{
	return to;
}

std::string ValueException::typeString(const ValueType t)
{
	switch (t)
	{
		case NULLTYPE:
			return "Null";

		case BOOL:
			return "Bool";

		case NUMBER:
			return "Number";

		case STRING:
			return "String";

		case ARRAY:
			return "Array";

		case OBJECT:
			return "Object";

		default:
			return "unknown type";
	}
}

