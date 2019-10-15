#ifndef SEARCH_TASK_H
#define SEARCH_TASK_H

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

#include "predicate.h"
#include "object.h"
#include "state.h"
#include "goal_condition.h"
#include "action_schema.h"


/*
 * The initial state and the static info have info for all predicates. However, the static predicates
 * will always be empty in any state and the fluents are always empty in the static_info variable.
 */

class Task {

public:

    std::vector<Predicate> predicates;
    std::vector<Object> objects;
    State initial_state;
    StaticInformation static_info;
    GoalCondition goal;
    std::vector<ActionSchema> actions;
    std::vector<std::string> type_names;
    std::unordered_set<int> nullary_predicates;

    Task(const std::string& domain_name, const std::string& task_name) :
            domain_name(domain_name), task_name(task_name) {
        // Create class only with task and domain names
    }

    const std::string &getDomainName() const {
        return domain_name;
    }

    const std::string &getTaskName() const {
        return task_name;
    }

    const void addType(const std::string& type_name);

    const void
    addPredicate(std::string &name, int index, int arity, bool static_predicate, std::vector<int> &types);

    const void addObject(const std::string& name, int index, const std::vector<int>& types);

    void initializeEmptyInitialState();

    void initializeGoal(std::vector<AtomicGoal> goals,
                        std::unordered_set<int> nullary_goals,
                        std::unordered_set<int> negative_nullary_goals);

    void initializeActionSchemas(const std::vector<ActionSchema> &action_list);

    void dumpState(State s) const;

    void dumpGoal();

    bool is_goal(const State &state, const GoalCondition &goal_condition) const;

    bool is_trivially_unsolvable() const;

private:
    const std::string &domain_name;
    const std::string &task_name;
};


#endif //SEARCH_TASK_H
