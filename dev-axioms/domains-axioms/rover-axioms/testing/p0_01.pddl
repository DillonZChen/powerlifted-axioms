(define (problem rover-01)
    (:domain rover)
    (:requirements :derived-predicates :existential-preconditions :negative-preconditions :strips :typing)
    (:objects camera1 - camera general - lander colour high_res low_res - mode objective1 - objective rover1 - rover rover1store - store waypoint1 waypoint2 waypoint3 waypoint4 - waypoint)
    (:init (at rover1 waypoint2) (at_lander general waypoint1) (at_rock_sample waypoint1) (at_rock_sample waypoint2) (at_rock_sample waypoint3) (at_soil_sample waypoint2) (at_soil_sample waypoint3) (calibration_target camera1 objective1) (communicated_rock_data_G waypoint2) (communicated_rock_data_G waypoint3) (communicated_soil_data_G waypoint2) (empty rover1store) (equipped_for_imaging rover1) (equipped_for_rock_analysis rover1) (equipped_for_soil_analysis rover1) (on_board camera1 rover1) (store_of rover1store rover1) (supports camera1 colour) (supports camera1 high_res) (supports camera1 low_res) (visible waypoint1 waypoint2) (visible waypoint1 waypoint3) (visible waypoint1 waypoint4) (visible waypoint2 waypoint1) (visible waypoint2 waypoint3) (visible waypoint2 waypoint4) (visible waypoint3 waypoint1) (visible waypoint3 waypoint2) (visible waypoint3 waypoint4) (visible waypoint4 waypoint1) (visible waypoint4 waypoint2) (visible waypoint4 waypoint3) (visible_from objective1 waypoint1) (visible_from objective1 waypoint2) (visible_from objective1 waypoint3) (visible_from objective1 waypoint4))
    (:goal (and (communicated_rock_data waypoint3) (communicated_rock_data waypoint2) (communicated_soil_data waypoint2)))
)