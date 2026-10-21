# System Architecture

```
+------------------+
|  UserInterface   |  menus, input validation, display
+--------+---------+
         |
         v
+--------+---------+
|     Editor       |  façade, owns Document + UndoRedoManager
+--------+---------+
         |
    +----+----+------------+--------------+
    |         |            |              |
    v         v            v              v
Document  UndoRedoMgr  SearchEngine  FileManager
    |         |
    v         v
DoublyLinkedList   Command hierarchy
(lines)            (Insert/Delete/Replace...)
```

**Document** is the single source of truth for content.  
**Commands** mutate the Document and know how to reverse the mutation.  
**UndoRedoManager** owns the history stacks.  
**UserInterface** never touches the linked list directly; it only calls Editor methods.
