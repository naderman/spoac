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

#include <spoac/cea/ActionStateMachine.h>

using namespace spoac;

ActionStateMachine::ActionStateMachine(ActionStatePtr startState) :
    currentState(startState)
{
}

void ActionStateMachine::setup(
    const ObjectVector& objects,
    JSON::ValuePtr config)
{
    this->objects = objects;
    this->config = config;
    // setup start state
    currentState->setup(objects, config);
}

void ActionStateMachine::run()
{
    ActionStatePtr nextState = currentState->run();

    if (nextState != currentState)
    {
        nextState->setup(objects, config);

        currentState = nextState;
    }
}

bool ActionStateMachine::isFinished() const
{
    return currentState->isFinal();
}
