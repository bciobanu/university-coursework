#!/usr/bin/env python3
import argparse
from collections import deque


class Automaton:
    LINK_FORMAT = "https://chart.googleapis.com/chart?cht=gv&chl=digraph{0}"

    class Node:
        LAMBDA = "λ"

        def __init__(self):
            self.transitions = []

        def add_transition(self, node, character=LAMBDA):
            self.transitions.append((node, character))


    def __init__(self, character):
        self.start_node = Automaton.Node()
        self.final_node = Automaton.Node()
        self.start_node.add_transition(self.final_node, character)

    def concat(self, other):
        self.final_node.add_transition(other.start_node)
        self.final_node = other.final_node
        return self

    def union(self, other):
        new_start = Automaton.Node()
        new_start.add_transition(self.start_node)
        new_start.add_transition(other.start_node)
        self.start_node = new_start

        new_end = Automaton.Node()
        self.final_node.add_transition(new_end)
        other.final_node.add_transition(new_end)
        self.final_node = new_end
        return self

    def wildcard(self):
        new_start = Automaton.Node()
        new_end = Automaton.Node()
        new_start.add_transition(self.start_node)
        new_start.add_transition(new_end)
        self.final_node.add_transition(self.start_node)
        self.final_node.add_transition(new_end)

    def __repr__(self):
        def get_index(node):
            if node not in get_index.node_indexer:
                get_index.node_indexer[node] = get_index.index
                get_index.index += 1
            return get_index.node_indexer[node]

        get_index.node_indexer = {}
        get_index.index = 0

        q = deque([self.start_node])
        s = ""
        link_data = "{"
        while len(q) > 0:
            node = q.popleft()
            for adj, ch in node.transitions:
                s += f"{get_index(node)} {get_index(adj)} {ch}\n"
                link_data += f'{get_index(node)}->{get_index(adj)}[label="{ch}"];'
                q.append(adj)
        link_data = link_data[:-1] + "}"
        image_link = Automaton.LINK_FORMAT.format(link_data)
        s += f"Imagine: {image_link}"
        return s


class Solver:
    def __init__(self, expr):
        self.expr = iter(expr)
        self.consume()

    def consume(self):
        self.curr = next(self.expr, None)

    def parse_parentheses(self):
        if self.curr == "(":
            self.consume()
            result = self.parse()
            self.consume()
        else:
            result = Automaton(self.curr)
            self.consume()
        return result

    def parse_wildcard(self):
        result = self.parse_parentheses()
        if self.curr == "*":
            self.consume()
            result.wildcard()
        return result

    def parse_concat(self):
        result = self.parse_wildcard()
        while self.curr == ".":
            self.consume()
            result.concat(self.parse_wildcard())
        return result

    def parse_union(self):
        result = self.parse_concat()
        while self.curr == "|":
            self.consume()
            result.union(self.parse_concat())
        return result

    def parse(self):
        return self.parse_union()


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("regex", type=str)
    return parser.parse_args()


def main():
    args = parse_args()
    print(Solver(args.regex).parse())


if __name__ == "__main__":
    main()
