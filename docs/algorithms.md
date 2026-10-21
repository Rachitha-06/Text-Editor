# Algorithms in LineForge

## 1. Text Insertion

### Line insertion
Locate the insertion point by walking from head or tail (whichever is closer), then rewire four pointers (or fewer at the ends).  
Time: O(n) to find + O(1) to link.

### Character insertion inside a line
`std::string::insert` – linear in the length of the line.

## 2. Text Deletion

Symmetric to insertion. The deleted line’s text is saved inside the corresponding `DeleteLineCommand` / `DeleteTextCommand` so it can be restored on undo.

## 3. Search (Simple Substring)

For each line, a linear scan looks for the pattern:

```
for each line i:
    for pos = 0 to line.size()-pattern.size():
        if matchesAt(line, pos, pattern, caseSensitive):
            record match
```

Case-insensitive mode converts both characters to lower-case on the fly.

**Time complexity:** O(L · M) where L is total characters and M is pattern length (worst case).

## 4. Search (Knuth–Morris–Pratt – optional demonstration)

KMP preprocesses the pattern into a longest-prefix-suffix (LPS) array in O(M) time.  
The search itself never backtracks in the text, giving O(N + M) total time.

### LPS construction (prefix function)

```
lps[0] = 0
len = 0, i = 1
while i < M:
    if pattern[i] == pattern[len]:
        len++
        lps[i] = len
        i++
    else if len > 0:
        len = lps[len-1]
    else:
        lps[i] = 0
        i++
```

KMP is implemented in `SearchEngine::kmpSearch` and exercised by the test suite. The interactive editor currently uses the simple search for clarity and because documents are expected to be modest in size.

## 5. Replace

- **Replace first** – locate the first match, then issue a `ReplaceTextCommand`.
- **Replace all** – collect all matches first (so positions stay valid), build a `ReplaceAllCommand` that stores every old/new pair, then apply the replacements from the **end** of the document toward the beginning. This prevents position invalidation and also correctly handles the case where the replacement text itself contains the search pattern (no infinite loop).

## 6. Undo / Redo

Classic Command-pattern algorithm:

```
execute(cmd):
    cmd.execute()
    undoStack.push(cmd)
    redoStack.clear()

undo():
    if undoStack empty → nothing
    cmd = undoStack.pop()
    cmd.undo()
    redoStack.push(cmd)

redo():
    if redoStack empty → nothing
    cmd = redoStack.pop()
    cmd.execute()
    undoStack.push(cmd)
```

## 7. File Loading / Saving

- Load: read entire file into a string via `rdbuf()`, then split on `'\n'` into lines.
- Save: walk the linked list and write each line followed by `'\n'` (except possibly a trailing newline policy that matches common editors).

Both operations are O(total characters).
