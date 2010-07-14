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

#define BOOST_TEST_MODULE spoac_ObjectVector
#include <spoactest/test.h>

#include <iostream>
#include <spoac/stm/PlanNetworkPerceptionHandler.h>
#include <spoac/stm/STM.h>

BOOST_AUTO_TEST_CASE(testEmpty)
{
    spoac::STMPtr stm(new spoac::STM());
    spoac::PKSServicePtr pks(new spoac::PKSService(
        stm, spoac::ice::IceHelperPtr()));
    spoac::PlanNetworkPerceptionHandlerPtr controller(
        new spoac::PlanNetworkPerceptionHandler(
            spoac::ice::IceHelperPtr(),
            pks));

    spoac::ObjectPtr obj(new spoac::Object("obj1", "obj1"));
    (*obj)["pred1"] = "foo";
    (*obj)["func1"] = "foo";

    stm->insert(obj);

    spoac::PlanningSlice::PredicateDefinition pred1;
    pred1.name = "pred1";
    pred1.arguments = 1;

    spoac::PlanningSlice::PredicateDefinition pred2;
    pred2.name = "pred2";
    pred2.arguments = 2;

    spoac::PlanningSlice::FunctionDefinition func1;
    func1.name = "func1";;
    func1.arguments = 0;

    spoac::LTMSlice::Scenario scenario;
    scenario.predicates.push_back(pred1);
    scenario.functions.push_back(func1);

    controller->setScenario(scenario);

    spoac::PlanningSlice::StateUpdate state = controller->getState(stm);

    BOOST_CHECK_EQUAL(state.knownPredicates.size(), 1);
    BOOST_CHECK_EQUAL(state.knownFunctionValues.size(), 1);
}
