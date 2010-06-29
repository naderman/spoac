import sys
from spoac import SymbolicExecutionSlice

from spoac.util.IceApplication import IceApplication

class CommandSenderApp(IceApplication):
    def run(self, args):

        action = SymbolicExecutionSlice.Action()

        print "getTopic"
        topic = self.stormGetTopic("CEAController",
            SymbolicExecutionSlice.CEAControllerTopicPrx)
        print "startAction"
        print action
        topic.startAction(action)
        print "finished!"

        return 0

sender = CommandSenderApp()
status = sender.main(sys.argv)
sys.exit(status)
