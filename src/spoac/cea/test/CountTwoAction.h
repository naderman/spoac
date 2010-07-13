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
    class CountTwoAction : public spoac::Action
    {
    public:
        static int counter;

        virtual void setup(
            const spoac::ObjectVector& objects,
            JSON::ValuePtr config)
        {
            counter = 0;
        }

        virtual void run()
        {
            counter++;
        }

        virtual bool isFinished() const
        {
            return counter >= 2;
        }

        static std::string getName()
        {
            return "CountTwo";
        }

        static boost::shared_ptr<Action> createInstance(
            spoac::DependencyManagerPtr manager
        )
        {
            boost::shared_ptr<Action> action(new CountTwoAction);
            return action;
        }

        static Register<CountTwoAction> r;
    };
    CountTwoAction::Register<CountTwoAction> CountTwoAction::r;
    int CountTwoAction::counter = 4;
}
