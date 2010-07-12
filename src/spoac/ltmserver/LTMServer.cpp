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

#include <spoac/ltm/LTM.h>
#include <spoac/common/DependencyManager.h>

#include <Ice/Ice.h>

using namespace spoac;

class LTMServer : virtual public Ice::Application
{
public:
    virtual int run(int, char*[])
    {
        DependencyManagerPtr manager(new DependencyManager());

        Ice::ObjectAdapterPtr adapter =
            communicator()->createObjectAdapterWithEndpoints(
                "LTM", "tcp -p 10099");
        Ice::ObjectPtr object = new LTM;
        adapter->add(object, communicator()->stringToIdentity("LTM"));
        adapter->activate();

        communicator()->waitForShutdown();

        return 0;
    }
};

int
main(int argc, char* argv[])
{
    LTMServer app;
    return app.main(argc, argv);
}
