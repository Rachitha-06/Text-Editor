# Complexity Analysis

Assumptions:
- Document contains *n* lines.
- Average / total characters ≈ *L*.
- Pattern length = *M*.
- Finding a node by index walks from the nearer end → average O(n/2) = O(n).

| Operation                        | Time Complexity | Space Complexity | Notes |
|----------------------------------|-----------------|------------------|-------|
| Insert line at known index       | O(n)            | O(1)             | Find node + O(1) link |
| Append line                      | O(1)            | O(1)             | Tail pointer |
| Delete line by index             | O(n)            | O(1)             | |
| Access line by index             | O(n)            | O(1)             | |
| Insert/delete chars in a line    | O(line length)  | O(1)             | `std::string` |
| Find all (simple)                | O(L · M)        | O(k)             | k = number of matches |
| Find all (KMP per line)          | O(L + n·M)      | O(M + k)         | |
| Replace first                    | O(L · M)        | O(1)             | |
| Replace all                      | O(L · M)        | O(k)             | Collect then apply |
| Load file                        | O(L)            | O(L)             | |
| Save file                        | O(L)            | O(1)             | |
| Undo / Redo                      | O(cost of the original command) | O(1) extra | |
| Document statistics              | O(L)            | O(1)             | |
| Clear document                   | O(n)            | O(1)             | |

**Space for the whole document:** O(L) – each character is stored once inside the `std::string`s of the nodes, plus a constant overhead per node for the two pointers and bookkeeping.
