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

#define BOOST_TEST_MODULE spoac_Action
#include <spoactest/test.h>

#include <iostream>
#include <spoac/cea/Action.h>

namespace spoactest
{
    class CountTwoAction : public spoac::Action
    {
    public:
        int counter;

        virtual void setup(const spoac::ObjectVector& objects)
        {
            counter = 0;
        }

        virtual void run()
        {
            counter++;
        }

        virtual bool isFinished() const
        {
            return counter >= 2;
        }
    };

    class EmptyAction : public spoac::Action
    {
    public:
        virtual void setup(const spoac::ObjectVector& objects)
        {
        }

        virtual void run()
        {
        }

        virtual bool isFinished() const
        {
            return true;
        }
    };
}

BOOST_AUTO_TEST_CASE(testCountTwoAction)
{
    spoactest::CountTwoAction action;

    action.setup(spoac::ObjectVector());

    while (!action.isFinished())
    {
        action.run();
    }

    BOOST_CHECK_EQUAL(action.counter, 2);
}

