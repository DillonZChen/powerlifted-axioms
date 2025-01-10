#ifndef SEARCH_AXIOMS_EVALUATOR_H
#define SEARCH_AXIOMS_EVALUATOR_H

#include "axiom_matcher.h"

#include "../axiom.h"

#include <queue>
#include <unordered_map>
#include <vector>


/*
  NOTE: The algorithm implemented here is very similar to the weighted grounder
  used to compute delete-relaxed heuristics. However, here we allow for
  stratified negation, and don't need anything from annotations, weights, and
  best achievers.

  Therefore, we prefer to pay the price of some "redundant" code to keep thigs
  cleaner and more flexible. Conceptually, one could use a generic semi-naive
  evaluation for both cases and then postprocess what is specific for each use
  case. But this would make the code more confusing and slower, so we go with a
  specialized implementation for each case.

*/

// TODO Do we want its own namespace?

class AxiomsEvaluator {

    // Stratification based on predicate symbol, instead of rule
    std::unordered_map<int, int> map_pred_layer;

    AxiomMatcher axiom_matcher;

/*
              std::vector<datalog::DatalogLiteral> body = axiom->get_conditions();
              for (size_t i = 0; i < body.size(); ++i) {
                  predicate_index = body[i].get_predicate_index();
                  if (map_pred_layer.count(predicate_index) == 0)
                      map_pred_layer[predicate_index] = layer;
              }
*/

  void set_stratification_by_predicate(const std::vector<std::vector<Axiom>> &stratified_axioms) {
      // We store the axioms in the evaluator and define some useful
      // data structures.
      for (size_t layer = 0; layer < stratified_axioms.size(); ++layer) {
        int predicate_index;
          for (const auto &axiom : stratified_axioms[layer]) {
              datalog::DatalogAtom head = axiom->get_effect();
              predicate_index = head.get_predicate_index();
              if (map_pred_layer.count(predicate_index) == 0)
                  map_pred_layer[predicate_index] = layer;
          }
      }
  }

  void create_axiom_matcher(const std::vector<std::vector<Axiom>> &stratified_axioms) {
      for (const auto &layer : stratified_axioms)
          for (const auto &rule : layer) {
              int cont = 0;
              for (const auto &condition : rule->get_conditions()) {
                  axiom_matcher.insert(condition.get_predicate_index(),
                                      rule->get_index(),
                                      condition.is_negated(),
                                      cont++);
              }
          }
  }

public:
    AxiomsEvaluator() = default;

    void set_axioms(const std::vector<std::vector<Axiom>> &stratified_axioms) {
      set_stratification_by_predicate(stratified_axioms);
      create_axiom_matcher(stratified_axioms);
    }
};

#endif // SEARCH_AXIOMS_EVALUATOR_H
