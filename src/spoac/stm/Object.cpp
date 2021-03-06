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

#include <spoac/stm/Object.h>

using namespace spoac;

Object::Object(const std::string& name, const std::string& id) :
    name(name),
    id(id)
{
}

std::string Object::getName()
{
    return name;
}

std::string Object::getId()
{
    return id;
}

std::string Object::toJSONString()
{
    JSON::ObjectPtr object = toJSON();

    return object->toJSON();
}

JSON::ObjectPtr Object::toJSON()
{
    JSONVisitor visitor;
    JSON::ObjectPtr object(new JSON::Object);
    iterator it;

    for (it = begin(); it != end(); ++it)
    {
        (*object)[it->first] = boost::apply_visitor(visitor, it->second);
    }

    return object;
}

LTMSlice::Obj Object::toLTMObj()
{
    LTMSlice::Obj obj;

    obj.id = getId();

    JSONVisitor visitor;
    iterator it;

    for (it = begin(); it != end(); ++it)
    {
        obj.properties.insert(
            std::pair<std::string, std::string>(
                it->first, boost::apply_visitor(visitor, it->second)->toJSON()
            )
        );
    }

    return obj;
}
