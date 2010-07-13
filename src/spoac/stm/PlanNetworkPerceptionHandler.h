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

#ifndef SPOAC_STM_PLANNETWORKPERCEPTIONHANDLER
#define SPOAC_STM_PLANNETWORKPERCEPTIONHANDLER

#include <spoac/stm/PerceptionHandler.h>
#include <spoac/Planning.h>
#include <spoac/ice/IceHelper.h>

namespace spoac
{
    class PlanNetworkPerceptionHandler : public PerceptionHandler
    {
    public:
        static std::string getName()
        {
            return "PlanNetworkPerceptionHandler";
        }

        static PerceptionHandlerPtr createInstance(DependencyManagerPtr m);

        PlanNetworkPerceptionHandler(ice::IceHelperPtr iceHelper);

        void update(spoac::STMPtr stm);

        void setScenario(const LTMSlice::Scenario& scenario);

    protected:
        PlanningSlice::PredicateDefinitionList predicates;
        PlanningSlice::FunctionDefinitionList functions;

        ice::IceHelperPtr iceHelper;
        PlanningSlice::PlanControllerTopicPrx planner;

        static Register<PlanNetworkPerceptionHandler> r;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<PlanNetworkPerceptionHandler>
        PlanNetworkPerceptionHandlerPtr;
}

#endif
