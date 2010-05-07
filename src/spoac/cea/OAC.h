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

#ifndef SPOAC_CEA_OAC_H
#define SPOAC_CEA_OAC_H

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

namespace spoac
{
    /**
    * The basic Object Action Complex data structure.
    *
    * It contains the action name and ids of all involved objects.
    */
    class OAC
    {
    public:
        /**
        * Object Action Complex constructor.
        *
        * @param name    Name of the OAC's action.
        * @param objects Vector of short term memory object identifiers.
        */
        OAC(const std::string& name, const std::vector<std::string>& objects);

        /**
        * Getter for the OAC action name.
        */
        std::string getName() const;

        /**
        * Getter for the involved objects.
        */
        std::vector<std::string> getObjects() const;

    protected:
        std::string name;
        std::vector<std::string> objects;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<OAC> OACPtr;
}

#endif

