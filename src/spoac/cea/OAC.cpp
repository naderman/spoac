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

#include <spoac/cea/OAC.h>
#include <spoac/cea/ActionException.h>

using namespace spoac;

OAC::OAC(const std::string& name, const std::vector<std::string>& objectIds) :
    name(name), objectIds(objectIds)
{
}

std::string OAC::getName() const
{
    return name;
}

std::vector<std::string> OAC::getObjectIds() const
{
    return objectIds;
}

ActionPtr OAC::setupAction(DependencyManagerPtr manager) const
{
    STMPtr stm = manager->getService<STM>();

    ObjectVector objects = stm->vectorFromIds(getObjectIds());

    ActionPtr action = Action::fromName(getName(), manager);

    if (action.get() == NULL)
    {
        throw ActionException(
            std::string("Cannot create unregistered action: '") + getName() +
            std::string("'")
        );
    }

    action->setup(objects);

    return action;
}

SymbolicExecutionSlice::Action OAC::getIceAction() const
{
    SymbolicExecutionSlice::Action action;
    action.name = getName();

    std::vector<std::string>::const_iterator it;

    for (it = objectIds.begin(); it != objectIds.end(); ++it)
    {
        SymbolicExecutionSlice::Obj obj;
        obj.id = *it;
        action.parameters.push_back(obj);
    }

    return action;
}

