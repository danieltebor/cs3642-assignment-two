#include "node.h"

// Checks if two nodes are equivalent.
bool check_states_are_equivalent(const unsigned int state1[3][3], const unsigned int state2[3][3]) {
    // Traverse state.
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            // Check if tile is misplaced. If so return false.
            if (state1[y][x] != state2[y][x]) {
                return false;
            }
        }
    }
    return true;
}