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

#define BOOST_TEST_MODULE spoac_ActivityController
#include <spoactest/test.h>

#include <iostream>
#include <spoac/cea/ActivityController.h>
#include "DummyActivityController.h"

namespace spoactest
{
    class DummyCEA : public spoac::CEAControl
    {
        virtual void startOAC
            (spoac::ActivityControllerPtr src, spoac::OACPtr oac) {}
        virtual void stopOAC
            (spoac::ActivityControllerPtr src, spoac::OACPtr oac) {}
        virtual void taskCompleted(spoac::ActivityControllerPtr src) {}
        virtual void pause(spoac::ActivityControllerPtr src) {}
        virtual void unpause(spoac::ActivityControllerPtr src) {}
        virtual void reset(spoac::ActivityControllerPtr src) {}
    };
}

BOOST_AUTO_TEST_CASE(testDummyActivityController)
{
    spoac::CEAControlPtr cea(new spoactest::DummyCEA());
    spoac::OACPtr oac(new spoac::OAC("no-action", std::vector<std::string>()));

    boost::shared_ptr<spoactest::DummyActivityController> activityController(
        new spoactest::DummyActivityController(
            spoac::CEAControlWeakPtr(cea), oac
        )
    );

    activityController->start();
    activityController->oacStarted(oac);
    activityController->stop();
    activityController->oacStopped(oac);

    activityController->start();
    activityController->oacStarted(oac);
    activityController->oacFinished(oac);

    // important so the CEA is not deleted, we still need it for the weak ptr.
    cea.reset();
}
