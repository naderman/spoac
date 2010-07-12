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

#include <spoac/stm/STM.h>

using namespace spoac;

STM::RegisterService STM::r;

boost::shared_ptr<void> STM::createService(DependencyManagerPtr manager)
{
    boost::shared_ptr<void> stm(new STM);
    return stm;
}

void STM::addPerceptionHandler(PerceptionHandlerPtr handler)
{
    handlers.push_back(handler);
}

void STM::addPerceptionHandler(
    std::string handlerName, DependencyManagerPtr dependencyManager)
{
    PerceptionHandlerPtr handler = PerceptionHandler::fromName(
        handlerName,
        dependencyManager
    );

    addPerceptionHandler(handler);
}

void STM::clearPerceptionHandlers()
{
    handlers.clear();
}

void STM::setPerceptionHandlers(
    std::vector<std::string> handlerNames,
    DependencyManagerPtr dependencyManager)
{
    std::vector<std::string>::iterator it;

    clearPerceptionHandlers();

    for (it = handlerNames.begin(); it != handlerNames.end(); ++it)
    {
        addPerceptionHandler(*it, dependencyManager);
    }
}

void STM::update()
{
    std::vector<PerceptionHandlerPtr>::iterator it;

    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        (*it)->update(shared_from_this());
    }
}

ObjectVector STM::vectorFromIds(const std::vector<std::string>& ids)
{
    ObjectVector vector;
    std::vector<std::string>::const_iterator id;

    for (id = ids.begin(); id != ids.end(); ++id)
    {
        vector.push_back(get(*id));
    }

    return vector;
}

STM::size_type STM::sizeNonHardcoded() const
{
    STM::size_type counter = 0;

    iterator it;
    for (it = begin(); it != end(); ++it)
    {
        if ( ! (*it)->get<bool>("__hardcoded", false)) // false == default
        {
            counter++;
        }
    }

    return counter;
}

void STM::setScenario(const LTMSlice::Scenario& scenario)
{
    std::vector<PerceptionHandlerPtr>::iterator it;

    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        (*it)->setScenario(scenario);
    }

}

std::vector<std::string> STM::extractPlanConstants()
{
    std::vector<std::string> constants;

    iterator it;
    for (it = begin(); it != end(); ++it)
    {
        // all objects for now
        constants.push_back((*it)->getId());
    }

    return constants;
}
