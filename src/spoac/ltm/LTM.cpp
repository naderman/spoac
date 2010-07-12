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
#include <iostream>

using namespace spoac;
namespace fs = boost::filesystem;

LTMSlice::Scenario LTM::getScenario(
    const std::string& name,
    const Ice::Current& c)
{
    LTMSlice::Scenario scenario;

    JSON::ValuePtr value = findAndParseFile("scenarios", name);

    JSON::Object& document = value->toObject();

    scenario.name = document["name"]->toString();

    scenario.activityControllers = vectorFromArray(
        document["activityControllers"]);
    scenario.perceptionHandlers = vectorFromArray(
        document["perceptionHandlers"]);
    scenario.perceptionHandlers = vectorFromArray(
        document["oacs"]);

    scenario.goals = mapFromObject(
        document["goals"]);

    return scenario;
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
        JSON::Parser jsonParser;
        result = jsonParser.readFromFile(path.string());
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


