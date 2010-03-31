#ifndef _PLANNING_SLICE
#define _PLANNING_SLICE

#include <spoac/SymbolicExecution.ice>

module spoac
{
    module PlanningSlice
    {
        sequence<string> ConstantList;
        sequence<string> ConstantDefinitionList;
        sequence<string> TypeNameList;

        struct PredicateDefinition
        {
            string name;
            short arguments;
        };
        sequence<PredicateDefinition> PredicateDefinitionList;

        struct FunctionDefinition
        {
            string name;
            short arguments;
        };
        sequence<FunctionDefinition> FunctionDefinitionList;

        struct TypeDefinition
        {
            string name;
            TypeNameList parents;
        };
        sequence<TypeDefinition> TypeDefinitionList;

        struct ActionParameter
        {
            string name;
            string type;
        };
        sequence<ActionParameter> ActionParameterList;

        struct ActionDefinition
        {
            string name;
            ActionParameterList parameters;
            string precondition;
            string effect;
        };
        sequence<ActionDefinition> ActionDefinitionList;

        struct SymbolDefinition
        {
            PredicateDefinitionList predicates;
            FunctionDefinitionList functions;
            TypeDefinitionList types;
            ConstantDefinitionList constants;
        };

        struct PredicateInstance
        {
            string name;
            ConstantList parameters;
            bool value;
        };
        sequence<PredicateInstance> PredicateInstanceList;

        struct FunctionValue
        {
            string name;
            ConstantList parameters;
            double value;
        };
        sequence<FunctionValue> FunctionValueList;

        struct Goal
        {
            string goalExpression;
        };

        struct StateUpdate
        {
            PredicateInstanceList knownPredicates;
            PredicateInstanceList unknownPredicates;
            FunctionValueList knownFunctionValues;
            FunctionValueList unknownFunctionValues;
        };


        // Interfaces


        /**
        * Publish to this topic to set up and instruct the planner.
        * Subscribe to this topic to listen to what the planner is told to do.
        */
        interface PlanControllerTopic
        {
            void setSymbolDefinitions(SymbolDefinition symbols);
            void setActionDefinitions(ActionDefinitionList actions);

            void setGoal(Goal g);
            void updateState(StateUpdate update);
            void actionFinished(SymbolicExecutionSlice::Action a);

            void startPlan();
            void stopPlan();

            void resetState();
        };

        /**
        * Use this interface to ask the Planner about its current
        * state.
        */
        interface PlannerInfo
        {
            idempotent SymbolicExecutionSlice::Action getCurrentAction();
            idempotent ActionDefinitionList getActionDefinitions();
            idempotent SymbolDefinition getSymbols();
            idempotent Goal getGoal();
            idempotent SymbolicExecutionSlice::ActionList getNextActions(int max);
        };
    };
};
#endif
