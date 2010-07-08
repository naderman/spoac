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

#include <spoac/cea/CEA.h>

using namespace spoac;

CEA::RegisterService CEA::r;

boost::shared_ptr<void> CEA::createService(DependencyManagerPtr manager)
{
    boost::shared_ptr<void> cea(new CEA(
        manager->getService<STM>(),
        manager
    ));
    return cea;
}

CEA::CEA(STMPtr stm, DependencyManagerWeakPtr weakDependencyManager) :
    stm(stm),
    weakDependencyManager(weakDependencyManager),
    paused(false)
{
}

void CEA::addActivityController(ActivityControllerPtr activityController)
{
    activityControllers.push_back(activityController);
}

void CEA::addActivityController(
    std::string controllerName, DependencyManagerPtr dependencyManager)
{
    ActivityControllerPtr controller = ActivityController::fromName(
        controllerName,
        dependencyManager
    );

    addActivityController(controller);
}

void CEA::clearActivityControllers()
{
    activityControllers.clear();
}

void CEA::setActivityControllers(
    std::vector<std::string> controllerNames,
    DependencyManagerPtr dependencyManager)
{
    std::vector<std::string>::iterator it;

    clearActivityControllers();

    for (it = controllerNames.begin(); it != controllerNames.end(); ++it)
    {
        addActivityController(*it, dependencyManager);
    }
}

void CEA::startOAC(ActivityControllerPtr src, OACPtr oac)
{
    IceUtil::Mutex::Lock lock(mutex);

    plannedOACs.push(oac);
    paused = false;
}

void CEA::stopOAC(ActivityControllerPtr src, OACPtr oac)
{
    IceUtil::Mutex::Lock lock(mutex);

    StoppedNotifier n(runningOAC);
    notifyActivityControllers(&n);

    runningAction = ActionPtr();
    runningOAC = OACPtr();
}

void CEA::taskCompleted(ActivityControllerPtr src)
{
    IceUtil::Mutex::Lock lock(mutex);

    runningAction = ActionPtr();
    runningOAC = OACPtr();
}

void CEA::pause(ActivityControllerPtr src)
{
    PauseNotifier n;
    notifyActivityControllers(&n);
    paused = true;
}

void CEA::unpause(ActivityControllerPtr src)
{
    UnpauseNotifier n;
    notifyActivityControllers(&n);
    paused = false;
}

void CEA::reset(ActivityControllerPtr src)
{
    ResetNotifier n;
    notifyActivityControllers(&n);

    {
        IceUtil::Mutex::Lock lock(mutex);

        while (!plannedOACs.empty())
        {
            plannedOACs.pop();
        }
        runningAction = ActionPtr();
        paused = false;
    }
}

OACPtr CEA::getCurrentOAC()
{
    IceUtil::Mutex::Lock lock(mutex);

    return runningOAC;
}

void CEA::run()
{
    if (paused)
    {
        return;
    }

    stm->update();

    {
        IceUtil::Mutex::Lock lock(mutex);
        if (runningAction.get() != NULL)
        {
            if (!runningAction->isFinished())
            {
                runningAction->run();
            }
            else
            {
                lock.release();

                FinishedNotifier n(runningOAC);
                notifyActivityControllers(&n);

                lock.acquire();

                runningAction = ActionPtr();
                runningOAC = OACPtr();
            }
        }
        else
        {
            lock.release();
            nextAction();
        }
    }
}

void CEA::nextAction()
{
    IceUtil::Mutex::Lock lock(mutex);

    // no action -> try to find one
    if (plannedOACs.empty())
    {
        lock.release();

        RequestActionNotifier r;
        notifyActivityControllers(&r);

        lock.acquire();
    }

    // if we have an action now ...
    if (!plannedOACs.empty())
    {
        OACPtr nextOAC = plannedOACs.top();
        plannedOACs.pop();

        if (DependencyManagerPtr manager = weakDependencyManager.lock())
        {
            runningAction = nextOAC->setupAction(manager);
            runningOAC = nextOAC;

            lock.release();

            StartedNotifier n(runningOAC);
            notifyActivityControllers(&n);
        }
    }
}

void CEA::notifyActivityControllers(Notifier* notifier)
{
    std::vector<ActivityControllerPtr>::iterator it;

    for (
        it = activityControllers.begin();
        it != activityControllers.end();
        ++it)
    {
        (*notifier)(*it);
    }
}
