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

#ifndef SPOAC_STM_STMEXCEPTION_H
#define SPOAC_STM_STMEXCEPTION_H

#include <spoac/common/Exception.h>

namespace spoac
{
    /**
    * Exception for all short term memory related exceptions.
    */
    class STMException : public Exception
    {
    public:
        /**
        * Constructs new STMException with an error message.
        */
        STMException(const std::string& message) throw();

        /**
        * Empty destructor which does not throw.
        */
        virtual ~STMException() throw() {};
    };
}

#endif

