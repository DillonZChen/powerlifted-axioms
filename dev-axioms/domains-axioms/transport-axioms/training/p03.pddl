(define (problem transport-03)
    (:domain transport)
    (:requirements :derived-predicates :existential-preconditions :negative-preconditions :typing)
    (:objects l1 l2 l3 - location p1 - package c0 c1 - size v1 - vehicle)
    (:init (at p1 l3) (at v1 l1) (at_G p1 l1) (capacity v1 c1) (capacity-predecessor c0 c1) (road l1 l2) (road l2 l1) (road l2 l3) (road l3 l2))
    (:goal (at p1 l1))
)