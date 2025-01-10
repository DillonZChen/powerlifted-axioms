#ifndef AXIOMS_MATCHER_H_
#define AXIOMS_MATCHER_H_

#include "../parallel_hashmap/phmap.h"

#include <unordered_map>
#include <utility>
#include <vector>

class AxiomMatch {
    int axiom;
    int position;
    bool negated;

public:
    AxiomMatch(int r, int p, bool n) : axiom(r), position(p), negated(n) {}

    int get_axiom() const {
        return axiom;
    }

    int get_position() const {
        return position;
    }

    bool is_negated() const {
        return negated;
    }
};

class AxiomMatches {
    std::vector<AxiomMatch> matches;

public:
    AxiomMatches() = default;

    explicit
    AxiomMatches(std::vector<AxiomMatch> &&matches) : matches(std::move(matches)) {}

    void insert_new_match(int r, int p, bool n) {
        matches.emplace_back(r, p, n);
    }

    std::vector<AxiomMatch>::const_iterator begin() const {
        return matches.begin();
    }

    std::vector<AxiomMatch>::const_iterator end() const {
        return matches.end();
    }

};

class AxiomMatcher {
    /*
     Map index of an atom to a vector axiom matches
    */
    phmap::flat_hash_map<int, AxiomMatches> axiom_matcher;

    static const AxiomMatches empty_matches;

    bool atom_has_matched_axioms(int i) const {
        return (axiom_matcher.find(i)!=axiom_matcher.end());
    }

public:
    AxiomMatcher() = default;

    void insert(int predicate_index, int axiom_index, bool negated, int position) {
        if (!atom_has_matched_axioms(predicate_index)) {
            axiom_matcher[predicate_index] = AxiomMatches();
        }
        axiom_matcher[predicate_index].insert_new_match(axiom_index, position, negated);
    }

    const AxiomMatches &get_matched_axioms(int index) const {
        auto result = axiom_matcher.find(index);
        if (result==axiom_matcher.end())
            return empty_matches;
        return result->second;
    }

    void dump() {
        for (const auto &p : axiom_matcher) {
            std::cout << "predicate: " << p.first << " ";
            for (const auto &m : p.second) {
                std::cout << '(' << m.get_axiom() << ", " << m.get_position() << "," << m.is_negated() << "),";
            }
            std::cout << std::endl;
        }
    }

};

#endif //AXIOMS_MATCHER_H_
