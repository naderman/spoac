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

#include <spoac/cea/SuperAction.h>

using namespace spoac;

SuperAction::Register<SuperAction> SuperAction::r;

SuperAction::SuperAction(CEAControlWeakPtr cea) :
    yielded(false),
    weakCEA(cea)
{
}

void SuperAction::setup(
    const ObjectVector& objects,
    JSON::ValuePtr config)
{
    if (config->getType() != JSON::ARRAY)
    {
        throw Exception("SuperAction config is not a JSON array");
    }

    std::string oacName;
    std::vector<std::string> oacObjIds;
    JSON::ValuePtr oacParams;
    JSON::Array::const_iterator it;
    JSON::Array& array = config->toArray();

    for (it = array.begin(); it != array.end(); ++it)
    {
        if ((*it)->getType() != JSON::OBJECT)
        {
            throw Exception(
                "SuperAction config array contains non-object element"
            );
        }

        JSON::Object& object = (*it)->toObject();
        oacName = object["name"]->toString();
        oacObjIds.clear();
        oacParams = object["params"];

        if (oacParams->getType() != JSON::ARRAY)
        {
            throw Exception(std::string("SuperAction parameters for OAC ") +
                oacName + std::string("are not an array of numbers"));
        }

        JSON::Array& paramArray = oacParams->toArray();
        JSON::Array::const_iterator param;

        for (param = paramArray.begin(); param != paramArray.end(); ++param)
        {
            int paramOffset = (*param)->toInt();
            oacObjIds.push_back(objects.getValidated(paramOffset)->getId());
        }

        OACPtr oac(new OAC(oacName, oacObjIds));
        oacs.push_back(oac);
    }
}

void SuperAction::run()
{
    if (spoac::CEAControlPtr cea = weakCEA.lock())
    {
        cea->yieldSubOACs(shared_from_this(), oacs);

        yielded = true;
    }
}

bool SuperAction::isFinished() const
{
    return yielded;
}
