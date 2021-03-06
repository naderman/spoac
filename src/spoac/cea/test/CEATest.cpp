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
#include <spoac/ltm/LTM.h>

#include "DummyActivityController.h"
#include "CountTwoAction.h"
#include "CountTwoSuperAction.h"
#include "../../stm/test/CountPerceptionHandler.h"

BOOST_AUTO_TEST_CASE(testCEAComplete)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    spoac::STMPtr stm = manager->getService<spoac::STM>();

    spoac::ice::IceHelperPtr iceHelper =
        manager->getService<spoac::ice::IceHelper>();

    setenv("MCAPROJECTHOME", "../../ltm/test/", 1);

    Ice::ObjectPtr ltmObject = new spoac::LTM;
    iceHelper->registerAdapter(ltmObject, "LTM", "tcp -p 10099");

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

    BOOST_CHECK_EQUAL(cea->getCurrentOAC(), oac);

    cea->run();

    BOOST_CHECK_EQUAL(spoactest::CountPerceptionHandler::counter, 3);
    BOOST_CHECK_EQUAL(spoactest::CountTwoAction::counter, 2);
}

BOOST_AUTO_TEST_CASE(testIceScenario)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    spoac::ice::IceHelperPtr iceHelper(new spoac::ice::IceHelper);

    spoac::CEAPtr cea(new spoac::CEA(
        manager->getService<spoac::STM>(),
        manager
    ));

    setenv("MCAPROJECTHOME", "../../ltm/test/", 1);

    // set up in previous test - clean up?
    //Ice::ObjectPtr ltmObject = new spoac::LTM;
    //iceHelper->registerAdapter(ltmObject, "LTM", "tcp -p 10099");

    cea->setScenario("abc");
}

BOOST_AUTO_TEST_CASE(testCEAsuperOACs)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    spoac::STMPtr stm = manager->getService<spoac::STM>();

    spoac::ice::IceHelperPtr iceHelper =
        manager->getService<spoac::ice::IceHelper>();

    //setenv("MCAPROJECTHOME", "../../ltm/test/", 1);

    //Ice::ObjectPtr ltmObject = new spoac::LTM;
    //iceHelper->registerAdapter(ltmObject, "LTM", "tcp -p 10099");

    spoac::CEAPtr cea = manager->getService<spoac::CEA>();

    spoac::OACPtr superOAC(
        new spoac::OAC("CountTwoSuper", std::vector<std::string>()));

    spoactest::CountTwoAction::counter = 0;

    cea->startOAC(spoac::ActivityControllerPtr(), superOAC);

    BOOST_CHECK_EQUAL(spoactest::CountTwoAction::counter, 0);

    cea->run(); // first action
    cea->run(); // super OAC yields to CountTwo
    cea->run(); // 0
    cea->run(); // +1
    cea->run(); // +2

    BOOST_CHECK_EQUAL(spoactest::CountTwoAction::counter, 2);

    cea->run(); // finish first sub action
    cea->run(); // 0

    BOOST_CHECK_EQUAL(spoactest::CountTwoAction::counter, 0);

    cea->run(); // +1
    cea->run(); // +2
    cea->run(); // finish second sub action
    cea->run(); // finish super OAC

    BOOST_CHECK_EQUAL(cea->getCurrentOAC(), superOAC);
    BOOST_CHECK_EQUAL(spoactest::CountTwoAction::counter, 2);
}
