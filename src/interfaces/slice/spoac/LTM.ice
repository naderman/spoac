#ifndef _LTM_SLICE
#define _LTM_SLICE

#include <spoac/Planning.ice>

module spoac
{
    module LTMSlice
    {
        dictionary<string, string> PropertyMap;

        struct Obj
        {
            string id;
            PropertyMap properties;
        };
        sequence<Obj> ObjectList;

        struct OAC
        {
            string name;
            ObjectList objects;
        };

        sequence<string> NameList;
        dictionary<string, string> StringMap;

        struct Scenario
        {
            string name;

            NameList activityControllers;
            NameList perceptionHandlers;
            NameList oacs;

            PlanningSlice::PredicateDefinitionList predicates;
            PlanningSlice::FunctionDefinitionList functions;

            StringMap goals;
        };

        struct ActionConfig
        {
            string name;
            string config;
        };

        // Interfaces

        interface LTM {
            idempotent Scenario getScenario(string scenario);

            idempotent PlanningSlice::ActionDefinition getAction(string oac);

            ActionConfig getActionConfig(OAC oacInstance);
        };
    };
};

#endif
