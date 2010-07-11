import sys
from spoac import SymbolicExecutionSlice
from spoac import PlanningSlice

from spoac.util.IceApplication import IceApplication

class PlanTesterApp(IceApplication):
    def run(self, args):

        planner = self.stormGetTopic("PlanController",
            PlanningSlice.PlanControllerTopicPrx)

        symbolDefinition = PlanningSlice.SymbolDefinition()

        pred1 = PlanningSlice.PredicateDefinition()
        pred1.name = "pred1"
        pred1.arguments = 1

        symbolDefinition.predicates = [pred1]

        function1 = PlanningSlice.FunctionDefinition()
        function1.name = "function1"
        function1.arguments = 0

        symbolDefinition.functions = [function1]

        symbolDefinition.constants = ["obj1", "obj2"]

        print "setSymbolDefinitions"
        planner.setSymbolDefinitions(symbolDefinition)

        action1 = PlanningSlice.ActionDefinition()
        action1.name = "action1"

        action1parameter1 = PlanningSlice.ActionParameter()
        action1parameter1.name = "x"
        action1parameter2 = PlanningSlice.ActionParameter()
        action1parameter2.name = "y"

        action1.parameters = [action1parameter1, action1parameter2]
        action1.precondition = "K(pred1(?x)) & K(pred1(?y)) & K(function1 = ?x)"
        action1.effect = "add(Kf, function1 = ?y)"

        actionDefinitions = [action1]

        print "setActionDefinitions"
        planner.setActionDefinitions(actionDefinitions)

        state1 = PlanningSlice.StateUpdate()

        predInst1 = PlanningSlice.PredicateInstance()
        predInst1.name = "pred1"
        predInst1.parameters = ["obj1"]
        predInst1.value = True

        predInst2 = PlanningSlice.PredicateInstance()
        predInst2.name = "pred1"
        predInst2.parameters = ["obj2"]
        predInst2.value = True

        state1.knownPredicates = [predInst1, predInst2]
        state1.unknownPredicates = []

        funcValue1 = PlanningSlice.FunctionValue()
        funcValue1.name = "function1"
        funcValue1.parameters = []
        funcValue1.realValue = 0.0
        funcValue1.constantValue = "obj1"

        state1.knownFunctionValues = [funcValue1]
        state1.unknownFunctionValues = []

        print "updateState"
        planner.updateState(state1)

        goal = PlanningSlice.Goal()
        goal.goalExpression = "K(function1 = obj2)"

        print "setGoal"
        planner.setGoal(goal)

        print "startPlan"
        planner.startPlan()

        return 0

planTester = PlanTesterApp()
status = planTester.main(sys.argv)
sys.exit(status)
