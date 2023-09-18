from eight_puzzle_binding import *
from search_alg_binding import *

result = SearchResult()
start_node = generate_random_start_node().contents

# depth_first_search(start_node, result)
# breadth_first_search(start_node, result)
# uniform_cost_search(start_node, result)
a_star_search(start_node, result)

if result.trace:
    print(f'Nodes visited: {result.num_nodes_visited}')
    print(f'Moves to goal: {result.trace_size - 1}')
    print(f'Path to goal:')
    for i in range(result.trace_size):
        if i == 0:
            print('Start:')
        elif i == result.trace_size - 1:
            print('Goal:')
        else:
            print(f'Move {i}:')
        for row in result.trace[i].contents.state:
            for value in row:
                print(value, end=' ')
            print()
        print()
else:
    print('No solution found.')

free_search_result(result)