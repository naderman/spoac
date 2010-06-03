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

#include <sstream>
#include "ParserException.h"

using namespace JSON;

ParserException::ParserException(const std::string& error, size_t lineNumber) :
	Exception("ParserException (Line "), lineNumber(lineNumber)
{
	std::string num;
	std::stringstream ss;

	ss << lineNumber;
	ss >> num;

	errorMessage.append(num.c_str());
	errorMessage.append("): ");
	errorMessage.append(error);
}

size_t ParserException::getLineNumber() const
{
	return lineNumber;
}
