#include "parser.h"
#include "action_schema.h"
#include "goal_condition.h"
#include "task.h"

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

/**
 * For the format of the intermediate file produced by the PDDL translation,
 * check the comments of the translation source code.
 *
 * @param task
 * @param in
 * @return
 */

bool parse(Task &task, const ifstream &in)
{

    // String used to guarantee consistency throughout the parsing
    string canary;

    if (not is_sparse_representation(canary)) {
        return false;
    }

    int number_types;
    cin >> canary >> number_types;
    if (not is_next_section_correct(canary, "TYPES")) {
        return false;
    }
    cout << "Total number of types: " << number_types << endl;
    parse_types(task, number_types);

    int number_predicates;
    cin >> canary >> number_predicates;
    if (not is_next_section_correct(canary, "PREDICATES")) {
        return false;
    }
    cout << "Total number of predicates: " << number_predicates << endl;
    parse_predicates(task, number_predicates);


    int number_objects;
    cin >> canary >> number_objects;
    if (not is_next_section_correct(canary, "OBJECTS")) {
        return false;
    }
    cout << "Total number of objects: " << number_objects << endl;
    parse_objects(task, number_objects);


    int initial_state_size;
    cin >> canary >> initial_state_size;
    if (not is_next_section_correct(canary, "INITIAL-STATE")) {
        return false;
    }
    cout << "Total number of atoms in the initial state: " << initial_state_size << endl;
    task.create_empty_initial_state(task.predicates.size(), number_objects);
    parse_initial_state(task, initial_state_size);


    int goal_size;
    cin >> canary >> goal_size;
    if (not is_next_section_correct(canary, "GOAL")) {
        return false;
    }
    cout << "Total number of fluent atoms in the goal state: " << goal_size << endl;
    parse_goal(task, goal_size);


    int number_action_schemas;
    cin >> canary >> number_action_schemas;
    if (not is_next_section_correct(canary, "ACTION-SCHEMAS")) {
        return false;
    }
    cout << "Total number of action schemas: " << number_action_schemas << endl;
    parse_action_schemas(task, number_action_schemas);


    int number_axioms;
    cin >> canary >> number_axioms;
    if (not is_next_section_correct(canary, "AXIOMS")) {
        return false;
    }
    cout << "Total number of axioms: " << number_axioms << endl;
    parse_axioms(task, number_axioms);

    return true;
}

void parse_axioms(Task &task, int number_axioms)
{
    for (int i = 0; i < number_axioms; ++i) {
        string name;
        int head_index, num_pred_params, num_exist_params, num_body_atoms;
        cin >> name >> head_index >> num_pred_params >> num_exist_params >> num_body_atoms;
        assert(task.get_predicate_name(head_index) == name);

        // parameters
        vector<Parameter> predicate_parameters;
        vector<Parameter> existential_parameters;
        for (int j = 0; j < num_pred_params; ++j) {
            string param_name;
            int index, type;
            cin >> param_name >> index >> type;
            predicate_parameters.emplace_back(param_name, index, type);
        }
        for (int j = 0; j < num_exist_params; ++j) {
            string param_name;
            int index, type;
            cin >> param_name >> index >> type;
            existential_parameters.emplace_back(param_name, index, type);
        }

        // rule head
        // TODO construct rule head

        // rule body
        vector<datalog::DatalogLiteral> body;
        for (int j = 0; j < num_body_atoms; ++j) {
            string atom_name;
            int predicate_index, num_args;
            bool negated;
            cin >> atom_name >> predicate_index >> negated >> num_args;
            vector<int> arg_indices;
            for (int k = 0; k < num_args; ++k) {
                int arg_index;
                char parameter_type;
                cin >> parameter_type >> arg_index;
                arg_indices.push_back(arg_index);
            }
            // TODO add datalog atom
        }
    }
    std::vector<std::unique_ptr<datalog::RuleBase>> axioms;
    task.initialize_axioms(axioms);
}

void parse_action_schemas(Task &task, int number_action_schemas)
{
    vector<ActionSchema> actions;
    for (int i = 0; i < number_action_schemas; ++i) {
        string name;
        int cost, args, num_fresh_vars, precond_size, eff_size;
        cin >> name >> cost >> args >> num_fresh_vars >> precond_size >> eff_size;
        vector<Parameter> parameters;
        vector<FreshVariable> fresh_vars;
        vector<Atom> preconditions, static_preconditions, effects;
        vector<bool> positive_nul_precond(task.predicates.size(), false),
            negative_nul_precond(task.predicates.size(), false),
            positive_nul_eff(task.predicates.size(), false),
            negative_nul_eff(task.predicates.size(), false);
        for (int j = 0; j < args; ++j) {
            string param_name;
            int index, type;
            cin >> param_name >> index >> type;
            parameters.emplace_back(param_name, index, type);
        }
        if (num_fresh_vars > 0)
            task.flag_object_creation();
        for (int j = 0; j < num_fresh_vars; ++j) {
            string var_name;
            int index, type;
            cin >> var_name >> index >> type;
            fresh_vars.emplace_back(var_name, index, false);
        }
        for (int j = 0; j < precond_size; ++j) {
            string precond_name;
            int index;
            bool negated;
            int arguments_size;
            cin >> precond_name >> index >> negated >> arguments_size;
            if (arguments_size == 0) {
                assert(task.nullary_predicates.find(index) != task.nullary_predicates.end());
                if (!negated)
                    positive_nul_precond[index] = true;
                else
                    negative_nul_precond[index] = true;
            }
            else if (boost::iequals(precond_name, "=")) {
                int id1, id2;
                char c, d;
                cin >> c >> id1 >> d >> id2;

                vector<Argument> arguments;
                arguments.emplace_back(id1, c == 'c', false);
                arguments.emplace_back(id2, d == 'c', false);
                static_preconditions.emplace_back(
                    std::move(arguments), std::move(precond_name), index, negated);
            }
            else {
                vector<Argument> arguments;
                for (int k = 0; k < arguments_size; ++k) {
                    char c;
                    int obj_index;
                    cin >> c >> obj_index;
                    if (c == 'c') {
                        arguments.emplace_back(obj_index, true, false);
                    }
                    else if (c == 'p') {
                        arguments.emplace_back(obj_index, false, false);
                    }
                    else {
                        cerr << "Error while reading action schema " << name
                             << ". Argument is neither constant or "
                                "object"
                             << endl;
                        exit(-1);
                    }
                }
                preconditions.emplace_back(
                    std::move(arguments), std::move(precond_name), index, negated);
            }
        }
        for (int j = 0; j < eff_size; ++j) {
            string eff_name;
            int index;
            bool negated;
            int arguments_size;
            cin >> eff_name >> index >> negated >> arguments_size;
            vector<Argument> arguments;
            if (arguments_size == 0) {
                assert(task.nullary_predicates.find(index) != task.nullary_predicates.end());
                if (!negated)
                    positive_nul_eff[index] = true;
                else
                    negative_nul_eff[index] = true;
                continue;
            }
            for (int k = 0; k < arguments_size; ++k) {
                char c;
                int obj_index;
                cin >> c >> obj_index;
                if (c == 'c') {
                    arguments.emplace_back(obj_index, true, false);
                }
                else if (c == 'p') {
                    arguments.emplace_back(obj_index, false, false);
                }
                else if (c == 'f') {
                    arguments.emplace_back(obj_index, false, true);
                }
                else {
                    cerr << "Error while reading action schema " << name
                         << ". Argument is neither constant or "
                            "object"
                         << endl;
                    exit(-1);
                }
            }
            effects.emplace_back(std::move(arguments), std::move(eff_name), index, negated);
        }
        ActionSchema a(name,
                       i,
                       cost,
                       parameters,
                       fresh_vars,
                       preconditions,
                       effects,
                       static_preconditions,
                       positive_nul_precond,
                       negative_nul_precond,
                       positive_nul_eff,
                       negative_nul_eff);
        actions.push_back(a);
    }
    task.initialize_action_schemas(actions);
}

void parse_goal(Task &task, int goal_size)
{
    vector<AtomicGoal> goals;
    unordered_set<int> positive_nullary_goals, negative_nullary_goals;
    for (int i = 0; i < goal_size; ++i) {
        string name;
        int predicate_index;
        bool negated;
        int number_args;
        cin >> name >> predicate_index >> negated >> number_args;
        if (number_args == 0) {
            if (negated)
                negative_nullary_goals.insert(predicate_index);
            else
                positive_nullary_goals.insert(predicate_index);
            continue;
        }
        vector<int> args;
        copy_next_n_values(number_args, args);
        goals.emplace_back(predicate_index, args, negated);
    }
    task.create_goal_condition(goals, positive_nullary_goals, negative_nullary_goals);
}

void parse_initial_state(Task &task, int initial_state_size)
{
    // StaticInformation static_info(task.predicates.size());
    for (int i = 0; i < initial_state_size; ++i) {
        string name;
        int index;
        int predicate_index;
        bool negated;
        int number_args;
        cin >> name >> index >> predicate_index >> negated >> number_args;
        if (number_args == 0) {
            assert(task.nullary_predicates.find(predicate_index) != task.nullary_predicates.end());
            task.initial_state.set_nullary_atom(predicate_index, true);
        }
        vector<int> args;
        copy_next_n_values(number_args, args);
        if (!task.initial_state.get_nullary_atoms()[predicate_index]) {
            if (!task.predicates[predicate_index].isStaticPredicate())
                task.initial_state.add_tuple(predicate_index, args);
            else
                task.static_info.add_tuple(predicate_index, args);
        }
    }
    // task.set_static_info(static_info);
}

void parse_objects(Task &task, int number_objects)
{
    // Set number of objects
    task.initial_state.set_number_objects(number_objects);

    for (int i = 0; i < number_objects; ++i) {
        string name;
        int index;
        int n;
        cin >> name >> index >> n;
        vector<int> types;
        copy_next_n_values(n, types);
        task.add_object(name, index, types);
    }
}

void parse_predicates(Task &task, int number_predicates)
{
    for (int j = 0; j < number_predicates; ++j) {
        string predicate_name;
        int index;
        int number_args;
        bool static_pred;
        cin >> predicate_name >> index >> number_args >> static_pred;
        if (number_args == 0) {
            task.nullary_predicates.insert(index);
        }
        vector<int> types;
        copy_next_n_values(number_args, types);
        task.add_predicate(predicate_name, index, number_args, static_pred, types);
    }
}

void parse_types(Task &task, int number_types)
{
    for (int i = 0; i < number_types; ++i) {
        string type_name;
        int type_index;
        cin >> type_name >> type_index;
        task.add_type(type_name);
    }
}

bool is_sparse_representation(string &canary)
{
    cin >> canary;
    if (canary != "SPARSE-REPRESENTATION") {
        cerr << "Representation is not sparse. Not supported." << endl;
        return false;
    }
    return true;
}

bool is_next_section_correct(string &canary, const string &expected)
{
    if (canary != expected) {
        cerr << "Error while reading " << expected << " section." << endl;
        output_error(canary);
        return false;
    }
    return true;
}

void copy_next_n_values(int n, vector<int> &v)
{
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        v.push_back(x);
    }
}

void output_error(string &msg)
{
    cerr << "String read was \'" << msg << "\' instead of the respective canary." << endl;
}
