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

#ifndef SPOAC_COMMON_DEPENDENCYMANAGER_H
#define SPOAC_COMMON_DEPENDENCYMANAGER_H

#include <spoac/common/Exception.h>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace spoac
{
    class DependencyManager;

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<DependencyManager> DependencyManagerPtr;

    /**
    * The dependency manager is a very simple dependency injection container
    */
    class DependencyManager :
        public boost::enable_shared_from_this<DependencyManager>
    {
    public:
        /**
        * Class instantiation function pointer type.
        *
        * The instantiation function receives the dependency manager as a
        * parameter so it can satisfy constructor dependencies. It returns
        * a void shared_ptr pointing to the service.
        */
        typedef boost::shared_ptr<void>(*CreateFun)(DependencyManagerPtr);

    protected:
        /**
        * Structure of a service stored in the dependency manager.
        */
        struct Service
        {
            CreateFun create;
            bool shared;
            boost::shared_ptr<void> instance;
        };

    public:

        /**
        * Map type for the typeid.name -> Service lookup table.
        */
        typedef std::map<std::string, Service> ServiceMap;

        /**
        * Retrieves the service identified by the given template parameter.
        *
        * It returns a new instance when the service is not shared. Otherwise
        * it will only create an instance of none exists yet.
        *
        * @return A shared_ptr of ServiceT.
        */
        template<typename ServiceT>
        boost::shared_ptr<ServiceT> getService()
        {
            boost::shared_ptr<void> instance;
            Service& service = findService<ServiceT>();

            if (!service.shared || service.instance.get() == NULL)
            {
                instance = service.create(shared_from_this());

                if (service.shared)
                {
                    service.instance = instance;
                }
            }
            else
            {
                instance = service.instance;
            }

            return boost::static_pointer_cast<ServiceT>(instance);
        }

        /**
        * Set the instance used for a shared service.
        *
        * @param instance A pointer to the instance for this service.
        */
        template<typename ServiceT>
        void setService(boost::shared_ptr<ServiceT> instance)
        {
            if (instance.get() == NULL)
            {
                throw spoac::Exception("setService: Called with NULL pointer");
            }

            try
            {
                Service& service = findService<ServiceT>();
                service.instance = boost::static_pointer_cast<void>(instance);
            }
            catch (const spoac::Exception& e)
            {
                // not registered yet
                registerService<ServiceT>(
                    &DependencyManager::throwOnCreate, true
                );
                Service& service = findService<ServiceT>();
                service.instance = boost::static_pointer_cast<void>(instance);
            }
        }

        static boost::shared_ptr<void> throwOnCreate(DependencyManagerPtr m)
        {
            throw spoac::Exception("An unregistered service cannot be created");
        }

        /**
        * Registers a service for retrieval with getService().
        *
        * @param create The instantiation function for this service.
        * @param shared True if there should only be a single instance of this
        *               service, false otherwise.
        */
        template<typename ServiceT>
        static void registerService(CreateFun create, bool shared = true)
        {
            Service service;

            service.create = create;
            service.shared = shared;

            services[std::string(typeid(ServiceT).name())] = service;
        }

        /**
        * A helper class used to register a class in the service map.
        *
        * @param ServiceT The service's type
        * @param shared   True if there should only be a single instance of this
        *                 service, false otherwise.
        */
        template<
            typename ServiceT,
            bool shared = true
        >
        class RegisterService
        {
        public:
            /**
            * This constructor only calls DependencyManager::registerService().
            */
            RegisterService()
            {
                DependencyManager::registerService<ServiceT>(
                    &ServiceT::createService, shared
                );
            }
        };

    protected:
        /**
        * Looks up a service in the ServiceMap and throws an exception if it
        * does not exist.
        *
        * @return An instance of the service class for the given template type.
        */
        template<typename ServiceT>
        Service& findService()
        {
            std::string serviceName(typeid(ServiceT).name());

            ServiceMap::iterator it = services.find(serviceName);

            if (it == services.end())
            {
                throw Exception(std::string("No such service: ") + serviceName);
            }

            return it->second;
        }

        static ServiceMap services;
    };
}

#endif

