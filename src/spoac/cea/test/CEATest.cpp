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

#define BOOST_TEST_MODULE spoac_CEA
#include <spoactest/test.h>

#include <iostream>
#include <vector>
#include <spoac/stm/STM.h>
#include <spoac/cea/CEA.h>
#include <spoac/cea/Action.h>

#include "DummyActivityController.h"
#include "CountTwoAction.h"
#include "../../stm/test/CountPerceptionHandler.h"

BOOST_AUTO_TEST_CASE(testCEAComplete)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    spoac::STMPtr stm = manager->getService<spoac::STM>();

    stm->addPerceptionHandler("CountPerceptionHandler", manager);

    spoac::CEAPtr cea(new spoac::CEA(
        manager->getService<spoac::STM>(),
        manager
    ));
    spoac::OACPtr oac(new spoac::OAC("CountTwo", std::vector<std::string>()));

    boost::shared_ptr<spoactest::DummyActivityController> activityController(
        new spoactest::DummyActivityController(
            spoac::CEAControlWeakPtr(cea), oac
        )
    );

    cea->addActivityController(activityController);

    spoactest::CountPerceptionHandler::counter = 0;
    spoactest::CountTwoAction::counter = 0;

    // action should only be executed 2 times, last run should not do anything
    cea->run();
    cea->run();
    cea->run();

    BOOST_CHECK_EQUAL(spoactest::CountPerceptionHandler::counter, 3);
    BOOST_CHECK_EQUAL(spoactest::CountTwoAction::counter, 2);
}

