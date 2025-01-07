(define (problem floortile-03)
    (:domain floortile)
    (:requirements :derived-predicates :existential-preconditions :negative-preconditions :typing)
    (:objects black white - color robot1 - robot tile_0_1 tile_0_2 tile_0_3 tile_1_1 tile_1_2 tile_1_3 tile_2_1 tile_2_2 tile_2_3 tile_3_1 tile_3_2 tile_3_3 tile_4_1 tile_4_2 tile_4_3 - tile)
    (:init (available-color black) (available-color white) (clear tile_0_1) (clear tile_0_2) (clear tile_0_3) (clear tile_1_1) (clear tile_1_2) (clear tile_1_3) (clear tile_2_1) (clear tile_2_2) (clear tile_2_3) (clear tile_3_1) (clear tile_3_3) (clear tile_4_1) (clear tile_4_2) (clear tile_4_3) (down tile_0_1 tile_1_1) (down tile_0_2 tile_1_2) (down tile_0_3 tile_1_3) (down tile_1_1 tile_2_1) (down tile_1_2 tile_2_2) (down tile_1_3 tile_2_3) (down tile_2_1 tile_3_1) (down tile_2_2 tile_3_2) (down tile_2_3 tile_3_3) (down tile_3_1 tile_4_1) (down tile_3_2 tile_4_2) (down tile_3_3 tile_4_3) (left tile_0_1 tile_0_2) (left tile_0_2 tile_0_3) (left tile_1_1 tile_1_2) (left tile_1_2 tile_1_3) (left tile_2_1 tile_2_2) (left tile_2_2 tile_2_3) (left tile_3_1 tile_3_2) (left tile_3_2 tile_3_3) (left tile_4_1 tile_4_2) (left tile_4_2 tile_4_3) (painted_G tile_1_1 white) (painted_G tile_1_2 black) (painted_G tile_1_3 white) (painted_G tile_2_1 black) (painted_G tile_2_2 white) (painted_G tile_2_3 black) (painted_G tile_3_1 white) (painted_G tile_3_2 black) (painted_G tile_3_3 white) (painted_G tile_4_1 black) (painted_G tile_4_2 white) (painted_G tile_4_3 black) (right tile_0_2 tile_0_1) (right tile_0_3 tile_0_2) (right tile_1_2 tile_1_1) (right tile_1_3 tile_1_2) (right tile_2_2 tile_2_1) (right tile_2_3 tile_2_2) (right tile_3_2 tile_3_1) (right tile_3_3 tile_3_2) (right tile_4_2 tile_4_1) (right tile_4_3 tile_4_2) (robot-at robot1 tile_3_2) (robot-has robot1 white) (up tile_1_1 tile_0_1) (up tile_1_2 tile_0_2) (up tile_1_3 tile_0_3) (up tile_2_1 tile_1_1) (up tile_2_2 tile_1_2) (up tile_2_3 tile_1_3) (up tile_3_1 tile_2_1) (up tile_3_2 tile_2_2) (up tile_3_3 tile_2_3) (up tile_4_1 tile_3_1) (up tile_4_2 tile_3_2) (up tile_4_3 tile_3_3))
    (:goal (and (painted tile_1_1 white) (painted tile_1_2 black) (painted tile_1_3 white) (painted tile_2_1 black) (painted tile_2_2 white) (painted tile_2_3 black) (painted tile_3_1 white) (painted tile_3_2 black) (painted tile_3_3 white) (painted tile_4_1 black) (painted tile_4_2 white) (painted tile_4_3 black)))
)