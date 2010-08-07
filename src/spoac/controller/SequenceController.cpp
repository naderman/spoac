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

#include <spoac/controller/SequenceController.h>

using namespace spoac;
using namespace spoac::controller;

SequenceController::SequenceController(
    CEAControlWeakPtr cea,
    std::vector<OACPtr> oacs) :
    ActivityController(cea),
    oacs(oacs)
{
    reset();
}

void SequenceController::requestAction()
{
    if (currentOAC == oacs.end())
    {
        // finished!
        if (spoac::CEAControlPtr cea = weakCEA.lock())
        {
            cea->taskCompleted(shared_from_this());
        }
        return;
    }

    if (spoac::CEAControlPtr cea = weakCEA.lock())
    {
        cea->startOAC(shared_from_this(), *currentOAC);
    }

    currentOAC++;
}

void SequenceController::pause()
{
}

void SequenceController::unpause()
{
}

void SequenceController::reset()
{
    currentOAC = oacs.begin();
}

