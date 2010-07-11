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

#include <spoac/ice/IceHelper.h>

using namespace spoac;
using namespace spoac::ice;

IceHelper::RegisterService IceHelper::r;
Ice::CommunicatorPtr IceHelper::globalIC;
int IceHelper::icCounter;

boost::shared_ptr<void> IceHelper::createService(DependencyManagerPtr manager)
{
    boost::shared_ptr<void> iceHelper(new IceHelper());
    return iceHelper;
}

IceHelper::~IceHelper()
{
    // unsubscribe all
    if (iceCommunicator)
    {
        std::vector<std::pair<std::string, Ice::ObjectPrx> >::iterator it;

        for (it = subscriptions.begin(); it != subscriptions.end(); ++it)
        {
            stormTopicRetrieve(it->first)->unsubscribe(it->second);
        }

        subscriptions.clear();
    }

    if (globalIC && icCounter == 1)
    {
        globalIC->destroy();
    }

    icCounter--;
}

Ice::CommunicatorPtr IceHelper::ic()
{
    if (!globalIC)
    {
        try {
            globalIC = Ice::initialize();
        } catch (const Ice::Exception& e) {
            std::cerr << e << std::endl;
        } catch (const char* msg) {
            std::cerr << msg << std::endl;
        }
    }

    icCounter++;
    iceCommunicator = globalIC;

    return iceCommunicator;
}

IceStorm::TopicManagerPrx IceHelper::topicManager()
{
    if (!topicManagerProxy)
    {
        Ice::ObjectPrx obj = ic()->stringToProxy(
            "IceStorm/TopicManager:tcp -p 10000");
        topicManagerProxy = IceStorm::TopicManagerPrx::checkedCast(obj);
    }

    return topicManagerProxy;
}

void IceHelper::stormSubscribeTopic(
    Ice::ObjectPtr subscriber,
    const std::string& topicName,
    const std::string& endpoints)
{
    Ice::ObjectAdapterPtr adapter =
        ic()->createObjectAdapterWithEndpoints(
        topicName, endpoints);

    Ice::ObjectPrx subscriberProxy = adapter->addWithUUID(subscriber)->ice_oneway();

    IceStorm::TopicPrx topic = stormTopicRetrieve(topicName);

    IceStorm::QoS qos;
    topic->subscribeAndGetPublisher(qos, subscriberProxy);

    adapter->add(subscriber, ic()->stringToIdentity(topicName));
    adapter->activate();

    subscriptions.push_back(std::pair<std::string, Ice::ObjectPrx>(topicName, subscriberProxy));
}

IceStorm::TopicPrx IceHelper::stormTopicRetrieve(const std::string& topicName)
{
    IceStorm::TopicPrx topic;

    std::map<std::string, IceStorm::TopicPrx>::iterator topicIterator =
        topics.find(topicName);

    if (topicIterator == topics.end())
    {
        try {
            topic = topicManager()->retrieve(topicName);
        }
        catch (const IceStorm::NoSuchTopic&) {
            topic = topicManager()->create(topicName);
        }

        topics[topicName] = topic;
    }
    else
    {
        topic = topicIterator->second;
    }

    return topic;
}
