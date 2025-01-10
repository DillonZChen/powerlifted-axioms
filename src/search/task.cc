#include "task.h"

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void Task::add_predicate(
    string &name, int index, int arity, bool static_predicate, vector<int> &types)
{
    predicates.emplace_back(std::move(name), index, arity, static_predicate, std::move(types));
}

void Task::add_object(const string &name, int index, const vector<int> &types)
{
    objects.emplace_back(name, index, types);
}

void Task::add_type(const string &type_name) { type_names.push_back(type_name); }

void Task::create_empty_initial_state(size_t number_predicates, int num_objects)
{
    /*
     * Creates one empty relation for every predicate of the task in the initial
     * state.
     */
    vector<Relation> fluents, static_preds;
    for (size_t i = 0; i < predicates.size(); ++i) {
        Relation r;
        r.predicate_symbol = i;
        assert(r.tuples.empty());
        static_preds.push_back(r);
        fluents.push_back(r);
    }
    initial_state =
        DBState(std::move(fluents), vector<bool>(predicates.size(), false), num_objects);
    static_info = StaticInformation(
        std::move(static_preds), vector<bool>(predicates.size(), false), num_objects);
}

void Task::dump_state(DBState s) const
{
    /*
     * Output initial state in a human readable way.
     */
    const auto &nullary_atoms = s.get_nullary_atoms();
    for (size_t j = 0; j < nullary_atoms.size(); ++j) {
        if (nullary_atoms[j])
            cout << predicates[j].get_name() << ", ";
    }
    const auto &relations = s.get_relations();
    for (size_t i = 0; i < relations.size(); ++i) {
        string relation_name = predicates[i].get_name();
        unordered_set<GroundAtom, TupleHash> tuples = relations[i].tuples;
        for (auto &tuple : tuples) {
            cout << relation_name << "(";
            for (auto obj : tuple) {
                cout << obj << ',';
                // cout << objects[obj].get_name() << ",";
            }
            cout << "), ";
        }
    }
    cout << endl;
}

void Task::dump_goal() const
{
    /*
     * Output goal condition in a readable format.
     */
    for (int g : goal.positive_nullary_goals) {
        cout << predicates[g].get_name() << endl;
    }
    for (int g : goal.negative_nullary_goals) {
        cout << "Not " << predicates[g].get_name() << endl;
    }
    for (const auto &g : goal.goal) {
        if (g.is_negated()) {
            cout << "Not ";
        }
        cout << predicates[g.get_predicate_index()].get_name() << " ";
        for (int arg : g.get_arguments()) {
            cout << objects[arg].get_name() << " ";
        }
        cout << endl;
    }
}


void Task::dump_axioms() const
{
    /*
     * Output axioms in a readable format.
     */
    cout << "{{BEGIN_DUMP Axioms}}" << endl;
    std::vector<std::string> p_names;
    for (const auto &p : predicates) {
        p_names.push_back(p.get_name());
    }
    for (size_t i = 0; i < stratified_axioms.size(); i++) {
        cout << "STRATUM " << i << endl;
        for (const auto &axiom : stratified_axioms[i]) {
            datalog::DatalogAtom head = axiom->get_effect();
            std::vector<datalog::DatalogLiteral> body = axiom->get_conditions();
            cout << head.to_string(p_names) << " :- ";
            for (size_t i = 0; i < body.size(); ++i) {
                if (body[i].is_negated()) {
                    cout << "!";
                }
                cout << body[i].get_atom().to_string(p_names);
                if (i != body.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
    cout << "{{END_DUMP Axioms}}" << endl;
}


void Task::create_goal_condition(std::vector<AtomicGoal> goals,
                                 std::unordered_set<int> nullary_goals,
                                 std::unordered_set<int> negative_nullary_goals)
{
    goal = GoalCondition(
        std::move(goals), std::move(nullary_goals), std::move(negative_nullary_goals));
}

void Task::initialize_action_schemas(const std::vector<ActionSchema> &action_list)
{
    action_schemas = action_list;
}

void Task::initialize_axioms(std::vector<Axiom> &axioms)
{
    /*
     * Checks whether axioms can be stratified and stratifies them.
     * Uses Algorithm 1 from Thiebaux et al., IJCAI-03
     *
     * Finishes by extending the initial state with axioms
     */

    // collect derived predicates
    std::unordered_map<int, int> derived_predicate_indices;
    for (const auto &axiom : axioms) {
        const datalog::DatalogAtom &head = axiom->get_effect();
        int head_predicate = head.get_predicate_index();
        if (predicates.at(head_predicate).isStaticPredicate()) {
            continue;
        }
        if (derived_predicate_indices.count(head_predicate)) {
            continue;
        }
        derived_predicate_indices[head_predicate] = derived_predicate_indices.size();
    }
    int n = derived_predicate_indices.size();

    // init stratification matrix
    std::vector<std::vector<int>> R(n, std::vector<int>(n, 0));

    // init values
    for (const auto &axiom : axioms) {
        int j = derived_predicate_indices[axiom->get_effect().get_predicate_index()];
        for (const auto &literal : axiom->get_conditions()) {
            int predicate = literal.get_predicate_index();
            if (!derived_predicate_indices.count(predicate)) {
                continue;
            }
            int i = derived_predicate_indices.at(predicate);
            if (literal.is_negated()) {
                R[i][j] = 2;
            }
            else {
                R[i][j] = std::max(R[i][j], 1);
            }
        }
    }

    // recurse
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (std::min(R[i][j], R[j][k]) > 0) {
                    R[i][k] = std::max(R[i][j], std::max(R[j][k], R[i][k]));
                }
            }
        }
    }

    // check if stratification is possible
    for (int i = 0; i < n; ++i) {
        if (R[i][i] == 2) {
            cout << "Error: Axioms cannot be stratified!" << endl;
            exit(1);
        }
    }

    // derived predicate indices to task predicate indices
    std::vector<int> derived_to_task(n);
    for (const auto &entry : derived_predicate_indices) {
        derived_to_task.at(entry.second) = entry.first;
    }

    // stratify predicates
    std::unordered_map<int, int> predicate_stratification;
    std::unordered_set<int> remaining;
    int level = 0;
    for (int i = 0; i < n; ++i) {
        remaining.insert(i);
    }
    while (!remaining.empty()) {
        std::vector<int> to_remove;

        for (const auto &j : remaining) {
            bool add = true;
            for (const auto &i : remaining) {
                if (R[i][j] == 2) {
                    add = false;
                    break;
                }
            }
            if (add) {
                predicate_stratification[derived_to_task[j]] = level;
                to_remove.push_back(j);
            }
        }

        // remaining = remaining \ stratum_derived_indices
        for (const auto &j : to_remove) {
            remaining.erase(j);
        }

        level++;
    }

    // stratify axioms
    stratified_axioms = std::vector<std::vector<Axiom>>(level);
    for (int i = 0; i < level; i++) {
        stratified_axioms[i] = std::vector<Axiom>();
    }
    for (const auto &axiom : axioms) {
        int level = predicate_stratification[axiom->get_effect().get_predicate_index()];
        stratified_axioms[level].push_back(axiom);
    }

    dump_axioms();
    // TODO extend initial state with axioms
}

void Task::initialize_axiom_evaluator() {
  axioms_evaluator.set_axioms(stratified_axioms);
  return;
}

bool Task::is_goal(const DBState &state) const
{
    for (int pred : goal.positive_nullary_goals) {
        if (!state.get_nullary_atoms()[pred])
            return false;
    }
    for (int pred : goal.negative_nullary_goals) {
        if (state.get_nullary_atoms()[pred])
            return false;
    }
    for (const AtomicGoal &atomicGoal : goal.goal) {
        int goal_predicate = atomicGoal.get_predicate_index();
        const Relation &relation_at_goal_predicate = state.get_relations()[goal_predicate];

        assert(!predicates[relation_at_goal_predicate.predicate_symbol].isStaticPredicate());
        assert(goal_predicate == relation_at_goal_predicate.predicate_symbol);

        const auto it = relation_at_goal_predicate.tuples.find(atomicGoal.get_arguments());
        const auto end = relation_at_goal_predicate.tuples.end();
        if ((!atomicGoal.is_negated() && it == end) || (atomicGoal.is_negated() && it != end)) {
            return false;
        }
    }
    return true;
}

bool Task::is_trivially_unsolvable() const
{
    /*
     * Checks whether the static conditions in the goal condition are not
     * satisfied.
     *
     * This should be guaranteed by the translator. Just a safety check.
     */
    for (const AtomicGoal &atomicGoal : goal.goal) {
        int goal_predicate = atomicGoal.get_predicate_index();
        Relation relation_at_goal_predicate = static_info.get_relations()[goal_predicate];
        if (!predicates[relation_at_goal_predicate.predicate_symbol].isStaticPredicate())
            continue;
        assert(goal_predicate == relation_at_goal_predicate.predicate_symbol);

        const auto it = relation_at_goal_predicate.tuples.find(atomicGoal.get_arguments());
        const auto end = relation_at_goal_predicate.tuples.end();
        if ((!atomicGoal.is_negated() && it == end) || (atomicGoal.is_negated() && it != end)) {
            return true;
        }
    }
    return false;
}


std::vector<std::vector<int>> Task::compute_object_index() const
{
    std::vector<std::vector<int>> objects_per_type(type_names.size());

    for (const Object &o : objects) {
        for (int t : o.get_types()) {
            objects_per_type[t].push_back(o.get_index());
        }
    }

    return objects_per_type;
}
