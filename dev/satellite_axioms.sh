#!/bin/bash

python3 powerlifted.py -d dev/domains/satellite-axioms/domain.pddl -i dev/domains/satellite-axioms/instances/p10.pddl -s gbfs -e goalcount --build
