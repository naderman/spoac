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

#ifndef SPOAC_CEA_OBJECT_H
#define SPOAC_CEA_OBJECT_H

#include <string>
#include <boost/shared_ptr.hpp>

namespace spoac
{
    /**
    * Abstract base class for all objects any action can deal with.
    */
    class Object
    {
    public:
        /**
        * Creates an empty object with a name.
        *
        * @param name The (type) name of the object, for example "cup".
        * @param id   The identifier of the object, for example "cup1" or
        *             "first green cup".
        */
        Object(const std::string& name, const std::string& id);

        /**
        * Getter for object (type) name.
        * @return Object's name.
        */
        std::string getName();

        /**
        * Returns the object's id.
        * @return Object's id.
        */
        std::string getId();

    protected:
        std::string name;
        std::string id;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<Object> ObjectPtr;
}

#endif

