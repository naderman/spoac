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

#include <spoac/stm/PKSService.h>

using namespace spoac;
using namespace spoac::PlanningSlice;

PKSService::RegisterService PKSService::r;

boost::shared_ptr<void> PKSService::createService(DependencyManagerPtr manager)
{
    boost::shared_ptr<void> pksService(new PKSService(
        manager->getService<STM>(),
        manager->getService<ice::IceHelper>()
    ));
    return pksService;
}

PKSService::PKSService(STMPtr stm, ice::IceHelperPtr iceHelper):
    stm(stm),
    iceHelper(iceHelper)
{
    if (iceHelper.get() != NULL)
    {
        planner = iceHelper->stormGetTopic<PlanControllerTopicPrx>(
            "PlanController");
    }
    stmSize = -1;
    sentGoal = false;
}

void PKSService::setScenario(const LTMSlice::Scenario& scenario)
{
    currentScenario = scenario;
}

void PKSService::setGoal(const PlanningSlice::Goal& goal)
{
    currentGoal = goal;
}

void PKSService::sendScenario()
{
    LTMSlice::LTMPrx ltm =
    iceHelper->getProxy<LTMSlice::LTMPrx>("LTM:tcp -p 10099");

    SymbolDefinition symbols;
    symbols.predicates = currentScenario.predicates;
    symbols.functions = currentScenario.functions;
    symbols.constants = stm->extractPlanConstants();
    stmSize = symbols.constants.size();

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

    sentGoal = false;
}

void PKSService::sendGoal()
{
    if (!currentGoal.goalExpression.empty())
    {
        planner->setGoal(currentGoal);
    }
    sentGoal = true;
}

void PKSService::updateState(const StateUpdate& state)
{
    if (stm->extractPlanConstants().size() != stmSize)
    {
        sendScenario();
    }

    planner->updateState(state);

    if (!sentGoal)
    {
        sendGoal();
    }
}

