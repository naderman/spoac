import sys
from spoac import SymbolicExecutionSlice

from spoac.util.IceApplication import IceApplication

class ControllerListener(SymbolicExecutionSlice.CEAControllerTopic):
    """
    Listens to the CEAControllerTopic and prints the commands
    """

    def startAction(self, action, current = None):
        print "ControllerListener: startAction"
        print "ControllerListener: " + str(action)

    def stopAction(self, action, current = None):
        print "ControllerListener: stopAction"
        print "ControllerListener: " + str(action)

    def taskComplete(self, current = None):
        print "ControllerListener: taskComplete"

    def pause(self, current = None):
        print "ControllerListener: pause"

    def reset(self, current = None):
        print "ControllerListener: reset"

class ControllerListenerApp(IceApplication):
    def run(self, args):

        listener = ControllerListener()

        self.stormSubscribeTopic(listener, "CEAController", "default -p 10001")
        self.waitForShutdown()

        return 0

listener = ControllerListenerApp()
status = listener.main(sys.argv)
sys.exit(status)
