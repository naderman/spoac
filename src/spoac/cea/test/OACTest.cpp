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

#define BOOST_TEST_MODULE spoac_OAC
#include <spoactest/test.h>

#include <iostream>
#include <spoac/cea/OAC.h>

BOOST_AUTO_TEST_CASE(testOAC)
{
    std::string name("foo");
    std::vector<std::string> objects;

    spoac::OAC oac(name, objects);

    BOOST_CHECK_EQUAL(oac.getName(), name);
    //BOOST_CHECK(oac.getObjects() == objects);
    BOOST_CHECK(oac.getObjectIds() == objects);

    spoac::SymbolicExecutionSlice::Action action = oac.getIceAction();
    BOOST_CHECK_EQUAL(action.name, name);
    BOOST_CHECK_EQUAL(action.parameters.size(), 0);
}
