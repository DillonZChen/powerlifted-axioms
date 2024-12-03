;; base case
;;
(define (problem blocksworld-07)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init
	(clear_G b1)
	(on_G b1 b2)
	(on_G b2 b3)
	(on_table_G b3)
 
    (arm_empty)
    (clear b3)
    (on b3 b2)
    (on b2 b1)
    (on_table b1)
)
 (:goal (and 
    (clear b1)
    (on b1 b2)
    (on b2 b3)
    (on_table b3)
)))
