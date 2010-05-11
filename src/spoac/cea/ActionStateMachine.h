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

#ifndef SPOAC_CEA_ACTIONSTATEMACHINE_H
#define SPOAC_CEA_ACTIONSTATEMACHINE_H

#include <boost/shared_ptr.hpp>

#include <spoac/cea/Action.h>
#include <spoac/cea/ActionState.h>
#include <spoac/cea/ActionStateFinal.h>

namespace spoac
{
    /**
    * An action implementation providing a state machine. This can be used to
    * implement an action by implementing only the individual states, rather
    * than than an entire action class.
    */
    class ActionStateMachine : public Action
    {
    public:
        ActionStateMachine(ActionStatePtr startState);

        /**
        * Sets up the state machine with parameters. These are passed to each
        * new state before it is run.
        *
        * @param objects The objects this action should be applied on.
        */
        virtual void setup(const ObjectVector& objects);

        /**
        * Executes the action on the robot.
        */
        virtual void run();

        /**
        * Checks whether the final state has been reached.
        *
        * @return True if the current state is the final state, else false.
        */
        virtual bool isFinished() const;

    protected:
        ActionStatePtr currentState;
        ObjectVector objects;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ActionStateMachine> ActionStateMachinePtr;
}

#endif

