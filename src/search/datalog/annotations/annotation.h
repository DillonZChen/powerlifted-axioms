#ifndef SEARCH_DATALOG_ANNOTATIONS_ANNOTATION_H_
#define SEARCH_DATALOG_ANNOTATIONS_ANNOTATION_H_

#include <functional>

#include "../ground_rule.h"

namespace datalog {

class Datalog;

class Annotation {
public:
    virtual ~Annotation() = default;

    virtual void execute(int head,
                         const Datalog &datalog) = 0;

    virtual bool operator==(const Annotation &other) = 0;

    bool operator!=(const Annotation &other) {return !(*this == other);}
};
}

#endif //SEARCH_DATALOG_ANNOTATIONS_ANNOTATION_H_
