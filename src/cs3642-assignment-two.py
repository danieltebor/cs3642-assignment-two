from eight_puzzle_binding import *
from search_alg_binding import *

result = SearchResult()
start_node = generate_random_start_node()
for row in start_node.state:
    for value in row:
        print(value, end=' ')
    print() 

depth_first_search(start_node, result)
#print(result.num_nodes_visited)
#free_search_result(result)