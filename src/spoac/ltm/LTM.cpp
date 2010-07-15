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

#include <spoac/ltm/LTM.h>
#include <spoac/common/Exception.h>
#include <iostream>

using namespace spoac;
namespace fs = boost::filesystem;

LTMSlice::Scenario LTM::getScenario(
    const std::string& name,
    const Ice::Current& c)
{
    LTMSlice::Scenario scenario;

    JSON::ValuePtr value = findAndParseFile("scenarios", name);

    if (value->getType() != JSON::OBJECT)
    {
        throw Exception(std::string("Scenario ") + name +
            " is not a JSON object");
    }

    JSON::Object& document = value->toObject();

    scenario.name = document["name"]->toString();

    scenario.activityControllers = vectorFromArray(
        document["activityControllers"]);
    scenario.perceptionHandlers = vectorFromArray(
        document["perceptionHandlers"]);
    scenario.oacs = vectorFromArray(
        document["oacs"]);

    scenario.goals = mapFromObject(
        document["goals"]);

    if (document["predicates"]->getType() == JSON::ARRAY)
    {
        JSON::Array& array = document["predicates"]->toArray();
        JSON::Array::const_iterator it;

        for (it = array.begin(); it != array.end(); ++it)
        {
            PlanningSlice::PredicateDefinition p;
            JSON::Object& def = (*it)->toObject();
            p.name = def["name"]->toString();
            p.arguments = (short) def["arguments"]->toInt();

            scenario.predicates.push_back(p);
        }
    }

    if (document["functions"]->getType() == JSON::ARRAY)
    {
        JSON::Array& array = document["functions"]->toArray();
        JSON::Array::const_iterator it;

        for (it = array.begin(); it != array.end(); ++it)
        {
            PlanningSlice::FunctionDefinition f;
            JSON::Object& def = (*it)->toObject();
            f.name = def["name"]->toString();
            f.arguments = (short) def["arguments"]->toInt();

            scenario.functions.push_back(f);
        }
    }

    return scenario;
}

LTMSlice::ActionConfig LTM::getActionConfig(
    const LTMSlice::OAC& oacInstance,
    const Ice::Current& c)
{
    bool found = false;
    LTMSlice::ActionConfig actionConfig;

    JSON::ValuePtr value = findAndParseFile("oacs", oacInstance.name);

    if (value->getType() != JSON::OBJECT)
    {
        throw Exception(std::string("OAC ") + oacInstance.name +
            " is not a JSON object");
    }

    JSON::Object& document = value->toObject();

    std::map<std::string, int> isParam;

    if (document["params"]->getType() == JSON::ARRAY)
    {
        JSON::Array& params = document["params"]->toArray();
        JSON::Array::iterator param;
        int i = 0;

        for (param = params.begin(); param != params.end(); ++param, ++i)
        {
            if ((*param)->getType() == JSON::STRING)
            {
                isParam[(*param)->toString()] = i;
            }
        }
    }

    if (document["match"]->getType() == JSON::ARRAY)
    {
        JSON::Array& matches = document["match"]->toArray();
        JSON::Array::const_iterator match;

        for (match = matches.begin(); match != matches.end(); ++match)
        {
            if (checkOACMatch(oacInstance, *match, isParam))
            {
                actionConfig.name = (*match)->toObject()["action"]->toString();

                if ((*match)->toObject()["config"]->getType() != JSON::NULLTYPE)
                {
                    actionConfig.config =
                        (*match)->toObject()["config"]->toJSON();
                }

                found = true;
                break;
            }
        }
    }

    if (!found && document["action"]->getType() == JSON::STRING)
    {
        actionConfig.name = document["action"]->toString();

        if (document["config"]->getType() != JSON::NULLTYPE)
        {
            actionConfig.config = document["config"]->toJSON();
        }
    }

    return actionConfig;
}

PlanningSlice::ActionDefinition LTM::getAction(
    const std::string& oac,
    const Ice::Current& c)
{
    PlanningSlice::ActionDefinition action;

    JSON::ValuePtr value = findAndParseFile("oacs", oac);

    if (value->getType() != JSON::OBJECT)
    {
        throw Exception(std::string("OAC ") + oac +
            " is not a JSON object");
    }

    JSON::Object& document = value->toObject();

    action.name = document["name"]->toString();

    if (document["precondition"]->getType() == JSON::STRING)
    {
        action.precondition = document["precondition"]->toString();
    }
    if (document["effect"]->getType() == JSON::STRING)
    {
        action.effect = document["effect"]->toString();
    }

    if (document["params"]->getType() == JSON::ARRAY)
    {
        JSON::Array& array = document["params"]->toArray();
        JSON::Array::const_iterator it;

        for (it = array.begin(); it != array.end(); ++it)
        {
            PlanningSlice::ActionParameter p;
            p.name = (*it)->toString();

            action.parameters.push_back(p);
        }
    }

    return action;
}

std::vector<std::string> LTM::vectorFromArray(JSON::ValuePtr value)
{
    std::vector<std::string> result;

    if (value->getType() == JSON::ARRAY)
    {
        JSON::Array& array = value->toArray();
        JSON::Array::const_iterator it;

        for (it = array.begin(); it != array.end(); ++it)
        {
            result.push_back((*it)->toString());
        }
    }

    return result;
}

std::map<std::string, std::string> LTM::mapFromObject(JSON::ValuePtr value)
{
    std::map<std::string, std::string> result;

    if (value->getType() == JSON::OBJECT)
    {
        JSON::Object& object = value->toObject();
        JSON::Object::const_iterator it;

        for (it = object.begin(); it != object.end(); ++it)
        {
            result[it->first.getString()->toString()] = it->second->toString();
        }
    }

    return result;
}

bool LTM::checkOACMatch(
    const LTMSlice::OAC& oac,
    JSON::ValuePtr match,
    const std::map<std::string, int>& isParam)
{
    if (match->getType() != JSON::OBJECT)
    {
        return false;
    }

    JSON::Object& object = match->toObject();
    JSON::Object::const_iterator it;
    std::map<std::string, int>::const_iterator param;

    for (it = object.begin(); it != object.end(); ++it)
    {
        param = isParam.find(it->first.getString()->toString());
        if (param == isParam.end())
        {
            return false;
        }

        size_t offset = param->second;

        if (offset >= oac.objects.size())
        {
            return false;
        }

        LTMSlice::Obj obj = oac.objects[offset];

        if (it->second->getType() == JSON::STRING)
        {
            if (it->second->toString() != obj.id)
            {
                return false;
            }
        }
        else if (it->second->getType() == JSON::OBJECT)
        {
            if (!checkObjectMatch(obj, it->second->toObject()))
            {
                return false;
            }
        }
    }

    return true;
}

bool LTM::checkObjectMatch(
    const LTMSlice::Obj& object,
    const JSON::Object& match)
{
    JSON::Object::const_iterator it;

    for (it = match.begin(); it != match.end(); ++it)
    {
        LTMSlice::PropertyMap::const_iterator prop =
            object.properties.find(it->first.getString()->toString());

        if (prop == object.properties.end())
        {
            return false;
        }

        if (it->second->getType() == JSON::STRING)
        {
            if (prop->second !=
                std::string("\"") + it->second->toString() + "\"")
            {
                return false;
            }
        }
        /*else if (it->second->getType() == JSON::NUMBER)
        {
        }*/
    }

    return true;
}

JSON::ValuePtr LTM::findAndParseFile(
    const std::string& dir,
    const std::string& name)
{
    JSON::ValuePtr result(new JSON::Value);

    const char* home = getenv("MCAPROJECTHOME");

    if (home == NULL)
    {
        home = "./";
    }

    fs::path base(home);
    base /= "cognition/memory/ltm_db";
    base /= dir;

    fs::path path;

    if (findFile(base, name + ".json", path))
    {
        std::cout << "Reading file: " << path.string() << std::endl;
        JSON::Parser jsonParser;
        result = jsonParser.readFromFile(path.string());
    }
    else
    {
        throw Exception(std::string("Could not find file ") + name + ".json" +
            " in " + base.string());
    }

    return result;
}

bool LTM::findFile(
    const fs::path& dirPath,
    const std::string& fileName,
    fs::path& pathFound)
{
    if (!fs::exists(dirPath))
    {
        return false;
    }

    fs::directory_iterator endItr;

    for (fs::directory_iterator itr(dirPath); itr != endItr; ++itr)
    {
        if (fs::is_directory(itr->status()))
        {
            if (findFile(itr->path(), fileName, pathFound))
            {
                return true;
            }
        }
        else if (itr->leaf() == fileName)
        {
            pathFound = itr->path();
            return true;
        }
    }

    return false;
}


