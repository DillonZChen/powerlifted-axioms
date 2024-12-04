#!/bin/bash

domain=openstacks
problem=p01
python3 powerlifted.py -d dev/domains/$domain/domain.pddl -i dev/domains/$domain/$problem.pddl -s gbfs -e goalcount --build
