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

#ifndef SPOAC_CEA_ACTIONSTATE_H
#define SPOAC_CEA_ACTIONSTATE_H

#include <spoac/cea/Object.h>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace spoac
{
    class ActionState;

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ActionState> ActionStatePtr;

    /**
    * Abstract base class for all states the ActionStateMachine can execute.
    */
    class ActionState :
        public boost::enable_shared_from_this<ActionState>
    {
    public:
        /**
        * Prepares the state for execution with a vector of parameter objects.
        *
        * @param objects The objects this action should be applied on.
        */
        virtual void setup(const std::vector<ObjectPtr>& objects) = 0;

        /**
        * Executes this state of the action on the robot.
        *
        * This function is called in a loop in should return quickly.
        * @warning The robot needs to be initialised before this method gets
        *          called.
        *
        * @return  The state to run in the next execution loop. To stay in the
        *          same state it should return shared_from_this().
        */
        virtual ActionStatePtr run() = 0;

        /**
        * Defines whether this is a final state.
        *
        * @return Always false, unless the method is overwritten in a subclass.
        */
        virtual bool isFinal() const;
    };
}

#endif

