#include "axioms_evaluator.h"


DBState AxiomsEvaluator::extend_state(const DBState &state) {
    DBState new_state = state;
    std::vector<datalog::DatalogAtom> derived_atoms = evaluate(state);
    // TODO convert DatalogAtom to Relation, could just be combined with `evaluate` later
    return new_state;
}

std::vector<datalog::DatalogAtom> AxiomsEvaluator::evaluate(const DBState &state) {
    std::vector<datalog::DatalogAtom> derived_atoms;
    std::vector<datalog::Fact> facts = get_datalog_facts_from_state(state);

    return derived_atoms;
}

std::vector<datalog::Fact> AxiomsEvaluator::get_datalog_facts_from_state(const DBState &s) {
    std::vector<datalog::Fact> facts = static_data;
    for (const auto &r: s.get_relations()) {
        for (const auto &tuple: r.tuples) {
            std::vector<std::pair<int, int>> args;
            for (int i: tuple) {
              args.emplace_back(i, datalog::OBJECT);
            }
            facts.emplace_back(datalog::Arguments(args), r.predicate_symbol, false);
        }
    }
    for (size_t i = 0; i < s.get_nullary_atoms().size(); ++i) {
        if (s.get_nullary_atoms()[i]) {
            facts.emplace_back(datalog::Arguments(), i, false);
        }
    }

    for (const datalog::Fact &f : facts) {
      std::cout << f.get_predicate_index() <<'(';
      for (auto p : f.get_arguments()) {
        std:: cout << p.get_index() << ", ";
      }
      std::cout << ')' << std::endl;
    }

    return facts;
}
