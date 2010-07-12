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

#ifndef SPOAC_CEA_CEA_H
#define SPOAC_CEA_CEA_H

#include <spoac/common/DependencyManager.h>
#include <spoac/cea/CEAControl.h>
#include <spoac/cea/ActivityController.h>
#include <spoac/ice/IceHelper.h>

#include <stack>
#include <functional>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <IceUtil/Mutex.h>

namespace spoac
{
    /**
    * The robot's central executive agent
    *
    * Asks ActivityControllers which action they want to execute and decides
    * which one is actually run.
    */
    class CEA : public CEAControl, public boost::enable_shared_from_this<CEA>
    {
    public:
        /**
        * Creates an instance of this class with the right dependencies.
        *
        * @param manager The DependencyManager providing necessary dependencies.
        */
        static boost::shared_ptr<void> createService(
            DependencyManagerPtr manager);

        /**
        * Constructor storing dependencies
        *
        * @param stm                   A pointer to the robot's short term
        *                              memory
        * @param weakDependencyManager Weak pointer to dependency manager,
        *                              avoiding circular referencing.
        */
        CEA(STMPtr stm, DependencyManagerWeakPtr weakDependencyManager);

        /**
        * Adds an activity controller which is regularly queried for new actions
        *
        * @param activityController The controller providing actions to execute
        */
        void addActivityController(ActivityControllerPtr activityController);

        /**
        * Adds an activity controller identified by its name.
        *
        * @param controllerName    The identifier for the controller class
        * @param dependencyManager Required to satisfy new class's dependencies
        */
        void addActivityController(
            std::string controllerName,
            DependencyManagerPtr dependencyManager
        );

        /**
        * Throws away all activity controllers.
        */
        void clearActivityControllers();

        /**
        * Clears the controller list and then adds all the given ones by name.
        *
        * @param handlerNames A vector of Pactivity controler class identifiers
        * @param dependencyManager Required to satisfy new classes' dependencies
        */
        void setActivityControllers(
            std::vector<std::string> controllerNames,
            DependencyManagerPtr dependencyManager
        );

        /**
        * Starts a particular OAC's execution.
        *
        * @param src The instructing ActivityController.
        * @param oac The OAC which shall be started
        */
        virtual void startOAC(ActivityControllerPtr src, OACPtr oac);

        /**
        * Stops a particular OAC's execution.
        *
        * @param src The instructing ActivityController.
        * @param oac The OAC which needs to be stopped.
        */
        virtual void stopOAC(ActivityControllerPtr src, OACPtr oac);

        /**
        * Informs the CEA about the completion of a task.
        *
        * @param src The ActivityController which completed its task.
        */
        virtual void taskCompleted(ActivityControllerPtr src);

        /**
        * Asks the CEA to pause the robot immediately.
        *
        * @param src The ActivityController which made the request.
        */
        virtual void pause(ActivityControllerPtr src);

        /**
        * Asks the CEA to unpause the robot, when it has been paused.
        *
        * @param src The ActivityController which made the request.
        */
        virtual void unpause(ActivityControllerPtr src);

        /**
        * Asks the CEA to reset the robot immediately.
        *
        * @param src The ActivityController which made the request.
        */
        virtual void reset(ActivityControllerPtr src);

        /**
        * Getter for the currently executing OAC
        */
        virtual OACPtr getCurrentOAC();

        /**
        * Execute a step of an action on the robot and/or perception.
        */
        virtual void run();

        /**
        * Tell the CEA to load a particular scenario from long term memory.
        *
        * @param scenario The name of the scenario to be loaded.
        */
        virtual void setScenario(const std::string& scenario);

        /**
        * Define a goal for the CEA as a PKS expression.
        *
        * @param goalExpression A PKS expression that should evaluate to true.
        */
        virtual void setGoalExpression(const std::string& goalExpression);

        /**
        * Select a named goal from the scenario as the goal.
        *
        * @param goalName The name of the goal to be reached.
        */
        virtual void setGoalName(const std::string& goalName);

    protected:
        /**
        * Base class for all notifiers
        */
        class Notifier
        {
        public:
            virtual void operator()(ActivityControllerPtr c) {};
        };

        /**
        * Notifier functor for pausing ActivityControllers
        */
        class RequestActionNotifier : public Notifier
        {
        public:
            virtual void operator()(ActivityControllerPtr c)
            {
                c->requestAction();
            }
        };

        /**
        * Notifier functor for pausing ActivityControllers
        */
        class ResetNotifier : public Notifier
        {
        public:
            virtual void operator()(ActivityControllerPtr c)
            {
                c->reset();
            }
        };

        /**
        * Notifier functor for pausing ActivityControllers
        */
        class PauseNotifier : public Notifier
        {
        public:
            virtual void operator()(ActivityControllerPtr c)
            {
                c->pause();
            }
        };

        /**
        * Notifier functor for unpausing ActivityControllers
        */
        class UnpauseNotifier : public Notifier
        {
        public:
            virtual void operator()(ActivityControllerPtr c)
            {
                c->unpause();
            }
        };

        /**
        * Notifier functor for informing ActivityControllers about OAC states.
        */
        class OACNotifier : public Notifier
        {
        public:
            OACNotifier(OACPtr oac) : oac(oac) {};
        protected:
            OACPtr oac;
        };

        /**
        * Notifier functor for informing ActivityControllers about started OACs
        */
        class StartedNotifier : public OACNotifier
        {
        public:
            StartedNotifier(OACPtr oac) : OACNotifier(oac) {};
            virtual void operator()(ActivityControllerPtr c)
            {
                c->oacStarted(oac);
            }
        };

        /**
        * Notifier functor for informing ActivityControllers about finished OACs
        */
        class FinishedNotifier : public OACNotifier
        {
        public:
            FinishedNotifier(OACPtr oac) : OACNotifier(oac) {};
            virtual void operator()(ActivityControllerPtr c)
            {
                c->oacFinished(oac);
            }
        };

        /**
        * Notifier functor for informing ActivityControllers about stopped OACs
        */
        class StoppedNotifier : public OACNotifier
        {
        public:
            StoppedNotifier(OACPtr oac) : OACNotifier(oac) {};
            virtual void operator()(ActivityControllerPtr c)
            {
                c->oacStopped(oac);
            }
        };

        /**
        * Notifier functor for informing ActivityControllers about goal changes.
        */
        class GoalNotifier : public Notifier
        {
        public:
            GoalNotifier(const std::string& goal) : goal(goal) {};
            virtual void operator()(ActivityControllerPtr c)
            {
                c->setGoalExpression(goal);
            }
        protected:
            std::string goal;
        };

        /**
        * Notifies all ActivityControllers with the given notifier instance.
        *
        * @param notifier The notifier to be applied on every controller.
        */
        void notifyActivityControllers(Notifier* notifier);

        /**
        * Retrieve the next action from the stack for execution
        */
        void nextAction();

        STMPtr stm;
        DependencyManagerWeakPtr weakDependencyManager;

        bool paused;

        std::vector<ActivityControllerPtr> activityControllers;
        std::stack<OACPtr> plannedOACs;
        ActionPtr runningAction;
        OACPtr runningOAC;

        std::map<std::string, std::string> goals;

        IceUtil::Mutex mutex;

        typedef DependencyManager::RegisterService<CEA> RegisterService;
        static RegisterService r;
    };

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::shared_ptr<CEA> CEAPtr;

    /**
    * Pointer type to reduce typing for shared pointers.
    */
    typedef boost::weak_ptr<CEA> CEAWeakPtr;
}

#endif

