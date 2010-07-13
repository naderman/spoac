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

#ifndef SPOAC_STM_OBJECT_H
#define SPOAC_STM_OBJECT_H

#include <spoac/JSON/AllValueTypes.h>
#include <spoac/stm/VariantMap.h>
#include <spoac/LTM.h>

#include <string>
#include <boost/shared_ptr.hpp>

namespace spoac
{
    /**
    * Abstract base class for all objects any action can deal with.
    */
    class Object : public VariantMap<
        std::string,
        std::string, bool, int, double, JSON::ValuePtr,
        std::pair<bool, std::string>
    >
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

        /**
        * Encodes the object for use with long term memory.
        */
        LTMSlice::Obj toLTMObj();

    protected:
        /**
        * A visitor which encodes the variant as a JSON object.
        */
        struct JSONVisitor : boost::static_visitor<JSON::ValuePtr>
        {
            JSON::ValuePtr operator()(const int& x) const
            {
                JSON::ValuePtr value(new JSON::Number(x));
                return value;
            }

            JSON::ValuePtr operator()(const double& x) const
            {
                JSON::ValuePtr value(new JSON::Number(x));
                return value;
            }

            JSON::ValuePtr operator()(const JSON::ValuePtr& x) const
            {
                return x;
            }

            JSON::ValuePtr operator()(const std::string& x) const
            {
                JSON::ValuePtr value(new JSON::String(x));
                return value;
            }

            JSON::ValuePtr operator()(const std::pair<bool, std::string>& rel) const
            {
                JSON::ValuePtr value(new JSON::Object);
                JSON::ValuePtr related(new JSON::Bool(rel.first));
                JSON::ValuePtr to(new JSON::String(rel.second));

                value->toObject().insert("related", related);
                value->toObject().insert("to", to);
                return value;
            }
        };

        std::string name;
        std::string id;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<Object> ObjectPtr;
}

#endif

