#ifndef SEARCH_AXIOM_H
#define SEARCH_AXIOM_H

#include "datalog/rules/generic_rule.h"

#include <memory>

using Axiom = std::unique_ptr<datalog::GenericRule>;

#endif  // SEARCH_AXIOM_H
