import ctypes

from eight_puzzle_binding import LIB, Node


# Wrapper function for SearchResult struct
class SearchResult(ctypes.Structure):
    _fields_ = [('trace_size', ctypes.c_uint),
                ('num_nodes_visited', ctypes.c_uint),
                ('cpu_time_taken_ms', ctypes.c_double)]

# Set up function argument types for the backend LIBrary.
LIB.depth_first_search.argtypes = (ctypes.POINTER(Node), ctypes.POINTER(SearchResult))
LIB.breadth_first_search.argtypes = (ctypes.POINTER(Node), ctypes.POINTER(SearchResult))
LIB.uniform_cost_search.argtypes = (ctypes.POINTER(Node), ctypes.POINTER(SearchResult))
LIB.a_star_search.argtypes = (ctypes.POINTER(Node), ctypes.POINTER(SearchResult))

# Wrapper function for search_alg functions.
def depth_first_search(start_node: Node, result: SearchResult):
    LIB.depth_first_search(start_node, result)

def breadth_first_search(start_node: Node, result: SearchResult):
    LIB.breadth_first_search(start_node, result)

def uniform_cost_search(start_node: Node, result: SearchResult):
    LIB.uniform_cost_search(start_node, result)

def a_star_search(start_node: Node, result: SearchResult):
    LIB.a_star_search(start_node, result)