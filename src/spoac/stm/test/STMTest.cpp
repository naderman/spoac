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

#define BOOST_TEST_MODULE spoac_Object
#include <spoactest/test.h>

#include <iostream>
#include <spoac/stm/STM.h>
#include "CountPerceptionHandler.h"

using spoactest::CountPerceptionHandler;

BOOST_AUTO_TEST_CASE(testSingleHandler)
{
    spoac::STMPtr stm(new spoac::STM);

    boost::shared_ptr<CountPerceptionHandler> handler(
        new CountPerceptionHandler
    );

    CountPerceptionHandler::counter = 0;

    stm->addPerceptionHandler(handler);
    stm->update();

    BOOST_CHECK_EQUAL(CountPerceptionHandler::counter, 1);

    stm->clearPerceptionHandlers();
    stm->update();

    BOOST_CHECK_EQUAL(CountPerceptionHandler::counter, 1);
}

BOOST_AUTO_TEST_CASE(testNamedHandlers)
{
    spoac::STMPtr stm(new spoac::STM);
    spoac::DependencyManagerPtr manager(new spoac::DependencyManager);

    std::vector<std::string> handlers;

    handlers.push_back("CountPerceptionHandler");
    handlers.push_back("CountPerceptionHandler");

    CountPerceptionHandler::counter = 0;

    stm->setPerceptionHandlers(handlers, manager);
    stm->update();

    BOOST_CHECK_EQUAL(CountPerceptionHandler::counter, 2);

    stm->clearPerceptionHandlers();
    stm->update();

    BOOST_CHECK_EQUAL(CountPerceptionHandler::counter, 2);
}

BOOST_AUTO_TEST_CASE(testHardcodedObjectSize)
{
    spoac::ObjectPtr foo1(new spoac::Object("foo", "foo1"));
    (*foo1)["__hardcoded"] = true;
    spoac::ObjectPtr bar1(new spoac::Object("bar", "bar1"));

    spoac::STMPtr stm(new spoac::STM);

    stm->insert(foo1);
    stm->insert(bar1);

    BOOST_CHECK_EQUAL(stm->size(), 2);
    BOOST_CHECK_EQUAL(stm->sizeNonHardcoded(), 1);
}
