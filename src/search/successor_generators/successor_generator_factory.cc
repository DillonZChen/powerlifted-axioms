
#include "successor_generator_factory.h"

#include "full_reducer_successor_generator.h"
#include "naive_successor.h"
#include "ordered_join_successor.h"
#include "random_successor.h"
#include "yannakakis.h"
#include "clique_successor_generator.h"

#include "../database/table.h"

#include <iostream>

#include <boost/algorithm/string.hpp>

SuccessorGenerator *SuccessorGeneratorFactory::create(const std::string &method,
                                                      unsigned seed,
                                                      Task &task,
                                                      AxiomsEvaluator &axioms_evaluator)
{
    std::cout << "Creating successor generator factory..." << std::endl;
    if (boost::iequals(method, "join")) {
        return new NaiveSuccessorGenerator(task, axioms_evaluator);
    }
    else if (boost::iequals(method, "full_reducer")) {
        return new FullReducerSuccessorGenerator(task, axioms_evaluator);
    }
    else if (boost::iequals(method, "inverse_ordered_join")) {
        return new OrderedJoinSuccessorGenerator<InverseOrderTable>(task, axioms_evaluator);
    }
    else if (boost::iequals(method, "ordered_join")) {
        return new OrderedJoinSuccessorGenerator<OrderTable>(task, axioms_evaluator);
    }
    else if (boost::iequals(method, "random_join")) {
        return new RandomSuccessorGenerator(task, axioms_evaluator, seed);
    }
    else if (boost::iequals(method, "yannakakis")) {
        return new YannakakisSuccessorGenerator(task, axioms_evaluator);
    }
    else if (boost::iequals(method, "clique_bk")) {
        return new CliqueSuccessorGenerator(task, axioms_evaluator, BronKerboschFirst);
    }
    else if (boost::iequals(method, "clique_kckp")) {
        return new CliqueSuccessorGenerator(task, axioms_evaluator, KCliqueKPartite);
    }
    else {
        std::cerr << "Invalid successor generator method \"" << method << "\"" << std::endl;
        exit(-1);
    }
}
