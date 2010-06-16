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

#ifndef SPOAC_STM_OBJECTSET_H
#define SPOAC_STM_OBJECTSET_H

#include <set>
#include <boost/shared_ptr.hpp>

#include <spoac/stm/Object.h>
#include <spoac/stm/STMException.h>

namespace spoac
{
    /**
    * A wrapper around std::set< ObjectPtr > and a STL map to look the objects
    * up by id.
    */
    class ObjectSet
    {
    public:
        /**
        * Convenience typedef of the underlying STL set.
        */
        typedef std::set<ObjectPtr> SetType;

        /**
        * Convenience typedef of the underlying STL map.
        */
        typedef std::map<std::string, SetType::iterator> MapType;

        /**
        * Vector size type.
        */
        typedef SetType::size_type size_type;

        /**
        * Set iterator type.
        */
        typedef SetType::iterator iterator;

        /**
        * Constant set iterator type.
        */
        typedef SetType::const_iterator const_iterator;

        /**
        * Map iterator type.
        */
        typedef MapType::iterator iterator_map;

        /**
        * Constant map iterator type.
        */
        typedef MapType::const_iterator const_iterator_map;

        /**
        * Return size
        *
        * @return Number of elements
        */
        size_type size() const;

        /**
        * Inserts a new object into the set.
        */
        void insert(ObjectPtr object);

        /**
        * Returns the object at the given index if it exists. Otherwise an
        * ActionException is thrown.
        *
        * @param  id The id of the requested object
        * @return    The object with the requested id.
        */
        ObjectPtr operator[](const std::string& id);

        /**
        * Alias of operator[] - used to access objects by id
        *
        * Throws a STMException if the object does not exist.
        *
        * @param  id The id of the requested object
        * @return    The object with the requested id.
        */
        ObjectPtr get(const std::string& id);

        /**
        * Checks whether a particular object exists in short term memory
        *
        * @param id The id of the object to be looked up
        * @return   True if the object exists, False otherwise
        */
        bool exists(const std::string& id) const;

        iterator           begin()          { return objects.begin(); }
        const_iterator     begin()    const { return objects.begin(); }
        iterator           end()            { return objects.end();   }
        const_iterator     end()      const { return objects.end();   }

        iterator_map       beginMap()       { return objectMap.begin(); }
        const_iterator_map beginMap() const { return objectMap.begin(); }
        iterator_map       endMap()         { return objectMap.end();   }
        const_iterator_map endMap()   const { return objectMap.end();   }

        /**
        * Copies all data into a JSON::Object representation.
        *
        * @return The JSON object.
        */
        JSON::ObjectPtr toJSON();

        /**
        * Encodes all data as JSON.
        *
        * @return A JSON string representing this object.
        */
        std::string toJSONString();

    protected:
        SetType objects;
        MapType objectMap;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ObjectSet> ObjectSetPtr;
}

#endif

