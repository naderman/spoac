import sys
from spoac import SymbolicExecutionSlice

from spoac.util.IceApplication import IceApplication

class CEAController(SymbolicExecutionSlice.CEAControllerTopic):
    """
    Very simple CEAController implementation which simply
    always does exactly what it is told.
    """

    def startAction(self, action, current = None):
        print "start"
        print action

        #topic = self.app.stormGetTopic("CEAInfo",
        #    SymbolicExecutionSlice.CEAInfoTopic)
        #topic.actionStarted(action)

    def stopAction(self, action, current = None):
        print "stop"
        print action

    def taskComplete(self, current = None):
        print "taskCompleted"

    def pause(self, current = None):
        print "pause"

    def reset(self, current = None):
        print "reset"

class CEAControllerApp(IceApplication):
    def run(self, args):

        cea = CEAController()

        self.stormSubscribeTopic(cea, "CEAController", "default -p 10001")
        self.waitForShutdown()

        return 0

cea = CEAControllerApp()
status = cea.main(sys.argv)
sys.exit(status)
