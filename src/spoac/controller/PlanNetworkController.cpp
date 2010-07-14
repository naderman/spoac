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
        manager->getService<IceHelper>(),
        manager->getService<STM>()
    ));

    return controller;
}

PlanNetworkController::PlanNetworkController(
    CEAControlWeakPtr cea, IceHelperPtr iceHelper, STMPtr stm) :
    ActivityController(cea),
    sentScenario(false),
    stm(stm),
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
    planner->actionFinished(oac->getIceAction());
}

void PlanNetworkController::oacStopped(OACPtr oac)
{
    reset();
}

void PlanNetworkController::requestAction()
{
    if (!sentScenario)
    {
        sendScenario();
    }

    planner->startPlan(); // only requests a new action
}

void PlanNetworkController::pause()
{
    planner->stopPlan();
}

void PlanNetworkController::unpause()
{
    planner->startPlan();
}

void PlanNetworkController::reset()
{
    planner->stopPlan();
    planner->resetState();

    sentScenario = false;
}

void PlanNetworkController::setScenario(const LTMSlice::Scenario& scenario)
{
    currentScenario = scenario;
}

void PlanNetworkController::setGoalExpression(const std::string& goalExpression)
{
    Goal g;
    g.goalExpression = goalExpression;

    currentGoal = g;

    if (sentScenario)
    {
        planner->setGoal(currentGoal);
    }
}

void PlanNetworkController::sendScenario()
{
    LTMSlice::LTMPrx ltm =
        iceHelper->getProxy<LTMSlice::LTMPrx>("LTM:tcp -p 10099");

    SymbolDefinition symbols;
    symbols.predicates = currentScenario.predicates;
    symbols.functions = currentScenario.functions;
    symbols.constants = stm->extractPlanConstants();

    // try again if no objects were known
    if (stm->sizeNonHardcoded() > 0)
    {
        sentScenario = true;
    }

    planner->setSymbolDefinitions(symbols);

    ActionDefinitionList actions;

    LTMSlice::NameList::const_iterator it;

    for (it = currentScenario.oacs.begin();
        it != currentScenario.oacs.end();
        ++it)
    {
        PlanningSlice::ActionDefinition action = ltm->getAction(*it);

        if (!action.effect.empty() && !action.precondition.empty())
        {
            actions.push_back(action);
        }
    }

    planner->setActionDefinitions(actions);

    if (!currentGoal.goalExpression.empty())
    {
        planner->setGoal(currentGoal);
    }
}
