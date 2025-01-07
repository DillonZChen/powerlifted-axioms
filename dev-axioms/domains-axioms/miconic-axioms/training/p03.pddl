(define (problem miconic-03)
    (:domain miconic)
    (:requirements :derived-predicates :existential-preconditions :negative-preconditions :strips :typing)
    (:objects f1 f2 - floor p1 p2 - passenger)
    (:init (above f1 f2) (destin p1 f2) (destin p2 f2) (lift-at f1) (origin p1 f1) (origin p2 f1) (served_G p1) (served_G p2))
    (:goal (and (served p1) (served p2)))
)