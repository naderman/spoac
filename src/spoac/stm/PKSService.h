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

#ifndef SPOAC_STM_PKSSERVICE_H
#define SPOAC_STM_PKSSERVICE_H

#include <spoac/common/DependencyManager.h>
#include <spoac/stm/STM.h>
#include <spoac/ice/IceHelper.h>
#include <spoac/LTM.h>

#include <IceUtil/Mutex.h>

namespace spoac
{
    /**
    * Acts as an in-between layer to talk to PKS.
    */
    class PKSService
    {
    public:
        /**
        * Creates an instance of this class with the right dependencies.
        *
        * @param manager The DependencyManager providing necessary dependencies.
        */
        static boost::shared_ptr<void> createService(
            DependencyManagerPtr manager);

        /**
        * Constructor storing dependencies
        *
        * @param stm                   A pointer to the robot's short term
        *                              memory
        */
        PKSService(STMPtr stm, ice::IceHelperPtr iceHelper);

        void setScenario(const LTMSlice::Scenario& scenario);
        void setGoal(const PlanningSlice::Goal& goal);

        void sendScenario();

        void checkScenario();

    protected:
        STMPtr stm;
        ice::IceHelperPtr iceHelper;

        size_t stmSize;

        PlanningSlice::PlanControllerTopicPrx planner;
        LTMSlice::Scenario currentScenario;
        PlanningSlice::Goal currentGoal;

        //IceUtil::Mutex mutex;

        typedef DependencyManager::RegisterService<PKSService> RegisterService;
        static RegisterService r;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<PKSService> PKSServicePtr;
}

#endif

