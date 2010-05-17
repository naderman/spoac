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

#ifndef SPOAC_STM_OBJECTVECTOR_H
#define SPOAC_STM_OBJECTVECTOR_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include <spoac/stm/Object.h>
#include <spoac/stm/ParameterException.h>

namespace spoac
{
    /**
    * A wrapper around std::vector< ObjectPtr > with utility methods.
    */
    class ObjectVector
    {
    public:
        /**
        * Vector size type.
        */
        typedef std::vector<ObjectPtr>::size_type size_type;

        /**
        * Add element at the end.
        */
        void push_back(const ObjectPtr& object);

        /**
        * Return size
        *
        * @return Number of elements
        */
        size_type size() const;

        /**
        * Returns the object at the given index if it exists. Otherwise a
        * ParameterException is thrown.
        *
        * @param  i The index of the requested object
        * @return   The object at position i.
        */
        ObjectPtr operator[](const int& i) const;

        /**
        * Throws an ParameterException if the number of elements does not match
        * the specified amount.
        *
        * @param n The amount of expected elements
        */
        void validate(size_type n) const;

        /**
        * Throws a ParameterException if the number of element does not match
        * the number of specified names, or if any of the object names do not
        * match the expected names.
        *
        * @param names Expected object names.
        */
        void validate(const std::vector<std::string>& names) const;

        /**
        * Throws a ParameterException if there are zero or more than one
        * elements or if the only element does not have the specified name.
        *
        * @param name Expected object name.
        */
        void validate(const std::string& name) const;

        /**
        * Throws a ParameterException if there are less than or more than two
        * elements or if the only two element do not have the specified names.
        *
        * @param name1 First object's expected name.
        * @param name2 Second object's expected name.
        */
        void validate(const std::string& name1, const std::string& name2) const;

        /**
        * Throws a ParameterException if there are less than or more than two
        * elements or if the only two element do not have the specified names.
        *
        * @param name1 First object's expected name.
        * @param name2 Second object's expected name.
        * @param name2 Third object's expected name.
        */
        void validate(
            const std::string& name1,
            const std::string& name2,
            const std::string& name3
        ) const;

        /**
        * Validates that there is exactly one object and returns it.
        *
        * @return The first object in the vector
        */
        ObjectPtr getValidated() const;

        /**
        * Validates that the object at the given position exists and returns it.
        *
        * @param  i The index of the requested object
        * @return   The object at position i.
        */
        ObjectPtr getValidated(size_type i) const;

        /**
        * Validates that the object at the given position has the correct name
        * and returns it.
        *
        * @param i    The index of the requested object
        * @param name Expected object name
        * @return     The object if successful.
        */
        ObjectPtr getValidated(size_type i, const std::string& name) const;

        /**
        * Validates that there is exactly one object of the correct name and
        * returns it.
        *
        * @param name The expected object name
        */
        ObjectPtr getValidated(const std::string& name) const;

    protected:
        std::vector<ObjectPtr> objects;

        void validateName(ObjectPtr obj, const std::string& name) const;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ObjectVector> ObjectVectorPtr;
}

#endif

