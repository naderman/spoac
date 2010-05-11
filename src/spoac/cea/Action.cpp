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

#include <spoac/cea/Action.h>

#include <boost/lexical_cast.hpp>

using namespace spoac;

ObjectPtr Action::singleObject(
    const std::vector<ObjectPtr>& objects,
    const std::string& name)
{
    if (objects.size() != 1)
    {
        throw ActionException(
            std::string("Incorrect number of parameters: ") +
            boost::lexical_cast<std::string>(objects.size())
        );
    }

    ObjectPtr object = objects.at(0);

    if (object->getName() != name)
    {
        throw ActionException(
            std::string("Incorrect parameter name: '") +
            object->getName() + std::string("' expected '") +
            name + std::string("'")
        );
    }

    return object;
}