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

#ifndef SPOAC_CEA_CEAINTERFACE_H
#define SPOAC_CEA_CEAINTERFACE_H

#include <spoac/cea/OAC.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace spoac
{
    class ActivityController;
    typedef boost::shared_ptr<ActivityController> ActivityControllerPtr;

    /**
    * Interface definition for the Central Executive Agent.
    */
    class CEAInterface
    {
    public:
        /**
        * Starts a particular OAC's execution.
        *
        * @param src The instructing ActivityController.
        * @param oac The OAC which shall be started
        */
        virtual void startOAC(ActivityControllerPtr src, OACPtr oac) = 0;

        /**
        * Stops a particular OAC's execution.
        *
        * @param src The instructing ActivityController.
        * @param oac The OAC which needs to be stopped.
        */
        virtual void stopOAC(ActivityControllerPtr src, OACPtr oac) = 0;

        /**
        * Informs the CEA about the completion of a task.
        *
        * @param src The ActivityController which completed its task.
        */
        virtual void taskCompleted(ActivityControllerPtr src) = 0;

        /**
        * Asks the CEA to pause the robot immediately.
        *
        * @param src The ActivityController which made the request.
        */
        virtual void pause(ActivityControllerPtr src) = 0;

        /**
        * Asks the CEA to reset the robot immediately.
        *
        * @param src The ActivityController which made the request.
        */
        virtual void reset(ActivityControllerPtr src) = 0;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<CEAInterface> CEAPtr;

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::weak_ptr<CEAInterface> CEAWeakPtr;
}

#endif
