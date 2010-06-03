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

#define BOOST_TEST_MODULE spoac_ObjectVector
#include <spoactest/test.h>

#include <iostream>
#include <vector>
#include <spoac/stm/ObjectVector.h>

using spoac::ParameterException;

BOOST_AUTO_TEST_CASE(testEmpty)
{
    spoac::ObjectVector objects;

    BOOST_CHECK_EQUAL(objects.size(), 0);

    BOOST_CHECK_NO_THROW(objects.validate(0));

    BOOST_CHECK_THROW(objects.validate(1), ParameterException);
    BOOST_CHECK_THROW(objects.validate("foo"), ParameterException);
    BOOST_CHECK_THROW(objects.getValidated(), ParameterException);
    BOOST_CHECK_THROW(objects.getValidated("foo"), ParameterException);
    BOOST_CHECK_THROW(objects.getValidated(0, "foo"), ParameterException);
}

BOOST_AUTO_TEST_CASE(testSingleElement)
{
    spoac::ObjectPtr foo1(new spoac::Object("foo", "foo1"));

    spoac::ObjectVector objects;
    objects.push_back(foo1);

    BOOST_CHECK_EQUAL(objects.size(), 1);
    BOOST_CHECK_EQUAL(objects.getValidated()->getId(), "foo1");
    BOOST_CHECK_EQUAL(objects.getValidated(0)->getId(), "foo1");
    BOOST_CHECK_EQUAL(objects.getValidated("foo")->getId(), "foo1");
    BOOST_CHECK_EQUAL(objects.getValidated(0, "foo")->getId(), "foo1");

    BOOST_CHECK_NO_THROW(objects.validate(1));
    BOOST_CHECK_NO_THROW(objects.validate("foo"));

    BOOST_CHECK_THROW(objects.validate(0), ParameterException);
    BOOST_CHECK_THROW(objects.validate(2), ParameterException);
    BOOST_CHECK_THROW(objects.validate("foo", "bar"), ParameterException);
    BOOST_CHECK_THROW(
        objects.validate("foo", "bar", "foo23"), ParameterException);
    BOOST_CHECK_THROW(objects.getValidated(1, "foo"), ParameterException);
}

BOOST_AUTO_TEST_CASE(testTwoElements)
{
    spoac::ObjectPtr foo1(new spoac::Object("foo", "foo1"));
    spoac::ObjectPtr bar1(new spoac::Object("bar", "bar1"));

    spoac::ObjectVector objects;
    objects.push_back(foo1);
    objects.push_back(bar1);

    BOOST_CHECK_EQUAL(objects.size(), 2);
    BOOST_CHECK_EQUAL(objects.getValidated(0, "foo")->getId(), "foo1");
    BOOST_CHECK_EQUAL(objects.getValidated(1, "bar")->getId(), "bar1");
    BOOST_CHECK_EQUAL(objects.getValidated(0)->getId(), "foo1");
    BOOST_CHECK_EQUAL(objects.getValidated(1)->getId(), "bar1");

    BOOST_CHECK_NO_THROW(objects.validate(2));
    BOOST_CHECK_NO_THROW(objects.validate("foo", "bar"));

    BOOST_CHECK_THROW(objects.validate(0), ParameterException);
    BOOST_CHECK_THROW(objects.validate(1), ParameterException);
    BOOST_CHECK_THROW(objects.validate(3), ParameterException);
    BOOST_CHECK_THROW(objects.validate("foo"), ParameterException);
    BOOST_CHECK_THROW(
        objects.validate("foo", "bar", "foo23"), ParameterException);
    BOOST_CHECK_THROW(objects.getValidated("foo"), ParameterException);
    BOOST_CHECK_THROW(objects.getValidated(1, "foo"), ParameterException);
}
