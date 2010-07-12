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

#include <spoac/controller/PlanNetworkController.h>
#include <spoac/cea/CEA.h>

using namespace spoac;
using namespace spoac::controller;
using namespace spoac::ice;
using namespace spoac::PlanningSlice;

PlanNetworkController::Register<PlanNetworkController> PlanNetworkController::r;

ActivityControllerPtr PlanNetworkController::createInstance(
    DependencyManagerPtr manager)
{
    ActivityControllerPtr controller(new PlanNetworkController(
        CEAControlWeakPtr(manager->getService<CEA>()),
        manager->getService<IceHelper>()
    ));

    return controller;
}

PlanNetworkController::PlanNetworkController(
    CEAControlWeakPtr cea, IceHelperPtr iceHelper) :
    ActivityController(cea),
    iceHelper(iceHelper)
{
    //iceHelper->stormSubscribeTopic(this, "CEAController", "tcp -p 10001");
    //iceHelper->stormGetTopic<PlanControllerTopicPrx>("PlanController", "tcp -p 10005");

    planner = iceHelper->stormGetTopic<PlanControllerTopicPrx>(
        "PlanController");
}

void PlanNetworkController::oacStarted(OACPtr oac)
{
}

void PlanNetworkController::oacFinished(OACPtr oac)
{
}

void PlanNetworkController::oacStopped(OACPtr oac)
{
}

void PlanNetworkController::requestAction()
{
}

void PlanNetworkController::pause()
{
}

void PlanNetworkController::unpause()
{
}

void PlanNetworkController::reset()
{
}

void PlanNetworkController::setScenario(const LTMSlice::Scenario& scenario)
{
    LTMSlice::LTMPrx ltm =
        iceHelper->getProxy<LTMSlice::LTMPrx>("LTM:tcp -p 10099");

    ActionDefinitionList actions;

    LTMSlice::NameList::const_iterator it;

    for (it = scenario.oacs.begin(); it != scenario.oacs.end(); ++it)
    {
        PlanningSlice::ActionDefinition action = ltm->getAction(*it);

        if (!action.effect.empty() && !action.precondition.empty())
        {
            actions.push_back(action);
        }
    }

    planner->setActionDefinitions(actions);
}

void PlanNetworkController::setGoalExpression(const std::string& goalExpression)
{
}
