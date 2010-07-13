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

#ifndef SPOAC_CEA_ACTIONSTATEFINAL_H
#define SPOAC_CEA_ACTIONSTATEFINAL_H

#include <spoac/cea/ActionState.h>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace spoac
{
    /**
    * Abstract base class for all states the ActionStateMachine can execute.
    */
    class ActionStateFinal :
        public ActionState
    {
    public:
        /**
        * Generic setup method with parameters that are ignored.
        *
        * @param objects The objects this action should be applied on.
        * @param config  A JSON configuration object retrieved from LTM.
        */
        virtual void setup(const ObjectVector& objects, JSON::ValuePtr config);

        /**
        * Does nothing but return the same state.
        *
        * @return  Always shared_from_this().
        */
        virtual ActionStatePtr run();

        /**
        * Define that this is a final state.
        *
        * @return True
        */
        virtual bool isFinal() const;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<ActionStateFinal> ActionStateFinalPtr;
}

#endif

