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

#ifndef SPOAC_LTM_LTM_H
#define SPOAC_LTM_LTM_H

#include <spoac/LTM.h>
#include <spoac/JSON/Parser.h>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

namespace spoac
{
    /**
    * Representation of the robot's short term memory.
    */
    class LTM : public LTMSlice::LTM
    {
    public:
        /**
        * Retrieves a scenario definition from the filesystem
        */
        LTMSlice::Scenario getScenario(
            const std::string& name,
            const Ice::Current& c);

        /**
        * Retrieves an Action Definition for the planner from OAC definitions
        * on disk.
        */
        PlanningSlice::ActionDefinition getAction(
            const std::string& oac,
            const Ice::Current& c);

        /**
        * Retrieve an action configuration for execution on the robot.
        */
        LTMSlice::ActionConfig getActionConfig(
            const LTMSlice::OAC& oacInstance,
            const Ice::Current& c);

        bool checkOACMatch(
            const LTMSlice::OAC& oac,
            JSON::ValuePtr match,
            const std::map<std::string, int>& isParam);

        bool checkObjectMatch(
            const LTMSlice::Obj& object,
            const JSON::Object& match);

    protected:
        std::vector<std::string> vectorFromArray(JSON::ValuePtr value);
        std::map<std::string, std::string> mapFromObject(JSON::ValuePtr value);

        JSON::ValuePtr findAndParseFile(
            const std::string& dir,
            const std::string& name);

        bool findFile(
            const boost::filesystem::path& dirPath,
            const std::string& fileName,
            boost::filesystem::path& pathFound);
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<LTM> LTMPtr;
}

#endif

