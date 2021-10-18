#! /usr/bin/env python

import os
import subprocess


class Hypertree:
    def __init__(self) -> None:
        self.bag = []
        self.cover = []
        self.parent = None
        self.children = []

    def set_bag(self, vertices):
        self.bag = vertices

    def set_cover(self, edges):
        self.cover = edges

    def add_child(self, child):
        self.children.append(child)
        child.parent = self

    def del_child(self, child):
        self.children.remove(child)
        child.bag = None
        child.cover = None
        child.parent = None
        child.children = None

    def _upwards(self):
        if self.parent is not None:
            p = self.parent
            if subset(self.bag, p.bag):
                p.cover.extend(self.cover)
                p.cover = list(dict.fromkeys(p.cover))
                for c in self.children:
                    p.add_child(c)

                p.del_child(self)
                return True
        return False

    def _downwards(self):
        heir = None
        for c in self.children:
            if subset(self.bag, c.bag):
                heir = c
                break
        if heir is not None:
            heir.cover.extend(self.cover)
            heir.cover = list(dict.fromkeys(heir.cover))
            self.children.remove(heir)
            for s in self.children:
                heir.add_child(s)
            if self.parent is not None:
                self.parent.add_child(heir)
                self.parent.del_child(self)
            else:
                heir.parent = None
                self.children = None
            return True
        return False


class Hypergraph:
    def __init__(self) -> None:
        self.edges = {}

    def add_edge(self, name, vertices):
        self.edges[name] = vertices


'''l1 subset l2'''
def subset(l1, l2):
    for e in l1:
        if e not in l2:
            return False
    return True

def get_hypertree_decompositions(task):
    print("Using Hypertree decompositions. 'BalancedGo' is expected to be in the PATH.")
    delete_previous_htd_files()
    for action in task.actions:
        f_name = generate_action_hypertree(action)
        hd = compute_decompositions(f_name)
        action.decomposition = parse_decompositions(hd, action)
        action.join_tree = get_join_tree(hd)
    delete_files(".ast")
    delete_files(".htd")

def get_join_tree(hd):
    '''
    Return list containing pairs of tree edges, from root (node 0) to leaves.
    '''
    edges = []
    queue = [hd[0]]
    while len(queue) > 0:
        top = queue[0]
        queue.pop(0)
        idx = hd.index(top)
        for c in top.children:
            idx_c = hd.index(c)
            edges.append((str(idx), str(idx_c)))
            queue.append(c)
    return edges


def parse_decompositions(hd, action):
    '''
    Transform hypertree into sequence of operations (join, projections, etc).

    '''
    decomposition = []
    map_prec_to_hyperedge = dict()
    for p in action.precondition.parts:
        if p.negated:
            continue
        map_prec_to_hyperedge[p.hyperedge] = p
    for node in hd:
        d = []
        for p in node.cover:
            d.append(map_prec_to_hyperedge[p])
        decomposition.append(d)
    return decomposition

def delete_previous_htd_files():
    print("Deleting previous '.htd' files.")
    delete_files(".htd")


def delete_files(extension):
    cwd = os.getcwd()
    files = os.listdir(cwd)
    for f in files:
        if f.endswith(extension):
            os.remove(os.path.join(cwd, f))


def generate_action_hypertree(action):
    i = 0
    f = open(action.name + ".ast", 'w')
    for p in action.precondition.parts:
        if p.predicate == '=':
            continue
        atom_name = "{}-{}".format(p.predicate, str(i))
        i = i + 1
        terms = ','.join(p.args).replace('?', '')
        f.write('%s(%s)\n' % (atom_name, terms))
        p.hyperedge = atom_name
    f.close()
    return f.name


def compute_decompositions(file):
    decomp_file_name = file
    decomp_file_name = decomp_file_name.replace('.ast', '.htd')
    f = open(decomp_file_name, 'w')
    BALANCED_GO_CMD = ['BalancedGo',
                       '-bench',
                       '-approx', '10',
                       '-det',
                       '-graph', file,
                       '-complete',
                       '-gml', decomp_file_name]

    res = subprocess.run(BALANCED_GO_CMD, stdout=subprocess.PIPE,
                         check=True, universal_newlines=True)
    hd = []
    parents = []
    for line in res.stdout.splitlines():
        if 'Bag: {' in line:
            node = Hypertree()
            line = line.strip()[6:-1]
            node.set_bag([v.strip() for v in line.split(',')])
            hd.append(node)

            if len(parents):
                par = parents[-1]
                par.add_child(node)
        elif 'Cover: {' in line:
            line = line.strip()[8:-1]
            hd[-1].set_cover([v.strip() for v in line.split(',')])
            #hd[-1].covered = covered(hd[-1]) # Davide told to comment out this list
        elif 'Children:' in line:
            parents.append(hd[-1])
        elif ']' in line:
            parents = parents[:-1]
    return hd


def print_decompositions(action, parameter_index, object_index, predicate_index, type_index, f):
    '''
    This is a bit of a hack, but we print the actions in the same order as they were printed by
    the translator.
    '''
    map_precond_to_position = dict()
    for idx, p in enumerate(action.get_action_preconditions):
        map_precond_to_position[p] = idx
    print(len(action.decomposition), file=f)
    for node in action.decomposition:
        print (len(node), file=f)
        for cover in node:
            print(" ".join([str(map_precond_to_position[cover])] +
                           [str(len(cover.args))] +
                           [str(parameter_index[arg]) for arg in cover.args]),
                  file=f, end=' ')
            print(file=f)
    print(len(action.join_tree), file=f)
    for edge in action.join_tree:
        print(" ".join(edge), file=f)