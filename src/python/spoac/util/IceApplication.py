import Ice
import IceStorm

class IceApplication(Ice.Application):
    """
    A wrapper class around Ice.Application providing a bunch of convenience
    methods.
    """

    def __init__(self, signalPolicy = 0):
        """
        IceApplication constructor only calls parent constructor and sets up
        some attributes.

        @param signalPolicy int 0 if Ice.Application shall handle signals else 1
        """

        Ice.Application.__init__(self, signalPolicy)
        self.subscriptions = []
        self.topics = {}
        self.topicManager = None
        self.proxy = {}

    def stormTopicRetrieve(self, topicName):
        """
        Retrieves a topic from the topic manager or creates it if necessary

        @param topicName The name of the topic to be retrieved or created
        """
        if self.topicManager == None:
            obj = self.communicator().stringToProxy(
                "IceStorm/TopicManager:tcp -p 10000")
            self.topicManager = IceStorm.TopicManagerPrx.checkedCast(obj)

        topic = None

        try:
            topic = self.topicManager.retrieve(topicName)
        except IceStorm.NoSuchTopic:
            try:
                topic = self.topicManager.create(topicName)
            except IceStorm.TopicExists:
                # someone else created it
                pass

        return topic


    def stormGetTopic(self, topicName, proxyType):
        """
        Returns an IceStorm topic object one can publish to.

        @param topicName The name of the IceStorm topic
        @param proxyType The proxy (Prx) class for the remote object
        """
        if not self.proxy.has_key(topicName):

            topic = self.stormTopicRetrieve(topicName)
            pub = topic.getPublisher().ice_oneway()

            specificTopic = proxyType.uncheckedCast(pub)

            self.proxy[topicName] = specificTopic

        return self.proxy[topicName]

    def stormSubscribeTopic(self, subscriber, topicName, endpoints):
        """
        Subscribes a server adapter object to an IceStorm topic.

        @param subscriber  object The subscribing interface implementation
        @param topicName   string Name of the topic and the adapter.
        @param endpoints   The endpoints for this particular adapter/topic.
        """

        topic = self.stormTopicRetrieve(topicName)

        adapter = self.communicator().createObjectAdapterWithEndpoints(
            topicName, endpoints)
        proxy = adapter.addWithUUID(subscriber).ice_oneway()

        qos = None
        topic.subscribeAndGetPublisher(qos, proxy)

        adapter.activate()

        # save the subscription so we can automatically unsubscribe later
        self.subscriptions.append({"topic": topic, "proxy": proxy})

        # make sure unsubscribe is called later
        self.callbackOnInterrupt()

    def stormUnsubscribeAll(self):
        """
        Unsubscribes this application from all IceStorm subscriptions.
        """

        for subscription in self.subscriptions:
            subscription["topic"].unsubscribe(subscription["proxy"])

    def interruptCallback(self, sig):
        """
        Calls custom functions first, then destroys the communicator.

        All non-custom code copied from destroyOnInterruptCallback in Ice.
        """
        self.stormUnsubscribeAll()

        self._condVar.acquire()
        if self._destroyed or self._nohup and sig == signal.SIGHUP:
            #
            # Being destroyed by main thread, or nohup.
            #
            self._condVar.release()
            return

        self._callbackInProcess = True
        self._interrupted = True
        self._destroyed = True
        self._condVar.release()

        try:
            self._communicator.destroy()
        except:
            msg = self._appName + " (while destroying in response to signal"
            msg = msg + str(sig) + "):"
            print msg
            traceback.print_exc()

        self._condVar.acquire()
        self._callbackInProcess = False
        self._condVar.notify()
        self._condVar.release()

    def waitForShutdown(self):
        """
        Wrapper function around the Ice Communicator's waitForShutdown() method.
        """

        self.communicator().waitForShutdown()
