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

#define BOOST_TEST_MODULE spoac_LTM
#include <spoactest/test.h>

#include <iostream>
#include <spoac/ltm/LTM.h>
#include <spoac/ice/IceHelper.h>

BOOST_AUTO_TEST_CASE(testGetScenario)
{
    spoac::ice::IceHelperPtr iceHelper(new spoac::ice::IceHelper);

    setenv("MCAPROJECTHOME", "./", 1);

    Ice::ObjectPtr ltmObject = new spoac::LTM;
    iceHelper->registerAdapter(ltmObject, "LTM", "tcp -p 10099");

    spoac::LTMSlice::LTMPrx ltm;
    ltm = iceHelper->getProxy<spoac::LTMSlice::LTMPrx>("LTM:tcp -p 10099");

    spoac::LTMSlice::Scenario scenario = ltm->getScenario("abc");

    BOOST_CHECK_EQUAL(std::string("abc"), scenario.name);
    BOOST_CHECK_EQUAL(std::string("IceNetwork"), scenario.activityControllers[0]);

    BOOST_CHECK_EQUAL(1, scenario.predicates.size());
    BOOST_CHECK_EQUAL(std::string("p"), scenario.predicates[0].name);

    BOOST_CHECK_EQUAL(1, scenario.functions.size());
    BOOST_CHECK_EQUAL(std::string("f"), scenario.functions[0].name);
}

BOOST_AUTO_TEST_CASE(testParameterMatching)
{
    spoac::LTMPtr ltm(new spoac::LTM);

    spoac::LTMSlice::Obj object1;
    object1.id = "obj1";
    object1.properties.insert(
        std::pair<std::string, std::string>("p1", "\"val1\"")); // value is JSON

    spoac::LTMSlice::Obj object2;
    object2.id = "obj2";
    object2.properties.insert(
        std::pair<std::string, std::string>("p1", "\"val2\""));
    object2.properties.insert(
        std::pair<std::string, std::string>("p2", "\"val1\""));

    std::map<std::string, int> isParam;
    isParam.insert(std::pair<std::string, int>("x", 0));
    isParam.insert(std::pair<std::string, int>("y", 1));

    JSON::Object match1;
    JSON::ValuePtr val1(new JSON::String("val1"));
    match1["p1"] = val1;
    BOOST_CHECK(ltm->checkObjectMatch(object1, match1));

    JSON::Object match2;
    JSON::ValuePtr val2(new JSON::String("wrong"));
    match2["p1"] = val2;
    BOOST_CHECK( ! ltm->checkObjectMatch(object1, match2));

    JSON::Object match3;
    match3["non-existent"] = val1;
    BOOST_CHECK( ! ltm->checkObjectMatch(object1, match3));

    spoac::LTMSlice::OAC oac1;
    oac1.name = "test-oac";
    oac1.objects.push_back(object1);
    oac1.objects.push_back(object2);

    JSON::ValuePtr oacMatch1(new JSON::Object());
    JSON::ValuePtr match4(new JSON::String("obj1"));
    oacMatch1->toObject().insert("x", match4);

    BOOST_CHECK(ltm->checkOACMatch(oac1, oacMatch1, isParam));

    JSON::ValuePtr oacMatch2(new JSON::Object());
    JSON::ValuePtr match5(new JSON::String("obj2"));
    oacMatch2->toObject().insert("x", match4);
    oacMatch2->toObject().insert("y", match5);

    BOOST_CHECK(ltm->checkOACMatch(oac1, oacMatch2, isParam));

    JSON::ValuePtr oacMatch3(new JSON::Object());
    JSON::ValuePtr match6(new JSON::Object(match1));
    oacMatch3->toObject().insert("x", match6);
    oacMatch3->toObject().insert("y", match5);

    BOOST_CHECK(ltm->checkOACMatch(oac1, oacMatch3, isParam));

    JSON::ValuePtr oacMatch4(new JSON::Object());
    oacMatch4->toObject().insert("x", match6);
    oacMatch4->toObject().insert("y", match4);

    BOOST_CHECK( ! ltm->checkOACMatch(oac1, oacMatch4, isParam));
}
