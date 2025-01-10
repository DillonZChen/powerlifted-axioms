#ifndef GROUNDER_LITERAL_H
#define GROUNDER_LITERAL_H

#include "datalog_atom.h"

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace datalog {

class DatalogLiteral {
    bool negated;
    DatalogAtom atom;

public:
    DatalogLiteral() = default;

    DatalogLiteral(DatalogAtom atom) : negated(false), atom(std::move(atom)) {}

    DatalogLiteral(bool negated, DatalogAtom atom) : negated(negated), atom(std::move(atom)) {}

    bool is_negated() const {
        return negated;
    }

    const DatalogAtom &get_atom() const {
        return atom;
    }

    int get_predicate_index() const {
        return atom.get_predicate_index();
    }

    const Arguments &get_arguments() const{
        return atom.get_arguments();
    }

    void update_arguments(std::vector<Term> &terms) {
        atom.update_arguments(terms);
    }

    void set_atom(DatalogAtom a) {
      atom = a;
    }

    friend bool operator==(const DatalogLiteral &lhs, const DatalogLiteral &rhs)
    {
        if (!(lhs.atom == rhs.atom))
            return false;
        return lhs.negated == rhs.negated;
    }
};

}  // namespace datalog

#endif  // GROUNDER_LITERAL_H
