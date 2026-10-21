# Viva Questions (LineForge)

1. What is the primary data structure used to store the document and why was it chosen?
2. Explain the structure of a node in your doubly linked list.
3. How do you handle insertion of a line at the beginning, middle, and end?
4. What is the time complexity of accessing the k-th line? Why?
5. How is memory managed for the linked list? Are there any risks of leaks?
6. Why did you disable the copy constructor of DoublyLinkedList / Document?
7. Explain the Command pattern and how it is used for undo/redo.
8. What happens to the redo stack when a new edit is performed after an undo?
9. How does a DeleteLineCommand restore the deleted line on undo?
10. Why is Replace-All implemented by collecting all matches first and then applying them from the end?
11. What is the difference between the simple search and KMP?
12. Explain the LPS array used by KMP.
13. What is the time complexity of KMP preprocessing and search?
14. How do you support case-insensitive search?
15. How is an empty search pattern handled?
16. Describe the steps performed when a file is opened.
17. When is the “modified” flag set and when is it cleared?
18. What happens if Save As is attempted to a path that already exists?
19. How are document statistics calculated? How are words counted?
20. Why might character counts not equal the number of visible Unicode characters?
21. How does the UI prevent the program from crashing on invalid menu input?
22. Explain RAII and how it appears in this project.
23. What is the role of the Editor class versus the Document class?
24. Why is the UserInterface separated from the core editing logic?
25. How would you change the design if you needed O(1) random access to any line?
26. What are the advantages of a doubly linked list over a singly linked list here?
27. How does the list decide whether to start searching from head or tail?
28. What is the space complexity of storing a document of L characters?
29. How would you implement multi-level undo limited to the last N operations?
30. What happens if the replacement text contains the search pattern during Replace All?
31. How are trailing newlines handled when loading a file?
32. Why is `std::unique_ptr` used for commands?
33. Explain move semantics in the context of DoublyLinkedList.
34. What testing strategy was used? How many automated assertions pass?
35. Name three edge cases that the test suite covers.
36. How would you add support for multiple open documents?
37. What limitations does a pure console application have compared with a GUI editor?
38. How can the same core be reused for a future GUI version?
39. What is the difference between Save and Save As in the implementation?
40. If you had more time, what would be the next feature you would add and why?
