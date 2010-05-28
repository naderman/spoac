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

#ifndef SPOAC_STM_STM_H
#define SPOAC_STM_STM_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <spoac/common/DependencyManager.h>
#include <spoac/stm/ObjectSet.h>
#include <spoac/stm/PerceptionHandler.h>

namespace spoac
{
    /**
    * Representation of the robot's short term memory.
    */
    class STM : public ObjectSet, public boost::enable_shared_from_this<STM>
    {
    public:
        /**
        * Adds a perception handler to the ones called by update().
        *
        * @param handler Pointer to a PerceptionHandler implementation.
        */
        void addPerceptionHandler(PerceptionHandlerPtr handler);

        /**
        * Adds a perception handler identified by its name.
        *
        * @param handlerName Name of a PerceptionHandler implementation
        */
        void addPerceptionHandler(
            std::string handlerName,
            DependencyManagerPtr dependencyManager
        );

        /**
        * Removes all registered perception handlers
        */
        void clearPerceptionHandlers();

        /**
        * Clears the handler list and then adds all the given ones by name.
        *
        * @param handlerNames A vector of PerceptionHandler implementation names
        */
        void setPerceptionHandlers(
            std::vector<std::string> handlerNames,
            DependencyManagerPtr dependencyManager
        );

        /**
        * Processes sensor input and updates object information.
        */
        void update();

    protected:
        std::vector<PerceptionHandlerPtr> handlers;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<STM> STMPtr;
}

#endif

