;; base case
;;
(define (problem blocksworld-05)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init
	(clear_G b3)
	(on_table_G b3)
	(clear_G b2)
	(on_table_G b2)
	(clear_G b1)
	(on_table_G b1)
 
    (arm_empty)
    (clear b3)
    (on b3 b2)
    (on b2 b1)
    (on_table b1)
)
 (:goal (and 
    (clear b3)
    (on_table b3)
    (clear b2)
    (on_table b2)
    (clear b1)
    (on_table b1)
)))
