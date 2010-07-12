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

#define BOOST_TEST_MODULE spoac_IceNetworkController
#include <spoactest/test.h>

#include <iostream>
#include <spoac/cea/CEA.h>
#include <spoac/controller/PlanNetworkController.h>
#include <spoac/ltm/LTM.h>
#include "../../cea/test/CountTwoAction.h"
#include "CountingCEA.h"

using namespace spoac::PlanningSlice;
using namespace spoac::SymbolicExecutionSlice;

namespace spoactest
{
    class CheckingPlanner : public PlanControllerTopic
    {
    public:
        void setSymbolDefinitions(
            const SymbolDefinition& symbolDefinition,
            const Ice::Current& c)
        {
        }

        static bool setActionDefinitionsCalled;
        void setActionDefinitions(
            const ActionDefinitionList& actionList,
            const Ice::Current& c)
        {
            setActionDefinitionsCalled = true;

            BOOST_CHECK_EQUAL(1, actionList.size());
        }

        void setGoal(const Goal& goal, const Ice::Current& c)
        {
        }

        void updateState(const StateUpdate& stateUpdate, const Ice::Current& c)
        {
        }

        void actionFinished(const Action& action, const Ice::Current& c)
        {
        }

        void startPlan(const Ice::Current&)
        {
        }

        void stopPlan(const Ice::Current&)
        {
        }

        void resetState(const Ice::Current&)
        {
        }
    };
    bool CheckingPlanner::setActionDefinitionsCalled;
}

using spoac::controller::PlanNetworkController;
using spoac::controller::PlanNetworkControllerPtr;

BOOST_AUTO_TEST_CASE(testEmpty)
{
    boost::shared_ptr<spoactest::CountingCEA> cea(new spoactest::CountingCEA);
    spoac::ice::IceHelperPtr iceHelper(new spoac::ice::IceHelper);

    PlanNetworkControllerPtr controller(
        new PlanNetworkController(
            spoac::CEAControlWeakPtr(cea),
            iceHelper
        )
    );

    Ice::ObjectPtr planner = new spoactest::CheckingPlanner;
    iceHelper->stormSubscribeTopic(planner, "PlanController", "tcp -p 10005");

    setenv("MCAPROJECTHOME", "../../ltm/test/", 1);
    Ice::ObjectPtr ltmObject = new spoac::LTM;
    iceHelper->registerAdapter(ltmObject, "LTM", "tcp -p 10099");

    spoac::LTMSlice::Scenario scenario;
    scenario.name = "testScenario";
    scenario.oacs.push_back("SampleOAC");

    spoactest::CheckingPlanner::setActionDefinitionsCalled = false;
    controller->setScenario(scenario);

    IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(100));
    BOOST_CHECK(spoactest::CheckingPlanner::setActionDefinitionsCalled);
}
