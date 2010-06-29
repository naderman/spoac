import sys
from spoac import SymbolicExecutionSlice
from spoac import PlanningSlice

from spoac.util.IceApplication import IceApplication

class PlanTesterApp(IceApplication):
    def run(self, args):

        planner = self.stormGetTopic("PlanController",
            PlanningSlice.PlanControllerTopicPrx)

        symbolDefinition = PlanningSlice.SymbolDefinition()
        actionDefinition = PlanningSlice.ActionDefinition()
        actionDefinitions = []
        goal = PlanningSlice.Goal()

        state1 = PlanningSlice.StateUpdate()

        print "setSymbolDefinitions"
        planner.setSymbolDefinitions(symbolDefinition)

        print "setActionDefinitions"
        planner.setActionDefinitions(actionDefinitions)

        print "setGoal"
        planner.setGoal(goal)

        print "updateState"
        planner.updateState(state1)

        print "startPlan"
        planner.startPlan()

        return 0

planTester = PlanTesterApp()
status = planTester.main(sys.argv)
sys.exit(status)
