/**
* This file is part of SPOAC.
*
* SPOAC is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* SPOAC is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* @package    SPOAC
* @author     Nils Adermann <naderman at naderman dot de>
* @copyright  2010 Nils Adermann
* @license    http://www.gnu.org/licenses/gpl.txt
*             GNU General Public License
*/

#define BOOST_TEST_MODULE spoac_Object
#include <spoactest/test.h>

#include <iostream>
#include <vector>
#include <spoac/stm/Object.h>

BOOST_AUTO_TEST_CASE(testEmptyObject)
{
    spoac::Object object("foo", "foo1");

    BOOST_CHECK_EQUAL(object.getId(), "foo1");
    BOOST_CHECK_EQUAL(object.getName(), "foo");

    BOOST_CHECK(object.empty());
    BOOST_CHECK_EQUAL(object.get<std::string>("bar"), std::string(""));
    BOOST_CHECK_EQUAL(object.get<int>("bar", 5), 5);
}

BOOST_AUTO_TEST_CASE(testOneElementObject)
{
    spoac::Object object("foo", "foo1");

    object["bar"] = std::string("bar");

    BOOST_CHECK(!object.empty());
    BOOST_CHECK_EQUAL(object.size(), 1);

    BOOST_CHECK_EQUAL(object.get<std::string>("bar"), std::string("bar"));
    BOOST_CHECK_EQUAL(object.get<int>("bar", 5), 5);
}

BOOST_AUTO_TEST_CASE(testJSON)
{
    spoac::Object object("foo", "foo1");

    object["bar"] = std::string("bar");
    object["foobar"] = 1337;
    object["42"] = 0.23;

    JSON::StringPtr s1(new JSON::String("abc"));
    JSON::StringPtr s2(new JSON::String("def"));

    JSON::ArrayPtr array(new JSON::Array());
    array->push_back(s1);
    array->push_back(s2);

    object["json"] = array;

    BOOST_CHECK_EQUAL(object.toJSONString(), "{\n"
"\t\"42\": 0.23,\n"
"\t\"bar\": \"bar\",\n"
"\t\"foobar\": 1337,\n"
"\t\"json\": [\n"
"\t\t\"abc\",\n"
"\t\t\"def\"\n"
"\t]\n"
"}");
}
