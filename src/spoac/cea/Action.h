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

#ifndef SPOAC_CEA_ACTION_H
#define SPOAC_CEA_ACTION_H

#include <boost/shared_ptr.hpp>

#include <spoac/common/AbstractFactoryMethod.h>
#include <spoac/common/DependencyManager.h>
#include <spoac/stm/ObjectVector.h>

namespace spoac
{
    /**
    * Abstract base class for all actions the CEA should be able to execute.
    */
    class Action : public AbstractFactoryMethod<Action, DependencyManagerPtr>
    {
    public:
        /**
        * Static method returning the action's name
        */
        static std::string getName();

        /**
        * Create an instance of this class, satisfying all its dependencies
        */
        static boost::shared_ptr<Action> createInstance(
            DependencyManagerPtr manager
        );

        /**
        * Prepares an action for execution with a vector of parameter objects.
        *
        * @param objects The objects this action should be applied on.
        */
        virtual void setup(const ObjectVector& objects) = 0;

        /**
        * Executes the action on the robot.
        *
        * This function is called in a loop in should return quickly.
        * @warning The robot needs to be initialised before this method gets
        *          called.
        */
        virtual void run() = 0;

        /**
        * Checks whether execution of the action has been completed.
        *
        * @return True if the action has been completed, false otherwise.
        */
        virtual bool isFinished() const = 0;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<Action> ActionPtr;
}

#endif

