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

#define BOOST_TEST_MODULE spoac_DependencyManager
#include <spoactest/test.h>

#include <iostream>
#include <spoac/common/DependencyManager.h>

namespace spoactest
{
    class UnregisteredService {};

    class UnsharedService
    {
    public:
        static boost::shared_ptr<void> createService(
            spoac::DependencyManagerPtr manager)
        {
            boost::shared_ptr<void> instance(new UnsharedService);
            return instance;
        }
    private:
        typedef spoac::DependencyManager::RegisterService<
            UnsharedService, false
        > RegisterService;

        static RegisterService r;
    };
    UnsharedService::RegisterService UnsharedService::r;

    class SharedService
    {
    public:
        // depends on UnsharedService
        SharedService(boost::shared_ptr<UnsharedService> u) {};

        static boost::shared_ptr<void> createService(
            spoac::DependencyManagerPtr manager)
        {
            boost::shared_ptr<void> instance(
                new SharedService(manager->getService<UnsharedService>())
            );
            return instance;
        }
    private:
        typedef spoac::DependencyManager::RegisterService<
            SharedService, true
        > RegisterService;

        static RegisterService r;
    };
    SharedService::RegisterService SharedService::r;
}

BOOST_AUTO_TEST_CASE(testUnregisteredServiceThrows)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    BOOST_CHECK_THROW(
        manager->getService<spoactest::UnregisteredService>(),
        spoac::Exception
    )
}

BOOST_AUTO_TEST_CASE(testUnsharedService)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    boost::shared_ptr<spoactest::UnsharedService> s1, s2;

    BOOST_REQUIRE_NO_THROW(
        s1 = manager->getService<spoactest::UnsharedService>()
    );
    BOOST_REQUIRE_NO_THROW(
        s2 = manager->getService<spoactest::UnsharedService>()
    );

    BOOST_CHECK(s1.get() != NULL);
    BOOST_CHECK(s2.get() != NULL);
    BOOST_CHECK(s1.get() != s2.get());
}

BOOST_AUTO_TEST_CASE(testSharedService)
{
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    boost::shared_ptr<spoactest::SharedService> s1, s2;

    BOOST_REQUIRE_NO_THROW(
        s1 = manager->getService<spoactest::SharedService>()
    );
    BOOST_REQUIRE_NO_THROW(
        s2 = manager->getService<spoactest::SharedService>()
    );

    BOOST_CHECK(s1.get() != NULL);
    BOOST_CHECK(s1.get() == s2.get());
}
