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

#include <spoac/stm/ObjectSet.h>

using namespace spoac;

ObjectSet::size_type ObjectSet::size() const
{
    return objects.size();
}

void ObjectSet::insert(ObjectPtr object)
{
    std::pair<ObjectSet::SetType::iterator, bool> result =
        objects.insert(object);

    if (result.second)
    {
        objectMap[object->getId()] = result.first;
    }
}

ObjectPtr ObjectSet::operator[](const std::string& id)
{
    ObjectSet::MapType::iterator objIt = objectMap.find(id);

    if (objIt == objectMap.end())
    {
        throw STMException(std::string("Object not found: ") + id);
    }

    return *(objIt->second);
}

ObjectPtr ObjectSet::get(const std::string& id)
{
    return operator[](id);
}

bool ObjectSet::exists(const std::string& id) const
{
    if (objectMap.find(id) == objectMap.end())
    {
        return false;
    }

    return true;
}

std::string ObjectSet::toJSONString()
{
    JSON::ObjectPtr object = toJSON();

    return object->toJSON();
}

JSON::ObjectPtr ObjectSet::toJSON()
{
    JSON::ObjectPtr object(new JSON::Object);
    iterator_map it;

    for (it = beginMap(); it != endMap(); ++it)
    {
        (*object)[it->first] = (*(it->second))->toJSON();
    }

    return object;
}
