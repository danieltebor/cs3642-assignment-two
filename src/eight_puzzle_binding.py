import ctypes

# Load the backend LIBrary.
LIB = ctypes.CDLL('./lib/libcs3642-assignment-two.so')

# Wrapper python class for Node struct
class Node(ctypes.Structure):
    pass

Node._fields_ = [('parent', ctypes.POINTER(Node)),
                 ('state', (ctypes.c_uint * 3) * 3),
                 ('depth', ctypes.c_uint),
                 ('heuristic', ctypes.c_uint)]

LIB.generate_random_start_node.restype = ctypes.POINTER(Node)

# Wrapper function for generate_random_start_node()
def generate_random_start_node() -> ctypes.POINTER(Node):
    return LIB.generate_random_start_node()