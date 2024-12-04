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
public:
    bool negated;
    DatalogAtom atom;

    DatalogLiteral(DatalogAtom atom) : negated(false), atom(std::move(atom)) {}

    DatalogLiteral(bool negated, DatalogAtom atom) : negated(negated), atom(std::move(atom)) {}

    DatalogLiteral() = default;

    friend bool operator==(const DatalogLiteral &lhs, const DatalogLiteral &rhs)
    {
        if (!(lhs.atom == rhs.atom))
            return false;
        return lhs.negated == rhs.negated;
    }
};

}  // namespace datalog

#endif  // GROUNDER_LITERAL_H
