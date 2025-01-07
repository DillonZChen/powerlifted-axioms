;; base case
;;
(define (problem blocksworld-01)
 (:domain blocksworld)
 (:objects  b1 b2 - object)
 (:init
	(arm_empty_G)
	(clear_G b1)
	(on_G b1 b2)
	(on_table_G b2)
 
    (arm_empty)
    (clear b2)
    (on_table b2)
    (clear b1)
    (on_table b1)
)
 (:goal (and 
     (arm_empty)
    (clear b1)
    (on b1 b2)
    (on_table b2)
)))