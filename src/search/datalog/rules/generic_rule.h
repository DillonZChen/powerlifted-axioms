#ifndef SEARCH_DATALOG_RULES_GENERIC_RULE_H_
#define SEARCH_DATALOG_RULES_GENERIC_RULE_H_

#include "rule_base.h"

namespace datalog {

class GenericRule : public RuleBase {

    int schema_index;

public:
    using RuleBase::RuleBase;

    GenericRule(int weight,
                DatalogAtom eff,
                std::vector<DatalogLiteral> c,
                std::unique_ptr<Annotation> annotation,
                int schema_index)
        : RuleBase(weight, eff, c, std::move(annotation)), schema_index(schema_index)
    {
    }

    GenericRule(DatalogAtom eff, std::vector<DatalogLiteral> c) : RuleBase(0, std::move(eff), std::move(c), nullptr) {};

    void clean_up() override {}

    int get_type() const override { return GENERIC; }

    int get_schema_index() { return schema_index; }

    std::string get_type_name() override { return "GenericRule"; }
};
}  // namespace datalog


#endif  // SEARCH_DATALOG_RULES_GENERIC_RULE_H_
