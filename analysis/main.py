import sys
from call_batch import query_sweep
from graph import graph

def query_mode():
    while True:
        mode = input("\nMode ([a]nalyse/[g]raph): ").strip().lower()

        if mode in ("a", "analyse"):
            return "analyse"

        if mode in ("g", "graph"):
            return "graph"

        print("Invalid mode.")


mode = query_mode()

if mode == "analyse":
    algorithm: str = input("\nInput algorithm: ")

    keyword_greedy = ['greedy']
    keyword_siman = ['siman','simulated_anneal','simulated_annealing','sim_anneal']
    algo_found = 0

    for keyword in keyword_greedy:
        if algorithm == keyword:
            algo_found = 1
            print("We don't currently support analysis for the greedy algorithm!")
            sys.exit(0)

    for keyword in keyword_siman:
        if algorithm == keyword:
            algo_found = 1
            query_sweep()

    if algo_found == 0:
        print("No valid algorithm picked")
else:
    graph();

