# My Algorithm:

Scan each block and check if its left block is valid when this block is set. This check uses bit operation and converte every 3 into 4 in order to calculate simplier.
After every block is put, calculate the total score and add with 2.31 * (blocks left) and then compare with current max score. If the current total score is too small, then cut this branch.
When we read in m and n, compare them. If N<M then transposition the map.

# My data structure:

Normal structure with a vector< vector<int> >.