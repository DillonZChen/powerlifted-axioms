;; base case
;;
(define (problem blocksworld-06)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init
	(clear_G b3)
	(on_G b3 b2)
	(on_G b2 b1)
	(on_table_G b1)
 
    (arm_empty)
    (clear b3)
    (on_table b3)
    (clear b2)
    (on_table b2)
    (clear b1)
    (on_table b1)
)
 (:goal (and
    (clear b3)
    (on b3 b2)
    (on b2 b1)
    (on_table b1)
)))
