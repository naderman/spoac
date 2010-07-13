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

#define BOOST_TEST_MODULE spoac_LTM
#include <spoactest/test.h>

#include <iostream>
#include <spoac/ltm/LTM.h>
#include <spoac/ice/IceHelper.h>

BOOST_AUTO_TEST_CASE(testGetScenario)
{
    spoac::ice::IceHelperPtr iceHelper(new spoac::ice::IceHelper);

    setenv("MCAPROJECTHOME", "./", 1);

    Ice::ObjectPtr ltmObject = new spoac::LTM;
    iceHelper->registerAdapter(ltmObject, "LTM", "tcp -p 10099");

    spoac::LTMSlice::LTMPrx ltm;
    ltm = iceHelper->getProxy<spoac::LTMSlice::LTMPrx>("LTM:tcp -p 10099");

    spoac::LTMSlice::Scenario scenario = ltm->getScenario("abc");

    BOOST_CHECK_EQUAL(std::string("abc"), scenario.name);
    BOOST_CHECK_EQUAL(std::string("IceNetwork"), scenario.activityControllers[0]);

    BOOST_CHECK_EQUAL(1, scenario.predicates.size());
    BOOST_CHECK_EQUAL(std::string("p"), scenario.predicates[0].name);

    BOOST_CHECK_EQUAL(1, scenario.functions.size());
    BOOST_CHECK_EQUAL(std::string("f"), scenario.functions[0].name);
}
