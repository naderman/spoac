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

#ifndef SPOAC_ICE_ICEHELPER_H
#define SPOAC_ICE_ICEHELPER_H

#include <spoac/common/DependencyManager.h>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>

namespace spoac
{
    namespace ice
    {
        /**
        * Provides a CEA with a sequence of OACs passed in on initialisation.
        */
        class IceHelper
        {
        public:
            /**
            * Creates an instance of this class with the right dependencies.
            *
            * @param manager The DependencyManager providing the necessary
            *                services.
            */
            static boost::shared_ptr<void> createService(
                DependencyManagerPtr manager);

            /**
            * Constructor only initialises counters
            */
            IceHelper();

            /**
            * Closes Ice connections if necessary and unsubscribes from storm
            * topics if subscribed.
            */
            ~IceHelper();

            /**
            * Access the Ice Communicator, it will be created if necessary.
            * The communicator is reference counted statically in this class
            * so never destroy an actual communicator outside.
            *
            * @return A pointer to the communicator
            */
            Ice::CommunicatorPtr ic();

            /**
            * Provide the helper with an external Ice Communicator.
            * Makes sure destroy is never called through fake reference counting
            *
            * @param externalIC The Ice Communicator created elsewhere.
            */
            void setIC(Ice::CommunicatorPtr externalIC);

            /**
            * Provides access to the Ice Storm Topic Manager.
            * A proxy is created if necessary.
            *
            * @return A proxy of the IceStorm TopicManager.
            */
            IceStorm::TopicManagerPrx topicManager();

            /**
            * Retrieves a proxy object.
            *
            * @param  objectIdentity The object's identity, e.g.
            *                        LTM:tcp ‑p 10002
            *
            * @return           A proxy of the remote instance.
            */
            template <class ProxyType>
            ProxyType getProxy(const std::string& objectIdentity)
            {
                Ice::ObjectPrx base = ic()->stringToProxy(objectIdentity);
                ProxyType proxy = ProxyType::checkedCast(base);

                if (!proxy)
                {
                    throw "Invalid proxy!";
                }

                return proxy;
            }

            /**
            * Register an object with Ice for being accessed over the network.
            *
            * @param object     The object to be registered, implementing an Ice
            *                   interface.
            * @param objectName The name this object should be available as.
            * @param endpoints  The endpoints for this object, e.g.
            *                   "tcp -p 10001"
            */
            void registerAdapter(
                Ice::ObjectPtr object,
                const std::string objectName,
                const std::string endpoints);

            /**
            * Gets an Ice Storm topic for publishing messages.
            *
            * @param  topicName The name of the topic to publish on.
            *
            * @return           A proxy of the topic's remote instance.
            */
            template <class TopicProxy>
            TopicProxy stormGetTopic(const std::string& topicName)
            {
                IceStorm::TopicPrx topic = stormTopicRetrieve(topicName);

                Ice::ObjectPrx pub = topic->getPublisher()->ice_oneway();
                TopicProxy castTopic = TopicProxy::uncheckedCast(pub);

                return castTopic;
            }

            /**
            * Subscribe an object to a particular Ice Storm topic.
            *
            * @param subcriber The object to be subscribed, implementing the
            *                  interface for listening on this topic
            * @param topicName The topic's name
            * @param endpoints The object's network endpoints, e.g.
            *                  "tcp -p 10001"
            */
            void stormSubscribeTopic(
                Ice::ObjectPtr subscriber,
                const std::string& topicName,
                const std::string& endpoints);

            /**
            * Unsubscribe from a topic previously subscribed to.
            *
            * @param topicName The name of the subscribed topic.
            */
            void stormTopicUnsubscribe(const std::string& topicName);

        protected:
            static Ice::CommunicatorPtr globalIC;
            static int icCounter;

            /**
            * Retrieves an Ice Storm topic from the topic manager if necessary.
            * Proxies are cached.
            *
            * @param  topicName The name of the topic to publish on.
            *
            * @return           A generic proxy instance of the topic. Has to be
            *                   cast for actual messages to be sent.
            */
            IceStorm::TopicPrx stormTopicRetrieve(const std::string& topicName);

            Ice::CommunicatorPtr iceCommunicator;
            IceStorm::TopicManagerPrx topicManagerProxy;

            std::map<std::string, IceStorm::TopicPrx> topics;
            std::vector<std::pair<std::string, Ice::ObjectPrx> >
                subscriptions;

            int subscriptionCounter;

            typedef DependencyManager::RegisterService<IceHelper>
                RegisterService;
            static RegisterService r;
        };

        /**
        * Pointer type to reduce typing for shared pointers.
        */
        typedef boost::shared_ptr<IceHelper> IceHelperPtr;
    }
}

#endif
