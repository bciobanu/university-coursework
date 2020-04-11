#!/usr/bin/env python3
import sys
from collections import deque
from enum import Enum, auto
from typing import NamedTuple, List, Set, Tuple, Optional, Dict, Union, FrozenSet


class SymbolType(Enum):
    TERMINAL = auto()
    NON_TERMINAL = auto()


class Symbol(NamedTuple):
    id: str
    type: SymbolType

    def __str__(self) -> str:
        return self.id


START_SYMBOL = Symbol(id="S`", type=SymbolType.NON_TERMINAL)
LAMBDA_SYMBOL = Symbol(id="λ", type=SymbolType.TERMINAL)
END_SYMBOL = Symbol(id="$", type=SymbolType.TERMINAL)


class Production(NamedTuple):
    initial: Symbol
    result: Tuple[Symbol, ...]

    def __str__(self) -> str:
        return (
            str(self.initial)
            + " -> "
            + "".join(str(result_symbol) for result_symbol in self.result)
        )


class Grammar(NamedTuple):
    productions: List[Production]
    symbols: List[Symbol]
    start_symbol: Symbol


class LR0Item(NamedTuple):
    production: Production
    index: int

    @property
    def running_symbol(self) -> Optional[Symbol]:
        if (
            self.index < len(self.production.result)
            and self.production.result[self.index] != END_SYMBOL
        ):
            return self.production.result[self.index]
        return None

    def __str__(self) -> str:
        result = str(self.production.initial) + " -> "
        for i, result_symbol in enumerate(self.production.result):
            if i == self.index:
                result = result + "."
            result = result + str(result_symbol)
        if self.index == len(self.production.result):
            result = result + "."
        return result


Node = FrozenSet[LR0Item]


class Automaton(NamedTuple):
    nodes: List[Node]
    edges: Dict[Tuple[Node, Symbol], Node]


class Shift(NamedTuple):
    state: int

    def __str__(self) -> str:
        return str(self.state)


class Reduce(NamedTuple):
    production: Production

    def __str__(self) -> str:
        return "R " + str(self.production)


class Accept:
    def __str__(self) -> str:
        return "A"


ParseTableElement = Union[Shift, Reduce, Accept, None]
ParseTable = List[Dict[Symbol, ParseTableElement]]


def lambda_closure(initial: Node, grammar: Grammar) -> Node:
    result = set(initial)
    bfq = deque(initial)
    while len(bfq) > 0:
        node = bfq.popleft()
        desired_symbol = node.running_symbol
        if desired_symbol is None or desired_symbol.type is SymbolType.TERMINAL:
            continue
        for production in grammar.productions:
            if production.initial == desired_symbol:
                item = LR0Item(production=production, index=0)
                if item not in result:
                    result.add(item)
                    bfq.append(item)
    return frozenset(result)


def goto(node: Node, symbol: Symbol, grammar: Grammar) -> Node:
    initial_set = frozenset(
        LR0Item(production=item.production, index=item.index + 1)
        for item in node
        if item.running_symbol == symbol
    )
    return lambda_closure(initial_set, grammar)


def build_dfa(grammar: Grammar) -> Automaton:
    initial_state = lambda_closure(
        frozenset([LR0Item(production=grammar.productions[0], index=0)]), grammar,
    )
    states = {initial_state}
    edges = {}

    bfq, qhead = [initial_state], 0
    while qhead != len(bfq):
        node = bfq[qhead]
        qhead += 1
        for symbol in grammar.symbols:
            adj = goto(node, symbol, grammar)
            if not adj:
                continue
            edges[(node, symbol)] = adj
            if adj not in states:
                states.add(adj)
                bfq.append(adj)
    return Automaton(nodes=bfq, edges=edges)


def join_first(
    first: Dict[Symbol, Set[Symbol]], symbols: Tuple[Symbol, ...]
) -> Set[Symbol]:
    result = set()
    lambda_set = {LAMBDA_SYMBOL}
    index = 0
    while index < len(symbols) and LAMBDA_SYMBOL in first[symbols[index]]:
        result |= first[symbols[index]] - lambda_set
        index += 1
    if index < len(symbols):
        result |= first[symbols[index]]
    else:
        result |= lambda_set
    return result


def build_first(grammar: Grammar) -> Dict[Symbol, Set[Symbol]]:
    first: Dict[Symbol, Set[Symbol]] = {symbol: set() for symbol in grammar.symbols}
    for symbol in grammar.symbols:
        if symbol.type is SymbolType.TERMINAL:
            first[symbol].add(symbol)
    for production in grammar.productions:
        if production.result == (LAMBDA_SYMBOL,):
            first[production.initial].add(LAMBDA_SYMBOL)

    def step(
        grammar: Grammar, first: Dict[Symbol, Set[Symbol]]
    ) -> Dict[Symbol, Set[Symbol]]:
        new_first = {key_symbol: set(values) for key_symbol, values in first.items()}
        for production in grammar.productions:
            if (
                production.result == (LAMBDA_SYMBOL,)
                or production.initial is START_SYMBOL
            ):
                continue
            new_first[production.initial] |= join_first(first, production.result)
        return new_first

    while True:
        next_first = step(grammar, first)
        if first == next_first:
            break
        first = next_first
    return first


def build_follow(
    grammar: Grammar, first: Dict[Symbol, Set[Symbol]]
) -> Dict[Symbol, Set[Symbol]]:
    follow: Dict[Symbol, Set[Symbol]] = {
        symbol: set()
        for symbol in grammar.symbols
        if symbol.type is SymbolType.NON_TERMINAL
    }

    for production in grammar.productions:
        for index in range(len(production.result) - 1):
            if production.result[index] in follow:
                follow[production.result[index]] |= join_first(
                    first, production.result[index + 1 :]
                ) - {LAMBDA_SYMBOL}

    for from_symbol, to_symbols in follow.items():
        print(
            f"Follow {from_symbol} = {''.join(str(to_symbol) for to_symbol in to_symbols)}"
        )
    print("")

    def step(
        grammar: Grammar,
        first: Dict[Symbol, Set[Symbol]],
        follow: Dict[Symbol, Set[Symbol]],
    ) -> Dict[Symbol, Set[Symbol]]:
        new_follow = {key_symbol: set(values) for key_symbol, values in follow.items()}
        for production in grammar.productions:
            if production.initial not in follow:
                continue
            for resulting in reversed(production.result):
                if resulting in follow:
                    new_follow[resulting] |= follow[production.initial]
                if LAMBDA_SYMBOL not in first[resulting]:
                    break
        return new_follow

    while True:
        next_follow = step(grammar, first, follow)
        if follow == next_follow:
            break
        follow = next_follow
    return follow


def build_table(dfa: Automaton, grammar: Grammar) -> Optional[ParseTable]:
    node_idx = {state: idx for idx, state in enumerate(dfa.nodes)}

    first = build_first(grammar)
    for from_symbol, to_symbols in first.items():
        print(
            f"First {from_symbol} = {''.join(str(to_symbol) for to_symbol in to_symbols)}"
        )
    print("")

    follow = build_follow(grammar, first)
    for from_symbol, to_symbols in follow.items():
        print(
            f"Follow {from_symbol} = {''.join(str(to_symbol) for to_symbol in to_symbols)}"
        )
    print("")

    table: ParseTable = []
    for state in dfa.nodes:
        row: Dict[Symbol, ParseTableElement] = {}
        for lr0item in state:
            if (
                lr0item.running_symbol is grammar.start_symbol
                and lr0item.running_symbol == END_SYMBOL
            ):
                row[lr0item.running_symbol] = Accept()
            elif lr0item.running_symbol is not None:
                if lr0item.running_symbol.type is SymbolType.TERMINAL:
                    if lr0item.running_symbol in row:
                        return None

                    row[lr0item.running_symbol] = Shift(
                        state=node_idx[dfa.edges[(state, lr0item.running_symbol)]]
                    )
            elif lr0item.production.initial in follow:
                for follow_symbol in follow[lr0item.production.initial]:
                    if follow_symbol in row:
                        return None
                    row[follow_symbol] = Reduce(production=lr0item.production)

        for non_terminal in grammar.symbols:
            if non_terminal.type is not SymbolType.NON_TERMINAL:
                continue
            if (state, non_terminal) in dfa.edges:
                row[non_terminal] = Shift(
                    state=node_idx[dfa.edges[(state, non_terminal)]]
                )
        table.append(row)
    return table


def augment_grammar(grammar: Grammar) -> Grammar:
    return Grammar(
        productions=[
            Production(initial=START_SYMBOL, result=(grammar.start_symbol, END_SYMBOL,))
        ]
        + grammar.productions,
        symbols=[grammar.start_symbol, END_SYMBOL] + grammar.symbols,
        start_symbol=START_SYMBOL,
    )


def main() -> None:
    E = Symbol(id="E", type=SymbolType.NON_TERMINAL)
    T = Symbol(id="T", type=SymbolType.NON_TERMINAL)
    F = Symbol(id="F", type=SymbolType.NON_TERMINAL)

    plus = Symbol(id="+", type=SymbolType.TERMINAL)
    star = Symbol(id="*", type=SymbolType.TERMINAL)
    ids = Symbol(id="id", type=SymbolType.TERMINAL)
    open_bracket = Symbol(id="(", type=SymbolType.TERMINAL)
    closed_bracket = Symbol(id=")", type=SymbolType.TERMINAL)

    """
    1) E  -> E + T
    2) E  -> T
    3) T  -> T * F
    4) T  -> F
    5) F  -> ( E )
    6) F  -> id
    """

    grammar = Grammar(
        productions=[
            Production(initial=E, result=(E, plus, T,)),
            Production(initial=E, result=(T,)),
            Production(initial=T, result=(T, star, F,)),
            Production(initial=T, result=(F,)),
            Production(initial=F, result=(open_bracket, E, closed_bracket,)),
            Production(initial=F, result=(ids,)),
        ],
        symbols=[T, F, plus, star, ids, open_bracket, closed_bracket],
        start_symbol=E,
    )

    grammar = augment_grammar(grammar)
    automaton = build_dfa(grammar)
    table = build_table(automaton, grammar)

    for i, node in enumerate(automaton.nodes):
        print(f"Node {i}:")
        for lr0item in node:
            print(lr0item)
        print("")

    print("\t".join(str(symbol) for symbol in grammar.symbols))
    for row in table:
        for symbol in grammar.symbols:
            print(row[symbol] if symbol in row else ".", end="\t")
        print("")


if __name__ == "__main__":
    main()
