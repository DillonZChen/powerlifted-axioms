;; base case
;;
(define (problem blocksworld-10)
 (:domain blocksworld)
 (:objects  b1 b2 b3 b4 - object)
 (:init
	(clear_G b1)
	(on_G b1 b2)
	(on_table_G b2)
	(clear_G b3)
	(on_G b3 b4)
	(on_table_G b4)
 
    (arm_empty)
    (clear b3)
    (on b3 b2)
    (on_table b2)
    (clear b1)
    (on b1 b4)
    (on_table b4)
)
 (:goal (and
    (clear b1)
    (on b1 b2)
    (on_table b2)
    (clear b3)
    (on b3 b4)
    (on_table b4)
)))