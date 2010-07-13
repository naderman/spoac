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
#include <spoac/stm/ObjectVector.h>
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
        * Creates an instance of this class with dependencies.
        *
        * @param manager The DependencyManager used to satisfy requirements
        */
        static boost::shared_ptr<void> createService(
            DependencyManagerPtr manager);

        /**
        * Adds a perception handler to the ones called by update().
        *
        * @param handler Pointer to a PerceptionHandler implementation.
        */
        void addPerceptionHandler(PerceptionHandlerPtr handler);

        /**
        * Adds a perception handler identified by its name.
        *
        * @param handlerName       Name of a PerceptionHandler implementation
        * @param dependencyManager Required to satisfy new class's dependencies
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
        * @param dependencyManager Required to satisfy new classes' dependencies
        */
        void setPerceptionHandlers(
            std::vector<std::string> handlerNames,
            DependencyManagerPtr dependencyManager
        );

        /**
        * Processes sensor input and updates object information.
        */
        void update();

        /**
        * Resolve a vector of object ids to an ObjectVector
        */
        ObjectVector vectorFromIds(const std::vector<std::string>& ids);

        /**
        * Returns the amount of objects which do not have the __hardcoded flag
        * set to true.
        *
        * @return The result of counting the objects
        */
        size_type sizeNonHardcoded() const;

        /**
        * Informs the STM which scenario is being run.
        *
        * @param scenario          The scenario definition
        * @param dependencyManager DependencyManager for perception handler
        *                          instantiation dependencies.
        */
        void setScenario(
            const LTMSlice::Scenario& scenario,
            DependencyManagerPtr dependencyManager);

        /**
        * Retrieve a list of object ids for planning - low level items are
        * excluded, as defined by the scenario
        *
        * @return A vector of constant names / object ids.
        */
        std::vector<std::string> extractPlanConstants();

    protected:
        std::vector<PerceptionHandlerPtr> handlers;

        typedef DependencyManager::RegisterService<STM> RegisterService;
        static RegisterService r;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<STM> STMPtr;
}

#endif

