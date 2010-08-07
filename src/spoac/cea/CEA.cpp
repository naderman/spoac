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
#include <spoac/LTM.h>

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
    paused(false),
    perceptionDisabled(false)
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

    if (controller.get() == NULL)
    {
        throw Exception(
            std::string("Could not create controller: ") + controllerName
        );
    }

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
    if (oac == OACPtr())
    {
        throw Exception("Cannot start an OAC with a null pointer.");
    }

    IceUtil::Mutex::Lock lock(mutex);

    plannedOACs.push_back(oac);
}

void CEA::injectOAC(ActivityControllerPtr src, OACPtr oac)
{
    if (oac == OACPtr())
    {
        throw Exception("Cannot inject an OAC with a null pointer.");
    }

    IceUtil::Mutex::Lock lock(mutex);

    plannedOACs.push_front(oac);
}

void CEA::stopOAC(ActivityControllerPtr src, OACPtr oac)
{
    IceUtil::Mutex::Lock lock(mutex);

    if (oac != runningOAC) // TODO: value compare instead?
    {
        throw Exception("Only the currently running OAC can be stopped");
    }

    StoppedNotifier n(runningOAC);
    notifyActivityControllers(&n);

    runningAction = ActionPtr();
    runningOAC = OACPtr();
}

void CEA::taskCompleted(ActivityControllerPtr src)
{
    IceUtil::Mutex::Lock lock(mutex);

    //runningAction = ActionPtr();
    //runningOAC = OACPtr();
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
            plannedOACs.pop_front();
        }
        runningAction = ActionPtr();
        paused = false;
    }

    perceptionDisabled = false;
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

    if (!perceptionDisabled)
    {
        stm->update();
    }

    {
        IceUtil::Mutex::Lock lock(mutex);
        if (runningAction.get() != NULL)
        {
            if (!runningAction->isFinished())
            {
                lock.release();

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

void CEA::setScenario(const std::string& scenario)
{
    if (DependencyManagerPtr manager = weakDependencyManager.lock())
    {
        ice::IceHelperPtr iceHelper = manager->getService<ice::IceHelper>();

        LTMSlice::LTMPrx ltm =
            iceHelper->getProxy<LTMSlice::LTMPrx>("LTM:tcp -p 10099");
        LTMSlice::Scenario s = ltm->getScenario(scenario);

        setActivityControllers(s.activityControllers, manager);
        stm->setScenario(s, manager);

        goals = s.goals;

        ScenarioNotifier n(s);
        notifyActivityControllers(&n);
    }
}

void CEA::setGoalExpression(const std::string& goalExpression)
{
    GoalNotifier n(goalExpression);
    notifyActivityControllers(&n);
}

void CEA::setGoalName(const std::string& goalName)
{
    std::string goalExpression = goals[goalName];
    // TODO: Throw exception if undefined
    setGoalExpression(goalExpression);
}

void CEA::disablePerception()
{
    perceptionDisabled = true;
}

void CEA::enablePerception()
{
    perceptionDisabled = false;
}

void CEA::yieldSubOACs(ActionPtr src, std::vector<OACPtr> oacs)
{
    IceUtil::Mutex::Lock lock(mutex);

    if (src != runningAction)
    {
        throw new Exception("Only the running Action can yield to sub OACs");
    }

    // insert empty pointer to force return
    plannedOACs.push_front(OACPtr());

    std::vector<OACPtr>::reverse_iterator it;
    for (it = oacs.rbegin(); it != oacs.rend(); ++it)
    {
        plannedOACs.push_front(*it);
    }

    blockedActions.push(runningAction);
    blockedOACs.push(runningOAC);

    runningAction = ActionPtr();
    runningOAC = OACPtr();
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

    // only continue if we have an action now
    if (plannedOACs.empty())
    {
        return;
    }

    // this is the next OAC
    OACPtr nextOAC = plannedOACs.front();

    // tell controllers about the next OAC
    lock.release();
    StartingNotifier n(nextOAC);
    notifyActivityControllers(&n);
    lock.acquire();

    // if an OAC was injected restart this process
    if (nextOAC != plannedOACs.front())
    {
        return;
    }

    plannedOACs.pop_front();

    // do we need to return from a sub OAC yield?
    if (nextOAC.get() == NULL)
    {
        runningAction = blockedActions.top();
        runningOAC = blockedOACs.top();

        blockedActions.pop();
        blockedOACs.pop();

        lock.release();
    }
    else if (DependencyManagerPtr manager = weakDependencyManager.lock())
    {
        runningAction = nextOAC->setupAction(manager);
        runningOAC = nextOAC;

        lock.release();

        StartedNotifier n(runningOAC);
        notifyActivityControllers(&n);
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
