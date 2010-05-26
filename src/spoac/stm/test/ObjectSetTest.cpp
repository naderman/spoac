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

#define BOOST_TEST_MODULE spoac_ObjectSet
#include <spoactest/test.h>

#include <iostream>
#include <vector>
#include <spoac/stm/ObjectSet.h>

using spoac::STMException;

BOOST_AUTO_TEST_CASE(testEmpty)
{
    spoac::ObjectSet objects;

    BOOST_CHECK_EQUAL(objects.size(), 0);

    BOOST_CHECK_THROW(objects["foo"], STMException);
}

BOOST_AUTO_TEST_CASE(testSingleElement)
{
    spoac::ObjectPtr foo1(new spoac::Object("foo", "foo1"));

    spoac::ObjectSet objects;

    objects.insert(foo1);
    BOOST_CHECK_EQUAL(objects.size(), 1);

    objects.insert(foo1);
    BOOST_CHECK_EQUAL(objects.size(), 1);

    BOOST_CHECK_EQUAL(objects["foo1"]->getId(), "foo1");

    BOOST_CHECK_THROW(objects["bar"], STMException);
}

BOOST_AUTO_TEST_CASE(testTwoElements)
{
    spoac::ObjectPtr foo1(new spoac::Object("foo", "foo1"));
    spoac::ObjectPtr bar1(new spoac::Object("bar", "bar1"));

    spoac::ObjectSet objects;

    objects.insert(foo1);
    objects.insert(bar1);
    BOOST_CHECK_EQUAL(objects.size(), 2);

    BOOST_CHECK_EQUAL(objects["foo1"]->getId(), "foo1");
    BOOST_CHECK_EQUAL(objects["bar1"]->getId(), "bar1");

    BOOST_CHECK_THROW(objects["foobar23"], STMException);
}
