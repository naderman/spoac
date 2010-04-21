import sys
import interfaces.python.SymbolicExecutionSlice

from Spoac.IceApplication import IceApplication

class CentralExecutiveAgentImpl(SymbolicExecutionSlice.CentralExecutiveAgent):
    def startAction(self, action, current = None):
        print "start"
        print action

    def stop(self, current = None):
        sys.exit(2)

    def taskCompleted(self, current = None):
        print "taskCompleted"

class CeaApplication(IceApplication):
    def run(self, args):

        cea = CentralExecutiveAgentImpl()

        self.stormSubscribeTopic(cea, "CentralExecutiveAgentAdapter", "default -p 10001")
        self.waitForShutdown()
        self.stormUnsubscribeAll()

        return 0

cea = CeaApplication()
status = cea.main(sys.argv)
sys.exit(status)
