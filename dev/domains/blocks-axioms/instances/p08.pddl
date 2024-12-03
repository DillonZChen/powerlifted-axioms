;; base case
;;
(define (problem blocksworld-08)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init
	(clear_G b3)
	(on_G b3 b2)
	(on_G b2 b1)
	(on_table_G b1)
 
    (arm_empty)
    (clear b1)
    (on b1 b2)
    (on b2 b3)
    (on_table b3)
)
 (:goal (and
    (clear b3)
    (on b3 b2)
    (on b2 b1)
    (on_table b1)
)))
