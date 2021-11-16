#ifndef SEARCH_FULL_REDUCER_SUCCESSOR_GENERATOR_H
#define SEARCH_FULL_REDUCER_SUCCESSOR_GENERATOR_H

#include "generic_join_successor.h"

class FullReducerSuccessorGenerator : public GenericJoinSuccessor {

    std::vector<std::vector<std::pair<int, int>>> full_reducer_order;
    std::vector<std::vector<int>> full_join_order;

    std::vector<bool> is_cyclic;

public:
  /**
   * @see full_reducer_successor_generator.cc
   * @param task
   */
    explicit FullReducerSuccessorGenerator(const Task &task);

    Table instantiate(const ActionSchema &action, const DBState &state) override;

};


#endif //SEARCH_FULL_REDUCER_SUCCESSOR_GENERATOR_H
