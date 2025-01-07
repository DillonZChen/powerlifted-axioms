;; base case
;;
(define (problem blocksworld-03)
 (:domain blocksworld)
 (:objects  b1 b2 - object)
 (:init
	(clear_G b2)
	(on_table_G b2)
	(clear_G b1)
	(on_table_G b1)
 
    (arm_empty)
    (clear b1)
    (on b1 b2)
    (on_table b2)
)
 (:goal (and
    (clear b2)
    (on_table b2)
    (clear b1)
    (on_table b1)
)))