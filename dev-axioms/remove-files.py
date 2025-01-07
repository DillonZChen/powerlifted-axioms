#! /usr/bin/env python3

import os

CUR_DIR = os.path.dirname(os.path.abspath(__file__))

BENCHMARK_DIR = f"{CUR_DIR}/domains-axioms"
DOMAINS = list(sorted(os.listdir(BENCHMARK_DIR)))

CAP = 3

for domain in DOMAINS:
    for split in ["training", "testing"]:
        for i, f in enumerate(sorted(os.listdir(f"{BENCHMARK_DIR}/{domain}/{split}"))):
            if i >= CAP:
                os.remove(f"{BENCHMARK_DIR}/{domain}/{split}/{f}")

for planner in ["fd-astar-blind", "symk"]:
    for _, f in enumerate(sorted(os.listdir(f"{CUR_DIR}/plans/{planner}"))):
        toks = f.split("_")
        difficulty = int(toks[1])
        problem = int(toks[2])
        if difficulty != 0 or problem > CAP:
            os.remove(f"{CUR_DIR}/plans/{planner}/{f}")