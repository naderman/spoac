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

#define BOOST_TEST_MODULE spoac_IceNetworkController
#include <spoactest/test.h>

#include <iostream>
#include <spoac/cea/CEA.h>
#include <spoac/controller/IceNetworkController.h>
#include "../../cea/test/CountTwoAction.h"

using namespace spoac::SymbolicExecutionSlice;

namespace spoactest
{
    class CheckingCEA : public spoac::CEAControl
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

        static int requestCount;
    };
    int CheckingCEA::requestCount;
}

using spoac::controller::IceNetworkController;
using spoac::controller::IceNetworkControllerPtr;

BOOST_AUTO_TEST_CASE(testEmpty)
{
    boost::shared_ptr<spoactest::CheckingCEA> cea(new spoactest::CheckingCEA);
    spoac::ice::IceHelperPtr iceHelper(new spoac::ice::IceHelper);

    IceNetworkControllerPtr controller(
        new IceNetworkController(
            spoac::CEAControlWeakPtr(cea),
            iceHelper
        )
    );

    spoac::SymbolicExecutionSlice::CEAControllerTopicPrx controllerTopic;
    Ice::CommunicatorPtr ic;

    try
    {
        ic = Ice::initialize();

        Ice::ObjectPrx obj = ic->stringToProxy(
            "IceStorm/TopicManager:tcp -p 10000");
        IceStorm::TopicManagerPrx topicManager =
            IceStorm::TopicManagerPrx::checkedCast(obj);

        IceStorm::TopicPrx topic;
        try {
            topic = topicManager->retrieve("CEAController");
        }
        catch (const IceStorm::NoSuchTopic&) {
            topic = topicManager->create("CEAController");
        }

        Ice::ObjectPrx pub = topic->getPublisher()->ice_oneway();
        controllerTopic = CEAControllerTopicPrx::uncheckedCast(pub);
    }
    catch (const Ice::Exception& e)
    {
        std::cerr << e << std::endl;
    }
    catch (const char* msg)
    {
        std::cerr << msg << std::endl;
    }

    spoactest::CheckingCEA::requestCount = 0;

    //controller->requestAction();
    Action action1;
    action1.name = "action1";
    Action action2;
    action2.name = "action2";

    controllerTopic->startAction(action1);
    // wait a bit to allow the network message to be processed
    IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(100));

    BOOST_CHECK_EQUAL(spoactest::CheckingCEA::requestCount, 1);

    controllerTopic->startAction(action2);
    // wait a bit to allow the network message to be processed
    IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(100));

    BOOST_CHECK_EQUAL(spoactest::CheckingCEA::requestCount, 2);
}
