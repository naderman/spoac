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

#include <spoac/controller/IceNetworkController.h>
#include <spoac/cea/CEA.h>

using namespace spoac;
using namespace spoac::controller;
using namespace spoac::ice;

IceNetworkController::Register<IceNetworkController> IceNetworkController::r;

ActivityControllerPtr IceNetworkController::createInstance(
    DependencyManagerPtr manager)
{
    ActivityControllerPtr controller(new IceNetworkController(
        CEAControlWeakPtr(manager->getService<CEA>()),
        manager->getService<IceHelper>()
    ));

    return controller;
}

IceNetworkController::IceNetworkController(
    CEAControlWeakPtr cea, IceHelperPtr iceHelper) :
    ActivityController(cea),
    iceHelper(iceHelper)
{
    iceHelper->stormSubscribeTopic(this, "CEAController", "tcp -p 10001");
}

void IceNetworkController::oacStarted(OACPtr oac)
{
}

void IceNetworkController::oacFinished(OACPtr oac)
{
}

void IceNetworkController::oacStopped(OACPtr oac)
{
}

void IceNetworkController::requestAction()
{
}

void IceNetworkController::pause()
{
}

void IceNetworkController::unpause()
{
}

void IceNetworkController::reset()
{
}

void IceNetworkController::setScenario(const LTMSlice::Scenario& scenario)
{
}

void IceNetworkController::setGoalExpression(const std::string& goalExpression)
{
}

void IceNetworkController::startAction(
    const SymbolicExecutionSlice::Action& action,
    const Ice::Current& c)
{
    OACPtr oac = actionToOAC(action);

    if (CEAControlPtr cea = weakCEA.lock())
    {
        cea->startOAC(shared_from_this(), oac);
    }
}

void IceNetworkController::stopAction(
    const SymbolicExecutionSlice::Action& action,
    const Ice::Current& c)
{
    OACPtr oac = actionToOAC(action);

    if (CEAControlPtr cea = weakCEA.lock())
    {
        cea->stopOAC(shared_from_this(), oac);
    }
}

void IceNetworkController::taskComplete(const Ice::Current& c)
{
    if (CEAControlPtr cea = weakCEA.lock())
    {
        cea->taskCompleted(shared_from_this());
    }
}

void IceNetworkController::pause(const Ice::Current& c)
{
    if (CEAControlPtr cea = weakCEA.lock())
    {
        cea->pause(shared_from_this());
    }
}

void IceNetworkController::unpause(const Ice::Current& c)
{
    if (CEAControlPtr cea = weakCEA.lock())
    {
        cea->unpause(shared_from_this());
    }
}

void IceNetworkController::reset(const Ice::Current& c)
{
    if (CEAControlPtr cea = weakCEA.lock())
    {
        cea->reset(shared_from_this());
    }
}

OACPtr IceNetworkController::actionToOAC(
    const SymbolicExecutionSlice::Action& action)
{
    std::string oacName = action.name;
    std::vector<std::string> oacObjectIds;

    SymbolicExecutionSlice::ObjectList::const_iterator it;

    for (it = action.parameters.begin(); it != action.parameters.end(); ++it)
    {
        oacObjectIds.push_back(it->id);
    }

    OACPtr oac(new OAC(oacName, oacObjectIds));

    return oac;
}
