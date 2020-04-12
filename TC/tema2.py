#!/usr/bin/env python3
import logging
from collections import deque
from enum import Enum, auto
from typing import Dict, FrozenSet, List, NamedTuple, Optional, Set, Tuple, Union

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)


def init_logger() -> None:
    handler = logging.StreamHandler()
    formatter = logging.Formatter("%(message)s")
    handler.setFormatter(formatter)
    logger.addHandler(handler)


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
    state: Node
    state_index: int

    def __str__(self) -> str:
        return f"S{self.state_index}"


class Reduce(NamedTuple):
    production: Production
    production_index: int

    def __str__(self) -> str:
        return f"R{self.production_index}"


class Accept:
    def __str__(self) -> str:
        return "A"


class TableRow(dict):
    def __setitem__(self, key: Symbol, value: Union[Shift, Reduce, Accept]) -> None:
        if key in self:
            raise KeyError
        dict.__setitem__(self, key, value)


ParseTable = Dict[Node, TableRow]


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
        logging.debug(
            f"Node {qhead}:\n" + "".join(str(lr0item) + "\n" for lr0item in node)
        )

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

    for from_symbol, to_symbols in first.items():
        logging.debug(
            f"First({from_symbol}) = "
            + "".join(str(to_symbol) for to_symbol in to_symbols)
        )
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

    for from_symbol, to_symbols in follow.items():
        logging.debug(
            f"Follow({from_symbol}) = "
            + "".join(str(to_symbol) for to_symbol in to_symbols)
        )
    return follow


def build_table(dfa: Automaton, grammar: Grammar) -> Optional[ParseTable]:
    first = build_first(grammar)
    follow = build_follow(grammar, first)

    node_indexer = {node: idx for idx, node in enumerate(dfa.nodes)}
    production_indexer = {
        production: idx for idx, production in enumerate(grammar.productions)
    }

    table: ParseTable = {}
    for state in dfa.nodes:
        row = TableRow()
        for lr0item in state:
            if (
                lr0item.running_symbol is grammar.start_symbol
                and lr0item.running_symbol == END_SYMBOL
            ):
                row[lr0item.running_symbol] = Accept()
            elif lr0item.running_symbol is not None:
                if lr0item.running_symbol.type is SymbolType.TERMINAL:
                    shift_state = dfa.edges[(state, lr0item.running_symbol)]
                    cell = Shift(
                        state=shift_state, state_index=node_indexer[shift_state]
                    )
                    try:
                        row[lr0item.running_symbol] = cell
                    except KeyError:
                        return None
            elif lr0item.production.initial in follow:
                for follow_symbol in follow[lr0item.production.initial]:
                    cell = Reduce(
                        production=lr0item.production,
                        production_index=production_indexer[lr0item.production],
                    )
                    try:
                        row[follow_symbol] = cell
                    except KeyError:
                        return None

        for non_terminal in grammar.symbols:
            if non_terminal.type is not SymbolType.NON_TERMINAL:
                continue
            if (state, non_terminal) in dfa.edges:
                shift_state = dfa.edges[(state, non_terminal)]
                row[non_terminal] = Shift(
                    state=shift_state, state_index=node_indexer[shift_state]
                )
        table[state] = row

    logger.debug("\t".join(str(symbol) for symbol in grammar.symbols))
    for row in table.values():
        logger.debug(
            "\t".join(
                str(row[symbol]) if symbol in row else "." for symbol in grammar.symbols
            )
        )

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


def read_grammar_and_queries() -> Tuple[Grammar, List[str]]:
    """
    E
    T F
    + * id ( )
    E -> E + T
    E -> T
    T -> T * F
    T -> F
    F -> ( E )
    F -> id
    (id*id)
    """
    symbols: Dict[str, Symbol] = {}
    symbols["λ"] = LAMBDA_SYMBOL

    start_symbol = Symbol(id=input(), type=SymbolType.NON_TERMINAL)
    symbols[start_symbol.id] = start_symbol
    for symbol_type in (SymbolType.NON_TERMINAL, SymbolType.TERMINAL):
        for symbol_id in input().split():
            assert symbol_id != START_SYMBOL.id and symbol_id != END_SYMBOL.id
            assert symbol_id not in symbols or symbols[symbol_id].type == symbol_type
            assert symbol_id != "->"
            symbols[symbol_id] = Symbol(id=symbol_id, type=symbol_type)

    queries: List[str] = []
    productions = []
    while True:
        try:
            line = input()
        except EOFError:
            break
        tokens = line.split()
        if len(tokens) > 1 and tokens[1] == "->":
            assert len(queries) == 0
            productions.append(
                Production(
                    initial=symbols[tokens[0]],
                    result=tuple(symbols[token] for token in tokens[2:]),
                )
            )
        else:
            assert len(tokens) == 1
            queries.append(line)
    del symbols[start_symbol.id]
    return (
        Grammar(
            productions=productions,
            symbols=list(symbols.values()),
            start_symbol=start_symbol,
        ),
        queries,
    )


def main() -> None:
    init_logger()

    grammar, queries = read_grammar_and_queries()
    grammar = augment_grammar(grammar)
    automaton = build_dfa(grammar)
    table = build_table(automaton, grammar)

    if not table:
        logger.warning("Not SLR(1)")
        return


if __name__ == "__main__":
    main()
