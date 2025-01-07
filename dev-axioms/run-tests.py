#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
import subprocess
import timeit
from itertools import product

"""
This test component just checks to see if the plans found
in several different domains and configurations have the optimal
cost expected.

The expected run time of each run is less than 30s.

"""

CURDIR = os.path.dirname(__file__)
BASEDIR = os.path.abspath(f"{CURDIR}/..")

DOMAINS = ["blocksworld", "ferry", "floortile", "miconic", "rover", "satellite", "transport"]
OPTIMAL_PLAN_COSTS = {}
for domain, i in product(DOMAINS, range(1, 3)):
    domain_pddl = f"{CURDIR}/domains-axioms/{domain}-axioms/domain.pddl"
    problem_pddl = f"{CURDIR}/domains-axioms/{domain}-axioms/testing/p0_{i:02d}.pddl"
    plan_file1 = f"{CURDIR}/plans/fd-astar-blind/{domain}_0_{i:02d}_0.plan"
    plan_file2 = f"{CURDIR}/plans/symk/{domain}_0_{i:02d}_0.plan"
    plan1 = open(plan_file1).read()
    plan2 = open(plan_file2).read()
    # assuming no comments in the files
    plan_length1 = plan1.count("(")
    plan_length2 = plan2.count("(")
    assert plan_length1 == plan_length2
    OPTIMAL_PLAN_COSTS[(domain_pddl, problem_pddl)] = plan_length1

SEARCH_CONFIGS = ['bfs', 'gbfs']
HEURISTIC_CONFIGS = ['blind']
GENERATOR_CONFIGS = ['full_reducer', 'join', 'yannakakis']


class TestRun:
    def __init__(self, domain, instance, config):
        self.domain = domain
        self.instance = instance
        self.search = config[0]
        self.heuristic = config[1]
        self.generator = config[2]

    def get_config(self):
        return "{}, {}, and {}".format(self.search,
                                           self.heuristic,
                                           self.generator)

    def __str__(self):
        return "{} with {}".format(self.instance, self.get_config())

    def run(self):
        print("Testing {} with {}: ".format(self.instance, self.get_config()), end='', flush=True)
        output = subprocess.check_output([os.path.join(BASEDIR, 'powerlifted.py'),
                                          '-d', self.domain,
                                          '-i', self.instance,
                                          '-s', self.search,
                                          '-e', self.heuristic,
                                          '-g', self.generator,
                                          '--validate'])
        return output

    def evaluate(self, output, optimal_cost):
        plan_length_found = None
        plan_valid = None
        for line in output.splitlines():
            if b'Total plan cost:' in line:
                plan_length_found = int(line.split()[3])
            if b'Plan valid' in line:
                plan_valid = True

        if plan_length_found == optimal_cost and plan_valid:
            print("PASSED")
            return True
        else:
            print("FAILED ", end="")
            if plan_length_found != optimal_cost:
                print("[expected: {}, plan length found: {}]".format(optimal_cost, plan_length_found), end="")
            if not plan_valid:
                print("[VAL did not validate the plan]", end="")
            print("")
            return False

    def remove_plan_file(self):
        plan_file = 'sas_plan'
        if os.path.isfile(plan_file):
            os.remove(plan_file)


def print_summary(passes, failures, starting_time):
    total = passes + failures
    print("Total number of passed tests: %d/%d" % (passes, total))
    print("Total number of failed tests: %d/%d" % (failures, total))
    print("Total time: %.2fs" % (timeit.default_timer() - starting_time))


def parse_options():
    parser = argparse.ArgumentParser()
    parser.add_argument('--minimal', dest='minimal', action='store_true',
                        help='Use minimal test set.')
    args = parser.parse_args()

    return args


if __name__ == '__main__':
    args = parse_options()

    if args.minimal:
        raise NotImplementedError()
        SEARCH_CONFIGS = ['bfs', 'gbfs']
        HEURISTIC_CONFIGS = ['blind']
        GENERATOR_CONFIGS = ['full_reducer', 'yannakakis']

    start = timeit.default_timer()
    failures = 0
    passes = 0
    for (domain, instance), cost in OPTIMAL_PLAN_COSTS.items():
        for config in product(SEARCH_CONFIGS, HEURISTIC_CONFIGS, GENERATOR_CONFIGS):
            test = TestRun(domain, instance, config)
            output = test.run()
            passed = test.evaluate(output, cost)
            if passed:
                passes += 1
            else:
                failures += 1
            test.remove_plan_file()

    print_summary(passes, failures, start)
