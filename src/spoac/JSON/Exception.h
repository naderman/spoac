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

#ifndef JSON_EXCEPTION_H
#define JSON_EXCEPTION_H

#include <string>

namespace JSON
{
	class Exception : public std::exception
	{
	public:
		Exception();
		Exception(const std::string& error);
		virtual ~Exception() throw() {};
		virtual std::string getMessage() const throw();
		virtual const char* what() const throw();
	protected:
		std::string errorMessage;
	};
}

#endif
