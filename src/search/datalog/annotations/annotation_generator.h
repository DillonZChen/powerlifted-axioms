#ifndef SEARCH_DATALOG_ANNOTATIONS_ANNOTATION_GENERATOR_H_
#define SEARCH_DATALOG_ANNOTATIONS_ANNOTATION_GENERATOR_H_

#include <functional>

#include "../ground_rule.h"


#include "../rules/rule_base.h"

#include "../../task.h"

namespace datalog {

using AnnotationGenerator = std::function<std::unique_ptr<Annotation>(int, const Task&)>;

}

#endif //SEARCH_DATALOG_ANNOTATIONS_ANNOTATION_GENERATOR_H_
