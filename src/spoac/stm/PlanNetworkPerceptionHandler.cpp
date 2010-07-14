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

#include <spoac/stm/STM.h>
#include <spoac/stm/PlanNetworkPerceptionHandler.h>

using namespace spoac;

PlanNetworkPerceptionHandler::Register<PlanNetworkPerceptionHandler>
    PlanNetworkPerceptionHandler::r;

PerceptionHandlerPtr PlanNetworkPerceptionHandler::createInstance(
    DependencyManagerPtr m)
{
    PerceptionHandlerPtr handler(new PlanNetworkPerceptionHandler(
        m->getService<ice::IceHelper>(),
        m->getService<PKSService>()
    ));

    return handler;
}

PlanNetworkPerceptionHandler::PlanNetworkPerceptionHandler(
    ice::IceHelperPtr iceHelper, PKSServicePtr pksService) :
    iceHelper(iceHelper),
    pksService(pksService),
    wait(5)
{
    if (iceHelper.get() != NULL)
    {
        planner = iceHelper->stormGetTopic<PlanningSlice::PlanControllerTopicPrx>(
            "PlanController"
        );
    }
}

void PlanNetworkPerceptionHandler::setScenario(
    const spoac::LTMSlice::Scenario& scenario)
{
    wait = 5;

    predicates = scenario.predicates;
    functions = scenario.functions;
}

void PlanNetworkPerceptionHandler::update(spoac::STMPtr stm)
{
    if (wait <= 0)
    {
        pksService->checkScenario();

        planner->updateState(getState(stm));
    }
    else
    {
        wait--;
    }
}


PlanningSlice::StateUpdate PlanNetworkPerceptionHandler::getState(
    spoac::STMPtr stm)
{
    PlanningSlice::PredicateDefinitionList::const_iterator pred;
    PlanningSlice::FunctionDefinitionList::const_iterator func;
    STM::iterator_map obj;

    PlanningSlice::StateUpdate state;

    for (pred = predicates.begin(); pred != predicates.end(); ++pred)
    {
        for (obj = stm->beginMap(); obj != stm->endMap(); ++obj)
        {
            ObjectPtr object = *(obj->second);

            if (object->has_key(pred->name))
            {
                PlanningSlice::PredicateInstance p;
                p.name = pred->name;

                if (pred->arguments == 1)
                {
                    p.parameters.push_back(object->getId());
                    p.value = object->get<bool>(pred->name);
                    state.knownPredicates.push_back(p);
                }
                else if (pred->arguments == 2)
                {
                    std::pair<bool, std::string> relation =
                        object->get<std::pair<bool, std::string> >(pred->name);

                    p.parameters.push_back(object->getId());
                    p.parameters.push_back(relation.second);
                    p.value = relation.first;

                    state.knownPredicates.push_back(p);
                }
            }
        }
    }

    for (func = functions.begin(); func != functions.end(); ++func)
    {
        for (obj = stm->beginMap(); obj != stm->endMap(); ++obj)
        {
            ObjectPtr object = *(obj->second);

            if (object->has_key(func->name))
            {
                PlanningSlice::FunctionValue f;
                f.name = func->name;

                if (func->arguments == 0)
                {
                    f.constantValue = object->get<std::string>(func->name);
                    f.realValue = object->get<double>(func->name);
                    state.knownFunctionValues.push_back(f);
                }
            }
        }
    }

    return state;
}
