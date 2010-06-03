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

#include "Parser.h"

#include <fstream>

using namespace JSON;

Parser::Parser()
{
	reset();
}

void Parser::reset()
{
	while (!values.empty())
	{
		values.pop();
	}
	
	while (!states.empty())
	{
		states.pop();
	}
	
	states.push(STATE_END);
	state = STATE_WAIT_VALUE;

	init = false;
	lineNumber = 1;
	lineNumberIndex = 0;

	c = 0;
	lastChar = 0;

	numBuffer.clear();
}

#define PARSE_ERROR_SIMPLE(message) \
	throw ParserException(message, lineNumber)

#define PARSE_ERROR(message, param1, param2) \
	{ \
		std::string s(message); \
		s.append(param1); \
		s.append(param2); \
		throw ParserException(s, lineNumber); \
	}

#define SKIP_WHITESPACE(char) \
	if (char == 0x20 || char == 0x09 || char == 0x0A || char == 0x0D) \
		break;

#define CASE_START_NUMBER \
	case '-': case '+': case '.': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case 'N': \
		state = STATE_NUMBER; \
		numBuffer.clear(); \
		i--;

#define CASE_LINE_TERMINATOR \
	case '\n': case '\r':

void Parser::read(const std::string& data)
{
	read(data.c_str(), data.size());
}

void Parser::read(const char* data, size_t len)
{
	boost::shared_ptr<Value> value;
	boost::shared_ptr<Value> tmp;
	boost::shared_ptr<Identifier> key;
	lineNumberIndex = -1;

	for (size_t i = 0; i < len; ++i)
	{
		lastChar = c;
		c = data[i];

		switch (state)
		{
			case STATE_END:
				SKIP_WHITESPACE(c)

				PARSE_ERROR("Unexpected data after JSON structure: found '", std::string(1, c), "'");
			break;

			case STATE_WAIT_VALUE:
				SKIP_WHITESPACE(c);

				state = STATE_READ_VALUE;
				i--;
			break;

			case STATE_READ_VALUE_SKIP_WHITESPACE_AND_COMMA:
				SKIP_WHITESPACE(c);

				if (c == ',')
				{
					break;
				}

				state = STATE_READ_VALUE;
				i--;
			break;

			case STATE_READ_VALUE_SKIP_WHITESPACE:
				SKIP_WHITESPACE(c);

				state = STATE_READ_VALUE;
				i--;
			break;

			case STATE_READ_VALUE:
				switch (c)
				{
					case '"':
						state = STATE_STRING_DOUBLE_QUOTE;
						value.reset(new String());
						pushValue(value);
					break;
			
					case '\'':
						state = STATE_STRING_SINGLE_QUOTE;
						value.reset(new String());
						pushValue(value);
					break;
			
					case '{':
						state = STATE_OBJECT;
						value.reset(new Object());
						pushValue(value);
						init = true;
					break;
			
					case '[':
						state = STATE_ARRAY;
						value.reset(new Array());
						pushValue(value);
						init = true;
					break;
			
					case 'n':
						state = STATE_NULL_N;
					break;
			
					case 'f':
						state = STATE_BOOL_F;
					break;
			
					case 't':
						state = STATE_BOOL_T;
					break;
			
					CASE_START_NUMBER
					break;
			
					default:
						PARSE_ERROR("Unexpected character data, expecting a value: found '", std::string(1, c), "'");
					break;
				}
			break;

			case STATE_ARRAY:
				SKIP_WHITESPACE(c);
				
				if (!init)
				{
					// get the value
					value = values.top();
					values.pop();
					// put it in the array
					boost::dynamic_pointer_cast<Array>(values.top())->push_back(value);
				}
				
				switch (c)
				{
					case ']':
						state = popState();
					break;

					// there may be any number of commas in an array
					case ',':
						pushState(STATE_ARRAY);
						state = STATE_READ_VALUE_SKIP_WHITESPACE_AND_COMMA;
					break;
				
					default:
						if (init)
						{
							pushState(STATE_ARRAY);
							state = STATE_READ_VALUE;
							i--;
						}
						else
						{
							PARSE_ERROR("Unexpected character data after array element, expecting a comma: found '", std::string(1, c), "'");
						}
					break;
				}

				if (init)
				{
					init = false;
				}
			break;

			// { key  : value, key2 : value2 }
			//  ^            ^              ^^
			case STATE_OBJECT:
				SKIP_WHITESPACE(c);

				if (init)
				{
					init = false;
					state = STATE_OBJECT_KEY;
					i--;
					break;
				}
				else
				{
					// get key and value
					value = popValue();

					tmp = popValue();
					if (tmp->getType() == STRING)
					{
						key.reset(new Identifier(boost::dynamic_pointer_cast<String>(tmp)));
					}
					else if (tmp->getType() == NUMBER)
					{
						key.reset(new Identifier(boost::dynamic_pointer_cast<Number>(tmp)));
					}
					else
					{
						std::string data;
						tmp->_toJSON(data, "");
						PARSE_ERROR("Unexpected key type, expecting a string or a number: found '", data, "'");
					}

					// put it in the object
					boost::dynamic_pointer_cast<Object>(values.top())->insert(*key, value);
				}

				switch (c)
				{
					case '}':
						state = popState();
					break;

					case ',':
						state = STATE_OBJECT_KEY;
					break;

					default:
						PARSE_ERROR("Unexpected character data after object property, expecting a comma: found '", std::string(1, c) , "'");
					break;
				}
			break;


			// { key  : value, key2 : value2 }
			//   ^            ^^
			case STATE_OBJECT_KEY:
				SKIP_WHITESPACE(c);
				if (c == ',')
				{
					break;
				}

				// try reading a key, otherwise look for end of object
				switch (c)
				{
					case '$':
					case '_':
						state = STATE_IDENTIFIER_STRING;
						value.reset(new String(c));
						pushValue(value);
						pushState(STATE_OBJECT_VALUE);
					break;

					case '"':
						state = STATE_STRING_DOUBLE_QUOTE;
						value.reset(new String());
						pushValue(value);
						pushState(STATE_OBJECT_VALUE);
					break;
			
					case '\'':
						state = STATE_STRING_SINGLE_QUOTE;
						value.reset(new String());
						pushValue(value);
						pushState(STATE_OBJECT_VALUE);
					break;

					CASE_START_NUMBER
						pushState(STATE_OBJECT_VALUE);
					break;

					case '}':
						state = popState();
					break;

					default:
						if (isLetter(c))
						{
							state = STATE_IDENTIFIER_STRING;
							value.reset(new String(c));
							pushValue(value);
							pushState(STATE_OBJECT_VALUE);
						}
						else
						{
							PARSE_ERROR("Unexpected character data, expecting an object key: found '", std::string(1, c), "'");
						}
					break;
				}
			break;

			// { key  : value, key2 : value2 }
			//      ^^^            ^^
			case STATE_OBJECT_VALUE:
				SKIP_WHITESPACE(c);

				if (values.top()->getType() != STRING && values.top()->getType() != NUMBER)
				{
					std::string data;
					values.top()->_toJSON(data, "");
					PARSE_ERROR("Unexpected key type, expecting a string or a number: found '", data , "'");
				}

				if (c != ':')
				{
					PARSE_ERROR("Unexpected character data after object key, expecting a colon: found '", std::string(1, c), "'");
				}
				else
				{
					pushState(STATE_OBJECT);
					state = STATE_READ_VALUE_SKIP_WHITESPACE;
				}
			break;

			case STATE_IDENTIFIER_STRING:
				// assumption: first character was already matched as valid start character

				switch (c)
				{
					case '$':
					case '_':
						topString()->append(c);
					break;

					default:
						if (isLetter(c) || isCombiningMark(c) || isDigit(c) || isConnectorPunctuation(c))
						{
							topString()->append(c);
						}
						else
						{
							state = popState();
							i--;
						}
					break;
				}
			break;

			case STATE_STRING_DOUBLE_QUOTE:
				switch (c)
				{
					case '"':
						state = popState();
					break;

					case '\\':
						state = STATE_STRING_ESCAPE;
						pushState(STATE_STRING_DOUBLE_QUOTE);
					break;
/*
					CASE_LINE_TERMINATOR
						PARSE_ERROR_SIMPLE("Unescaped line terminator in double quoted string");
					break;*/

					default:
						topString()->append(c);
					break;
				}
			break;

			case STATE_STRING_SINGLE_QUOTE:
				switch (c)
				{
					case '\'':
						state = popState();
					break;

					case '\\':
						state = STATE_STRING_ESCAPE;
						pushState(STATE_STRING_SINGLE_QUOTE);
					break;
/*
					CASE_LINE_TERMINATOR
						PARSE_ERROR_SIMPLE("Unescaped line terminator in single quoted string");
					break;
*/
					default:
						topString()->append(c);
					break;
				}
			break;

			case STATE_STRING_ESCAPE:
				switch (c)
				{
					case 'b':
						topString()->append('\b');
					break;
					case 'f':
						topString()->append('\f');
					break;
					case 'n':
						topString()->append('\n');
					break;
					case 'r':
						topString()->append('\r');
					break;
					case 't':
						topString()->append('\t');
					break;
/*
					CASE_LINE_TERMINATOR
						PARSE_ERROR_SIMPLE("Unescaped line terminator in string");
					break;
*/
					case '\'':
					case '\"':
					case '\\':
					case '/':
						topString()->append(c);
					break;

					default:
						topString()->append('\\');
						topString()->append(c);
					break;
				}

				state = popState();
			break;

			case STATE_NUMBER:
				switch (c)
				{
					case '0':
						numBuffer.push_back((char) c);
						state = STATE_NUMBER_DECIMAL_ZERO;
					break;

					// allow a leading -
					case '-':
					case '+':
						if (numBuffer.length() > 1)
						{
							PARSE_ERROR("Illegal leading sign, numbers must only be prefixed with a single sign: found '", std::string(1, c), "'");
						}
						else
						{
							numBuffer.push_back((char) c);
						}
					break;

					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						numBuffer.push_back((char) c);
						state = STATE_NUMBER_DECIMAL_INTEGER;
					break;

					case '.':
						numBuffer.push_back((char) c);
						state = STATE_NUMBER_DECIMAL_POINT;
					break;

					case 'N':
						state = STATE_NUMBER_N;
					break;

					default:
						PARSE_ERROR("Unexpected character data, expecting number: found '", std::string(1, c), "'");
					break;
				}
			break;

			case STATE_NUMBER_DECIMAL_ZERO:
				switch (c)
				{
					case '.':
						numBuffer.push_back('.');
						state = STATE_NUMBER_DECIMAL_POINT;
					break;

					case 'e':
					case 'E':
						numBuffer.push_back('E');
						state = STATE_NUMBER_EXPONENT;
					break;

					default:
						value.reset(new Number(numBuffer));
						pushValue(value);
						state = popState();
						i--;
					break;
				}
			break;

			case STATE_NUMBER_DECIMAL_INTEGER:
				switch (c)
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						numBuffer.push_back((char) c);
					break;

					case '.':
						numBuffer.push_back('.');
						state = STATE_NUMBER_DECIMAL_POINT;
					break;

					case 'e':
					case 'E':
						numBuffer.push_back('E');
						state = STATE_NUMBER_EXPONENT;
					break;

					default:
						value.reset(new Number(numBuffer));
						pushValue(value);
						state = popState();
						i--;
					break;
				}
			break;

			case STATE_NUMBER_DECIMAL_POINT:
				switch (c)
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						numBuffer.push_back((char) c);
					break;

					case '.':
						numBuffer.push_back('.');
						state = STATE_NUMBER_DECIMAL_POINT;
					break;

					case 'e':
					case 'E':
						numBuffer.push_back('E');
						state = STATE_NUMBER_EXPONENT;
					break;

					default:
						// a decimal point alone is not a number
						if (numBuffer.length() == 1)
						{
							PARSE_ERROR("Unexpected character data, expecting digit after decimal point: found '", std::string(1, c), "'");
						}
						else
						{
							value.reset(new Number(numBuffer));
							pushValue(value);
							state = popState();
							i--;
						}
					break;
				}
			break;

			case STATE_NUMBER_EXPONENT:
				switch (c)
				{
					case '-':
					case '+':
						numBuffer.push_back((char) c);
						state = STATE_NUMBER_EXPONENT_SIGNED;
					break;

					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						numBuffer.push_back((char) c);
						state = STATE_NUMBER_EXPONENT_UNSIGNED;
					break;

					default:
						PARSE_ERROR("Unexpected character data, expecting exponent value: found '", std::string(1, c), "'");
					break;
				}
			break;

			case STATE_NUMBER_EXPONENT_SIGNED:
				switch (c)
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						numBuffer.push_back((char) c);
						state = STATE_NUMBER_EXPONENT_UNSIGNED;
					break;

					default:
						PARSE_ERROR("Unexpected character data, expecting exponent value: found '", std::string(1, c), "'");
					break;
				}
			break;

			case STATE_NUMBER_EXPONENT_UNSIGNED:
				switch (c)
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						numBuffer.push_back((char) c);
					break;

					default:
						value.reset(new Number(numBuffer));
						pushValue(value);
						state = popState();
						i--;
					break;
				}
			break;

			case STATE_NUMBER_N:
				if (c == 'a')
				{
					state = STATE_NUMBER_NA;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'a' in NaN: found '", std::string(1, c), "'");
				}
			break;

			case STATE_NUMBER_NA:
				if (c == 'N')
				{
					value.reset(new Number(std::string("NaN")));
					pushValue(value);
					state = popState();
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'N' in NaN: found '", std::string(1, c), "'");
				}
			break;

			case STATE_NULL_N:
				if (c == 'u')
				{
					state = STATE_NULL_NU;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'u' in null: found '", std::string(1, c), "'");
				}
			break;

			case STATE_NULL_NU:
				if (c == 'l')
				{
					state = STATE_NULL_NUL;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'l' in null: found '", std::string(1, c), "'");
				}
			break;

			case STATE_NULL_NUL:
				if (c == 'l')
				{
					value.reset(new Null);
					pushValue(value);
					state = popState();
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'l' in null: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_T:
				if (c == 'r')
				{
					state = STATE_BOOL_TR;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'r' in true: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_TR:
				if (c == 'u')
				{
					state = STATE_BOOL_TRU;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'u' in true: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_TRU:
				if (c == 'e')
				{
					value.reset(new Bool(true));
					pushValue(value);
					state = popState();
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'e' in true: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_F:
				if (c == 'a')
				{
					state = STATE_BOOL_FA;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'a' in false: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_FA:
				if (c == 'l')
				{
					state = STATE_BOOL_FAL;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'l' in false: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_FAL:
				if (c == 's')
				{
					state = STATE_BOOL_FALS;
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 's' in false: found '", std::string(1, c), "'");
				}
			break;

			case STATE_BOOL_FALS:
				if (c == 'e')
				{
					value.reset(new Bool(false));
					pushValue(value);
					state = popState();
				}
				else
				{
					PARSE_ERROR("Unexpected character data, expecting 'e' in false: found '", std::string(1, c), "'");
				}
			break;

			default:
			break;
		}

		// count lines, but don't count the same line break twice
		if ((c == '\r' || (c == '\n' && lastChar != '\r'))  && lineNumberIndex < (int32_t) i)
		{
			lineNumber++;
			lineNumberIndex = i;
		}
	}
}

inline bool Parser::isLetter(char c)
{
	return (c >= 65 && c <= 91) || (c >= 97 && c <= 122);
}

inline bool Parser::isCombiningMark(char c)
{
	return false;
}

inline bool Parser::isDigit(char c)
{
	return (c >= 48 && c <= 57);
}

inline bool Parser::isConnectorPunctuation(char c)
{
	return (c == '_');
}

inline void Parser::pushState(ParserState state)
{
	states.push(state);
}

inline ParserState Parser::popState()
{
	if (states.empty())
	{
		PARSE_ERROR_SIMPLE("ParserState stack empty");
	}

	ParserState state = states.top();
	states.pop();
	return state;
}

inline void Parser::pushValue(boost::shared_ptr<Value> value)
{
	values.push(value);
}

inline boost::shared_ptr<Value> Parser::popValue()
{
	if (values.empty())
	{
		return boost::shared_ptr<Value>();
	}

	boost::shared_ptr<Value> v = values.top();
	values.pop();

	return v;
}

inline boost::shared_ptr<String> Parser::topString()
{
	return boost::dynamic_pointer_cast<String>(values.top());
}

boost::shared_ptr<Value> Parser::readFromFile(const std::string& path)
{
	std::ifstream file;
	std::string line;
		
	file.open(path.c_str());
	
	if (!file.is_open())
			PARSE_ERROR_SIMPLE("file could not be opened");
		
	while (!file.eof())
	{
		getline(file, line);
		this->read(line.c_str(), line.size());
	}
	
	file.close();
	return this->finish();
}

boost::shared_ptr<Value> Parser::finish()
{
	// read a space to make sure everything is over
	read(" ", 1);

	if (state != STATE_END)
	{
		PARSE_ERROR_SIMPLE("Incomplete JSON structure");
		return boost::shared_ptr<Value>();
	}

	boost::shared_ptr<Value> result = popValue();

	// restore initial state
	reset();

	return result;
}

