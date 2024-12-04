#ifndef GROUNDER_LITERAL_H
#define GROUNDER_LITERAL_H

#include "datalog_atom.h"

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace  datalog {

class DatalogLiteral {
    bool negated;
    DatalogAtom atom;

public:
    DatalogLiteral(bool negated, DatalogAtom atom) : negated(negated), atom(std::move(atom)) {}
};

}

#endif //GROUNDER_LITERAL_H
