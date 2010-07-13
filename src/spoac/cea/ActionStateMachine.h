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
        * @param config  A JSON configuration object retrieved from LTM.
        */
        virtual void setup(const ObjectVector& objects, JSON::ValuePtr config);

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
        JSON::ValuePtr config;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ActionStateMachine> ActionStateMachinePtr;
}

/**
* This macro can be used to generate an action state machine that starts with
* the given state. E.g.:
*
* SPOAC_ACTION_STATE_MACHINE(
*     MyStartState,
*     new MyStartState(manager->getService<SomeService>())
* );
*/
#define SPOAC_ACTION_STATE_MACHINE(StartState, newStartState) \
    class StartState ## StateMachine : public spoac::ActionStateMachine { \
    public: \
        static std::string getName() { return #StartState; } \
        static spoac::ActionPtr createInstance( \
            spoac::DependencyManagerPtr manager) { \
            spoac::ActionStatePtr startState(newStartState); \
            spoac::ActionPtr action(new StartState ## StateMachine(startState)); \
            return action; \
        } \
        StartState ## StateMachine(spoac::ActionStatePtr startState) : \
            ActionStateMachine(startState){} \
    protected: \
        static Register<StartState ## StateMachine> r; \
    };

#endif

