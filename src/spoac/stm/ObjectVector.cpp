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

#include <spoac/stm/ObjectVector.h>
#include <spoac/stm/ParameterException.h>

#include <boost/lexical_cast.hpp>

using namespace spoac;

void ObjectVector::push_back(const ObjectPtr& object)
{
    objects.push_back(object);
}

ObjectVector::size_type ObjectVector::size() const
{
    return objects.size();
}

ObjectPtr ObjectVector::operator[](const int& i) const
{
    return getValidated(i);
}

void ObjectVector::validate(ObjectVector::size_type n) const
{
    if (objects.size() != n)
    {
        throw ParameterException(
            std::string("Incorrect number of parameters: ") +
            boost::lexical_cast<std::string>(objects.size())
        );
    }
}

void ObjectVector::validate(const std::vector<std::string>& names) const
{
    // check size
    validate(names.size());

    std::vector<std::string>::const_iterator nameIt;
    std::vector<ObjectPtr>::const_iterator objIt;

    for (
        objIt = objects.begin(), nameIt = names.begin();
        objIt != objects.end();
        ++objIt, ++nameIt)
    {
        validateName(*objIt, *nameIt);
    }
}

void ObjectVector::validate(const std::string& name) const
{
    std::vector<std::string> names;
    names.push_back(name);

    validate(names);
}

void ObjectVector::validate(
    const std::string& name1, const std::string& name2
) const
{
    std::vector<std::string> names;
    names.push_back(name1);
    names.push_back(name2);

    validate(names);
}

void ObjectVector::validate(
    const std::string& name1,
    const std::string& name2,
    const std::string& name3
) const
{
    std::vector<std::string> names;
    names.push_back(name1);
    names.push_back(name2);
    names.push_back(name3);

    validate(names);
}

ObjectPtr ObjectVector::getValidated() const
{
    validate(1);

    return objects.at(0);
}

ObjectPtr ObjectVector::getValidated(ObjectVector::size_type i) const
{
    if (objects.size() <= i)
    {
        throw ParameterException(
            std::string("Undefined object vector offset: ") +
            boost::lexical_cast<std::string>(i)
        );
    }

    return objects.at(i);
}

ObjectPtr ObjectVector::getValidated(
    ObjectVector::size_type i, const std::string& name
) const
{
    ObjectPtr object = getValidated(i);

    validateName(object, name);

    return object;
}

ObjectPtr ObjectVector::getValidated(const std::string& name) const
{
    validate(1);

    return getValidated(0, name);
}

void ObjectVector::validateName(ObjectPtr obj, const std::string& name) const
{
    if (obj->getName() != name)
    {
        throw ParameterException(
            std::string("Incorrect parameter name: '") +
            obj->getName() + std::string("' expected '") +
            name + std::string("'")
        );
    }
}

