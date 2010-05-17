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
