#ifndef SEARCH_STRUCTURES_H
#define SEARCH_STRUCTURES_H

#include "hash_structures.h"

#include <string>
#include <utility>
#include <unordered_set>
#include <vector>

/**
 * @brief GroundAtom is an alias for vector of integers. It is represented
 * as a list of object indices.
 */
typedef std::vector<int> GroundAtom;


/**
 * @brief Represent a parameter for a given action schema.
 *
 * @var name: Name of the parameter in the action schema definition.
 * @var index: Index of this parameter in the list of parameters in the schema.
 * @var type: Type related to this parameter.
 */
struct Parameter {
    Parameter(std::string name, int index, int type)
            : name(std::move(name)), index(index), type(type) {}

    std::string name;
    int index;
    int type;

    int get_index() {
        return index;
    }
};


/**
 * @brief Implements an argument composing an atom. It can be a free
 * variable or a constant. If it is a variable, it can either be a fresh variable or
 * a state variable.
 *
 * @var index: If the argument is a constant, then it represents the index of the object,
 * otherwise it represents the index of the free variable in the parameters of the action
 * schema.
 * @var constant: Indicates whether the argument is a constanst or not (free variable, then).
 *
 */
class Argument {
    int index;
    bool constant;
    bool is_fresh;
    std::string name;
public:
    Argument(int index, bool constant, bool is_fresh) :
        index(index),
        constant(constant),
        is_fresh(is_fresh) {}

    int get_index() const {
        return index;
    }

    bool is_constant() const {
        return constant;
    }

    bool is_fresh_var() const {
        return is_fresh;
    }

};

class FreshVariable : public Argument {
    std::string name;
public:
    FreshVariable(std::string name, int index, bool constant) :
        Argument(index, constant, true),
        name(name){}

    const std::string &get_name() const {
        return name;
    }

    bool is_fresh_var() const {
        return true;
    }

};

/**
 * @brief A relation is a "table" with set of tuples corresponding to some
 * predicate in a state.
 *
 * @var predicate_symbol: Indicates its corresponding predicate.
 * @var tuples: Set of tuples (vectors) corresponding to the ground atoms in this relation.
 *
 */
struct Relation {
    Relation() = default;
    Relation(int predicate_symbol,
        std::unordered_set<GroundAtom, TupleHash> &&tuples)
            : predicate_symbol(predicate_symbol),
              tuples (std::move(tuples)) {}

    Relation(const Relation &) = default;


    bool operator==(const Relation &other) const {
        return predicate_symbol == other.predicate_symbol && tuples == other.tuples;
    }

    int predicate_symbol{};
    std::unordered_set<GroundAtom, TupleHash> tuples;
};

#endif //SEARCH_STRUCTURES_H
