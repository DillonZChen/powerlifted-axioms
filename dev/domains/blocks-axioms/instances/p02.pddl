;; base case
;;
(define (problem blocksworld-02)
 (:domain blocksworld)
 (:objects  b1 b2 - object)
 (:init
	(clear_G b2)
	(on_G b2 b1)
	(on_table_G b1)
 
    (arm_empty)
    (clear b2)
    (on_table b2)
    (clear b1)
    (on_table b1)
)
 (:goal (and 
    (clear b2)
    (on b2 b1)
    (on_table b1)
)))