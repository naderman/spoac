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

#ifndef JSON_PARSEREXCEPTION_H
#define JSON_PARSEREXCEPTION_H

#include <cstring>
#include "Exception.h"

namespace JSON
{
	class ParserException : public Exception
	{
	public:
		ParserException(const std::string& error, size_t lineNumber);
		virtual ~ParserException() throw() {};
		size_t getLineNumber() const;
	private:
		size_t lineNumber;
	};
}

#endif
