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

#ifndef SPOAC_CEA_ACTIONEXCEPTION_H
#define SPOAC_CEA_ACTIONEXCEPTION_H

#include <spoac/common/Exception.h>

namespace spoac
{
    /**
    * Exception for all errors occuring in an action.
    */
    class ActionException : public spoac::Exception
    {
    public:
        /**
        * Constructs new ActionException with an error message.
        */
        ActionException(const std::string& message) throw();

        /**
        * Empty destructor which does not throw.
        */
        virtual ~ActionException() throw() {};
    };
}

#endif

