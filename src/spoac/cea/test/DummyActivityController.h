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

namespace spoactest
{
    class DummyActivityController : public spoac::ActivityController
    {
    public:
        DummyActivityController(
            spoac::CEAControlWeakPtr cea, spoac::OACPtr oac) :
            spoac::ActivityController(cea),
            oac(oac)
        {
        }

        void start()
        {
            if (spoac::CEAControlPtr cea = weakCEA.lock())
            {
                cea->startOAC(shared_from_this(), oac);
            }
        }

        void stop()
        {
            if (spoac::CEAControlPtr cea = weakCEA.lock())
            {
                cea->stopOAC(shared_from_this(), oac);
            }
        }

        virtual void requestAction()
        {
            start();
        }
        virtual void pause() {}
        virtual void unpause() {}
        virtual void reset() {}

    protected:
        spoac::OACPtr oac;
    };
}
