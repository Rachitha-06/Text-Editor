# Viva Answers (LineForge)

**1.** A custom doubly linked list of lines. It demonstrates linked-list concepts required by the curriculum, allows O(1) splicing of lines once the node is known, and keeps the implementation understandable.

**2.** Each node contains a `std::string text`, a `Node* prev`, and a `Node* next`.

**3.** Beginning: new node becomes head, old head’s prev points to it. End: use the tail pointer. Middle: locate the node at the index, then rewire the four surrounding pointers.

**4.** O(n). The list must be traversed from the nearer end; there is no random-access array of pointers.

**5.** The list owns every node. The destructor and `clear()` walk the chain and `delete` each node. Move operations transfer ownership; copies are deleted to avoid double-free.

**6.** Deep-copying a whole document is expensive and rarely needed. Disabling copy prevents accidental expensive or incorrect copies; move is provided instead.

**7.** Each editable action is an object that knows how to `execute()` itself and how to `undo()` itself. The manager keeps stacks of these objects.

**8.** The redo stack is cleared. This is standard behaviour: a new edit creates a new future, discarding the previously undone branch.

**9.** Before deleting, the command stores a copy of the line’s text. On undo it simply inserts that text back at the same index.

**10.** Collecting first guarantees we have a consistent set of original positions. Applying from the end toward the beginning keeps earlier positions valid. It also avoids infinite loops when the replacement contains the pattern.

**11.** Simple search can take O(L·M) time and may re-examine characters. KMP preprocesses the pattern so the text is scanned only once – O(L+M) total.

**12.** The LPS (longest proper prefix that is also a suffix) array tells the algorithm how far to shift the pattern after a mismatch, avoiding unnecessary comparisons.

**13.** Preprocessing O(M), search O(N) where N is the text length – overall O(N+M).

**14.** Characters are converted to lower-case with `std::tolower` (cast to unsigned char) on both the line and the pattern before comparison.

**15.** An empty pattern immediately returns an empty match list; no replacement is performed.

**16.** The file is opened with `ifstream`, the entire content is read into a string, the string is split on newlines into the linked list, the filename is stored, and the modified flag is cleared.

**17.** Set on any successful editing operation. Cleared only after a successful Save or Save As.

**18.** The user is asked for confirmation before overwriting.

**19.** The list is walked once. Characters are the sum of `string::size()`. Words are counted by detecting transitions from whitespace to non-whitespace.

**20.** `std::string` stores bytes. A single Unicode code point may occupy multiple bytes in UTF-8; the editor does not perform grapheme clustering.

**21.** All numeric input is read as a string and then parsed with range checks inside a loop that repeats until a valid value is obtained.

**22.** Resource Acquisition Is Initialization – objects acquire resources in constructors and release them in destructors. Examples: `unique_ptr` for commands and the document, the linked-list destructor freeing nodes.

**23.** Document owns the line list and pure content operations. Editor owns the document, the undo/redo manager, and provides a higher-level API that automatically records history.

**24.** Separation of concerns. The core can be unit-tested and later reused under a GUI without rewriting editing logic.

**25.** Store the lines in a `std::vector<std::string>` (or keep a secondary vector of node pointers). That gives O(1) access at the cost of O(n) line insertion/deletion in the middle.

**26.** A doubly linked list allows efficient traversal in both directions and makes deletion of a known node O(1) without needing the predecessor to be passed in.

**27.** If the requested index is less than size/2 it starts from the head; otherwise it starts from the tail.

**28.** O(L) – each character lives in exactly one `std::string`, plus a small constant overhead per line for the node pointers.

**29.** Keep a maximum size on the undo stack; when it exceeds N, discard the oldest commands from the bottom of the stack (requires a different container or periodic pruning).

**30.** Because matches are collected on the original text and replacements are applied from the end, the algorithm never re-searches the newly inserted text, so it terminates after a finite number of replacements.

**31.** `std::getline` splits on `'\n'`. A final empty line appears only if the file ends with a newline in a way that produces an extra empty string; the implementation keeps the behaviour simple and documented.

**32.** Exclusive ownership, automatic deletion, and clear transfer of ownership when moving a command between the undo and redo stacks.

**33.** The move constructor/assignment steal the head, tail and size pointers and leave the source in an empty valid state, avoiding deep copies.

**34.** A custom lightweight test harness with 61 assertions covering list, document, history, search, files and edge cases. All currently pass.

**35.** Empty document, empty search pattern, out-of-range indices, missing files, redo stack cleared after a new edit.

**36.** Change the Editor to hold a collection of Documents (or a vector of unique_ptrs) and add a “switch buffer” command; the UI would show a list of open files.

**37.** No mouse, no rich fonts, limited Unicode display, no syntax highlighting, harder multi-pane layouts.

**38.** The Document, Command, UndoRedoManager, SearchEngine and FileManager classes have no console dependencies; a GUI would only replace the UserInterface layer.

**39.** Save writes to the current filename (and fails if it is still “Untitled”). Save As always asks for a path, may overwrite after confirmation, and updates the document’s filename only on success.

**40.** A simple GUI front-end (for example with Dear ImGui or Qt) that reuses the existing core, or multi-document support with tabs – both would make the project more useful while still exercising the same data structures.
