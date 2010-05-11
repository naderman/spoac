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

#define BOOST_TEST_MODULE spoac_ActionStateMachine
#include <spoactest/test.h>

#include <iostream>
#include <vector>
#include <spoac/cea/ActionStateMachine.h>
#include <spoac/cea/ActionStateFinal.h>

namespace spoactest
{
    class DummyActionState2 : public spoac::ActionState
    {
    public:
        spoac::ObjectPtr counter;

        virtual void setup(const std::vector<spoac::ObjectPtr>& objects)
        {
            counter = objects[0];
        }

        virtual spoac::ActionStatePtr run()
        {
            double value = counter->get<double>("value");

            if (value < 1.999)
            {
                (*counter)["value"] = value + 0.2;

                // stay in same state
                return shared_from_this();
            }

            spoac::ActionStatePtr final(new spoac::ActionStateFinal);
            return final;
        }
    };

    class DummyActionState1 : public spoac::ActionState
    {
    public:
        spoac::ObjectPtr counter;

        virtual void setup(const std::vector<spoac::ObjectPtr>& objects)
        {
            counter = objects[0];
        }

        virtual spoac::ActionStatePtr run()
        {
            double value = counter->get<double>("value");

            if (value < 0.999)
            {
                (*counter)["value"] = value + 0.1;

                // stay in same state
                return shared_from_this();
            }

            spoac::ActionStatePtr state2(new DummyActionState2);
            return state2;
        }
    };
}

BOOST_AUTO_TEST_CASE(testDummyActionStateMachine)
{
    // create start state instance
    spoac::ActionStatePtr start(new spoactest::DummyActionState1);

    // create state machine with start state
    spoac::ActionPtr action(new spoac::ActionStateMachine(start));

    // set up a counter
    spoac::ObjectPtr counter(new spoac::Object("counter", "counter1"));
    (*counter)["value"] = 0.0;

    std::vector<spoac::ObjectPtr> objects;
    objects.push_back(counter);

    action->setup(objects);

    while (!action->isFinished())
    {
        action->run();
    }

    BOOST_CHECK_CLOSE(counter->get<double>("value"), 2.0, 0.00001);
}
