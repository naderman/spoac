#ifndef _SYMBOLIC_EXECUTION_SLICE
#define _SYMBOLIC_EXECUTION_SLICE

module SymbolicExecutionSlice
{
    struct Obj
    {
        string id;
    };
    sequence<Obj> ObjectList;

    struct Action
    {
        string name;
        ObjectList parameters;
    };
    sequence<Action> ActionList;


    // Interfaces

    /**
    * Publish on this topic to tell the Central Executive Agent what to do.
    * Subscribe to this topic to listen to what the CEA is told to do.
    */
    interface CEAControllerTopic {
        void startAction(Action a);
        void stopAction(Action a);
        void taskComplete();

        void pause();
        void reset();
    };

    /**
    * Subscribe to this topic, to receive information from the Central
    * Executive Agent about what it is doing.
    * Only the CEA should publish to this topic.
    */
    interface CEAInfoTopic {
        void actionStarted(Action a);
        void actionFinished(Action a);
        void actionStopped(Action a);
        void actionInterrupted(Action wasRunning, Action interruptedBy);

        void taskCompleted();

        void stateUpdated();

        void paused();
        void resetted();
    };

    /**
    * Use this interface to ask the Central Executive Agent about its current
    * state.
    * A typical use case would be to first initialise with this data, and then
    * subscribe to CEAInfoTopic for updates.
    */
    interface CEAInfo
    {
        idempotent Action getCurrentAction();
        idempotent bool isPaused();
    }
};

#endif
