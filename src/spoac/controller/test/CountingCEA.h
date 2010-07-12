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

#include <spoac/cea/CEA.h>

namespace spoactest
{
    class CountingCEA : public spoac::CEAControl
    {
    public:
        virtual void startOAC
            (spoac::ActivityControllerPtr src, spoac::OACPtr oac)
        {
            requestCount++;
        }
        virtual void stopOAC
            (spoac::ActivityControllerPtr src, spoac::OACPtr oac) {}
        virtual void taskCompleted(spoac::ActivityControllerPtr src) {}
        virtual void pause(spoac::ActivityControllerPtr src) {}
        virtual void unpause(spoac::ActivityControllerPtr src) {}
        virtual void reset(spoac::ActivityControllerPtr src) {}
        virtual spoac::OACPtr getCurrentOAC() {return spoac::OACPtr();}
        virtual void setScenario(const std::string& scenario) {}
        virtual void setGoalExpression(const std::string& goalExpression) {}
        virtual void setGoalName(const std::string& goalName) {}

        static int requestCount;
    };
    int CountingCEA::requestCount;
}
