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

#ifndef SPOAC_CEA_ACTIVITYCONTROLLER_H
#define SPOAC_CEA_ACTIVITYCONTROLLER_H

#include <spoac/cea/CEAControl.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace spoac
{
    /**
    * Abstract base class for ActivityControllers that tell the CEA which OACs
    * need to be executed.
    *
    * When sending information to the CEA instances need to pass a this pointer
    * to identify themselves. Since ActivityControllers are held in shared
    * pointers, the this pointer has to be one, too.
    */
    class ActivityController :
        public AbstractFactoryMethod<ActivityController, DependencyManagerPtr>,
        public boost::enable_shared_from_this<ActivityController>
    {
    public:
        /**
        * The base constructor taking only a weak pointer to the CEA.
        *
        * @param cea A weak_ptr to the Central Executive Agent. Must be a weak
        *            pointer to avoid circular references, because the CEA
        *            manages all ActivityControllers.
        */
        ActivityController(CEAControlWeakPtr cea);

        /**
        * Informs the ActivityController that the CEA has started an OAC.
        *
        * @param oac The OAC which has started execution on the robot.
        */
        virtual void oacStarted(OACPtr oac) = 0;

        /**
        * Informs the ActivityController about a finished OAC.
        *
        * @param oac The OAC which has finished execution on the robot.
        */
        virtual void oacFinished(OACPtr oac) = 0;

        /**
        * Informs the ActivityController about an OAC that has been stopped
        * before execution was finished.
        *
        * @param oac The OAC which has been stopped.
        */
        virtual void oacStopped(OACPtr oac) = 0;

        /**
        * The robot does not know what to do, so it is requesting an action.
        */
        virtual void requestAction() = 0;

        /**
        * The robot is being paused.
        * All processing and communication should be paused.
        */
        virtual void pause() = 0;

        /**
        * The robot is being unpaused.
        * Processing and communication can continue
        */
        virtual void unpause() = 0;

        /**
        * The robot is being reset.
        * All processing and communication has to be stopped and reinitialized.
        */
        virtual void reset() = 0;

        /**
        * A goal has been set as a PKS expression.
        *
        * @param goalExpression A PKS expression that should evaluate to true.
        */
        virtual void setGoalExpression(const std::string& goalExpression) = 0;

    protected:
        CEAControlWeakPtr weakCEA;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ActivityController> ActivityControllerPtr;
}

#endif

