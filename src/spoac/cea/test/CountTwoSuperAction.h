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

namespace spoactest
{
    class CountTwoSuperAction : public spoac::Action
    {
    public:
        static int counter;

        CountTwoSuperAction(spoac::CEAControlWeakPtr cea) : weakCEA(cea)
        {
            yielded = false;
        }

        virtual void setup(
            const spoac::ObjectVector& objects,
            JSON::ValuePtr config)
        {
        }

        virtual void run()
        {
            if (yielded)
            {
                throw "CountTwoSuperAction called with yielded == true";
            }

            if (spoac::CEAControlPtr cea = weakCEA.lock())
            {
                spoac::OACPtr oac1(
                    new spoac::OAC("CountTwo", std::vector<std::string>())
                );
                spoac::OACPtr oac2(
                    new spoac::OAC("CountTwo", std::vector<std::string>())
                );

                std::vector<spoac::OACPtr> oacs;
                oacs.push_back(oac1);
                oacs.push_back(oac2);

                cea->yieldSubOACs(shared_from_this(), oacs);
                yielded = true;
            }
        }

        virtual bool isFinished() const
        {
            return yielded;
        }

        static std::string getName()
        {
            return "CountTwoSuper";
        }

        static boost::shared_ptr<Action> createInstance(
            spoac::DependencyManagerPtr manager
        )
        {
            boost::shared_ptr<Action> action(new CountTwoSuperAction(
                spoac::CEAControlWeakPtr(manager->getService<spoac::CEA>())));
            return action;
        }

        spoac::CEAControlWeakPtr weakCEA;
        bool yielded;
        static Register<CountTwoSuperAction> r;
    };
    CountTwoSuperAction::Register<CountTwoSuperAction> CountTwoSuperAction::r;
}
