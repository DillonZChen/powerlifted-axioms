;; blocks=5, out_folder=testing/easy, instance_id=2, seed=1008

(define (problem blocksworld-02)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 - object)
 (:init
	(clear_G b2)
	(on_G b2 b5)
	(on_G b5 b4)
	(on_G b4 b3)
	(on_G b3 b1)
	(on_table_G b1)
 
    (arm_empty)
    (clear b3)
    (on b3 b1)
    (on_table b1)
    (clear b2)
    (on b2 b4)
    (on b4 b5)
    (on_table b5))
 (:goal  (and 
    (clear b2)
    (on b2 b5)
    (on b5 b4)
    (on b4 b3)
    (on b3 b1)
    (on_table b1))))
