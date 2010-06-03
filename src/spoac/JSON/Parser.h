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

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "AllValueTypes.h"
#include "ParserException.h"

#include <stack>
#include <string>
#include <boost/shared_ptr.hpp>


namespace JSON
{
	/**
	* An enum of different states the Parser can reach.
	*/
	typedef enum
	{
		STATE_END,
		STATE_WAIT_VALUE,
		STATE_READ_VALUE_SKIP_WHITESPACE_AND_COMMA,
		STATE_READ_VALUE_SKIP_WHITESPACE,
		STATE_READ_VALUE,
		STATE_ARRAY,
		STATE_OBJECT,
		STATE_OBJECT_KEY,
		STATE_OBJECT_VALUE,
		STATE_IDENTIFIER_STRING,
		STATE_STRING_DOUBLE_QUOTE,
		STATE_STRING_SINGLE_QUOTE,
		STATE_STRING_ESCAPE,
		STATE_NUMBER,
		STATE_NUMBER_DECIMAL_ZERO,
		STATE_NUMBER_DECIMAL_INTEGER,
		STATE_NUMBER_DECIMAL_POINT,
		STATE_NUMBER_EXPONENT,
		STATE_NUMBER_EXPONENT_SIGNED,
		STATE_NUMBER_EXPONENT_UNSIGNED,
		STATE_NUMBER_N,
		STATE_NUMBER_NA,
		STATE_NULL_N,
		STATE_NULL_NU,
		STATE_NULL_NUL,
		STATE_BOOL_T,
		STATE_BOOL_TR,
		STATE_BOOL_TRU,
		STATE_BOOL_F,
		STATE_BOOL_FA,
		STATE_BOOL_FAL,
		STATE_BOOL_FALS,
	} ParserState;

	/**
	* A parser that constructs a JSON::Value from the given character data.
	*
	* The parser is used by calling read() and then finish() to retrieve
	* the result. read() can be called any number of times. The parser
	* continues to read the character data in the state it finished the
	* previous chunk. While finish() must only be called once as it resets
	* the parser for the next document. You can also manually reset it with
	* reset().
	*/
	class Parser
	{
	public:
		Parser();

		void reset();
		void read(const std::string& data);
		void read(const char* data, size_t len);
		ValuePtr readFromFile(const std::string& path);
		ValuePtr finish();
	private:
		Parser(const Parser&) {}; // do not allow copying
		Parser& operator=(const Parser&) { return *this; }; // do not allow assignment

		inline bool isLetter(char c);
		inline bool isCombiningMark(char c);
		inline bool isDigit(char c);
		inline bool isConnectorPunctuation(char c);
		inline void pushState(ParserState state);
		inline ParserState popState();
		inline void pushValue(ValuePtr value);
		inline ValuePtr popValue();
		inline StringPtr topString();

		std::stack<ValuePtr> values;
		std::stack<ParserState> states;

		ParserState state;

		char c;
		char lastChar;

		std::string numBuffer;

		size_t lineNumber;
		int32_t lineNumberIndex;

		bool init;
	};
}

#endif
