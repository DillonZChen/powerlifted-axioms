#!/usr/bin/env python3
import argparse
import os

_CUR_DIR = os.path.abspath(os.path.dirname(__file__))

_DOMAINS = {
    "blocks-axioms": "instances/p10.pddl",
    "satellite-axioms": "instances/p10.pddl",
    "openstacks": "p01.pddl",
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--domain", choices=_DOMAINS.keys(), default="blocks-axioms")
    args = parser.parse_args()
    domain_file = f"{_CUR_DIR}/domains/{args.domain}/domain.pddl"
    problem_file = f"{_CUR_DIR}/domains/{args.domain}/{_DOMAINS[args.domain]}"
    assert os.path.exists(domain_file)
    assert os.path.exists(problem_file)

    cmd = [
        "python3",
        "powerlifted.py",
        "-d",
        domain_file,
        "-i",
        problem_file,
        "-s",
        "gbfs",
        "-e",
        "goalcount",
        "--build",
    ]
    os.system(" ".join(cmd))


if __name__ == "__main__":
    main()
