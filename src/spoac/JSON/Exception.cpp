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

#include "Exception.h"

using namespace JSON;

Exception::Exception() :
	std::exception(),
	errorMessage("Default JSON Exception: Something went wrong (tm)")
{
}

Exception::Exception(const std::string& error) :
	errorMessage(error)
{
}

std::string Exception::getMessage() const throw()
{
	return errorMessage;
}

const char* Exception::what() const throw()
{
	return errorMessage.c_str();
}
