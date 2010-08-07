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

#ifndef SPOAC_CEA_SUPERACTION_H
#define SPOAC_CEA_SUPERACTION_H

#include <boost/shared_ptr.hpp>

#include <spoac/cea/CEA.h>

namespace spoac
{
    /**
    * An action for a generic super OAC. It simply yields to the yub OACs
    * given in the configuration.
    */
    class SuperAction : public Action
    {
    public:
        static std::string getName()
        {
            return "SuperOAC";
        }

        static boost::shared_ptr<Action> createInstance(
            spoac::DependencyManagerPtr manager
        )
        {
            boost::shared_ptr<Action> action(new SuperAction(
                spoac::CEAControlWeakPtr(manager->getService<spoac::CEA>())));
            return action;
        }

        /**
        * This action requires access to the CEA to yield to subOACs.
        *
        * @param cea A pointer to the CEA.
        */
        SuperAction(CEAControlWeakPtr cea);

        /**
        * Reads the list of OACs from configuration to prepare for execution.
        *
        * @param objects Parameter objects that need to be passed on to subOACs
        * @param config  A JSON configuration object retrieved from LTM.
        */
        virtual void setup(const ObjectVector& objects, JSON::ValuePtr config);

        /**
        * Yields to the subOACs and does nothing else.
        */
        virtual void run();

        /**
        * Checks whether the final state has been reached.
        *
        * @return True as soon as yielding to the subOACs was completed.
        */
        virtual bool isFinished() const;

    protected:
        bool yielded;
        CEAControlWeakPtr weakCEA;
        std::vector<OACPtr> oacs;

        static Register<SuperAction> r;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<SuperAction> SuperActionPtr;
}

#endif
