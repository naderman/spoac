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

namespace spoactest
{
    class DummyCEA : public spoac::CEAInterface
    {
        virtual void startOAC
            (spoac::ActivityControllerPtr src, spoac::OACPtr oac) {}
        virtual void stopOAC
            (spoac::ActivityControllerPtr src, spoac::OACPtr oac) {}
        virtual void taskCompleted(spoac::ActivityControllerPtr src) {}
        virtual void pause(spoac::ActivityControllerPtr src) {}
        virtual void reset(spoac::ActivityControllerPtr src) {}
    };

    class DummyActivityController : public spoac::ActivityController
    {
    public:
        DummyActivityController(spoac::CEAWeakPtr cea) :
            spoac::ActivityController(cea)
        {
        }

        void start(spoac::OACPtr oac)
        {
            if (spoac::CEAPtr cea = weakCEA.lock())
            {
                cea->startOAC(shared_from_this(), oac);
            }
        }

        void stop(spoac::OACPtr oac)
        {
            if (spoac::CEAPtr cea = weakCEA.lock())
            {
                cea->stopOAC(shared_from_this(), oac);
            }
        }

        virtual void oacStarted(spoac::OACPtr oac) {}
        virtual void oacFinished(spoac::OACPtr oac) {}
        virtual void oacStopped(spoac::OACPtr oac) {}
        virtual void pause() {}
        virtual void reset() {}
    };
}

BOOST_AUTO_TEST_CASE(testDummyActivityController)
{
    spoac::CEAPtr cea(new spoactest::DummyCEA());
    spoac::OACPtr oac(new spoac::OAC("no-action", std::vector<std::string>()));

    boost::shared_ptr<spoactest::DummyActivityController> activityController(
        new spoactest::DummyActivityController(spoac::CEAWeakPtr(cea))
    );

    activityController->start(oac);
    activityController->oacStarted(oac);
    activityController->stop(oac);
    activityController->oacStopped(oac);

    activityController->start(oac);
    activityController->oacStarted(oac);
    activityController->oacFinished(oac);

    // important so the CEA is not deleted, we still need it for the weak ptr.
    cea.reset();
}
