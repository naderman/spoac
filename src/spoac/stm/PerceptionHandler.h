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

#ifndef SPOAC_STM_PERCEPTIONHANDLER_H
#define SPOAC_STM_PERCEPTIONHANDLER_H

#include <boost/shared_ptr.hpp>
#include <spoac/common/AbstractFactoryMethod.h>
#include <spoac/LTM.h>

namespace spoac
{
    class STM;
    typedef boost::shared_ptr<STM> STMPtr;
    class DependencyManager;
    typedef boost::shared_ptr<DependencyManager> DependencyManagerPtr;

    /**
    * Representation of the robot's short term memory.
    */
    class PerceptionHandler :
        public AbstractFactoryMethod<PerceptionHandler, DependencyManagerPtr>
    {
    public:
        /**
        * Processes sensor input and updates object information.
        */
        virtual void update(STMPtr stm) = 0;

        /**
        * Let the handler know which scenario is being run.
        *
        * @param scenario The scenario definition.
        */
        virtual void setScenario(const LTMSlice::Scenario& scenario) = 0;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<PerceptionHandler> PerceptionHandlerPtr;
}

#endif

