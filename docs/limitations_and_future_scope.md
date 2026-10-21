# Limitations and Future Scope

## Current Limitations

- Console-only interface
- Single document at a time
- Byte-oriented character counts (no full Unicode grapheme support)
- Entire document held in RAM
- No syntax highlighting, folding, or multiple views
- Terminal must support basic ANSI / clear commands for best experience

## Future Scope

1. GUI front-end (Qt or Dear ImGui) sharing the same Document / Command core
2. Multiple buffers / tabs
3. Regular-expression search
4. Persistent configuration and recent-file list
5. Optional gap-buffer or rope for extremely large files
6. Plugin or scripting interface
