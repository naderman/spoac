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

#ifndef SPOAC_CONTROLLER_SEQUENCECONTROLLER_H
#define SPOAC_CONTROLLER_SEQUENCECONTROLLER_H

#include <spoac/cea/ActivityController.h>

namespace spoac
{
    namespace controller
    {
        /**
        * Provides a CEA with a sequence of OACs passed in on initialisation.
        */
        class SequenceController : public ActivityController
        {
        public:
            /**
            * Constructor which requires CEA access and a vector of OACs.
            *
            * @param cea  Central Executive Agent this controller will run on.
            * @param oacs The sequence of OACs which will be run in order.
            */
            SequenceController(
                CEAControlWeakPtr cea,
                std::vector<OACPtr> oacs
            );

            virtual void oacStarted(OACPtr oac);
            virtual void oacFinished(OACPtr oac);
            virtual void oacStopped(OACPtr oac);

            /**
            * Send the next action to the CEA if any are left or otherwise
            * informs the CEA about task completion.
            */
            virtual void requestAction();
            virtual void pause();
            virtual void unpause();
            virtual void reset();
            virtual void setScenario(const LTMSlice::Scenario& scenario);
            virtual void setGoalExpression(const std::string& goalExpression);

        protected:
            std::vector<OACPtr>::iterator currentOAC;
            std::vector<OACPtr> oacs;
        };

        /**
        * Pointer type to reduce typing for shared pointers.
        */
        typedef boost::shared_ptr<SequenceController> SequenceControllerPtr;
    }
}

#endif
