# Troubleshooting

**Build fails with “C++17 not supported”**  
Update your compiler (g++ ≥ 7, clang ≥ 5, recent MSVC).

**“Permission denied” when running the binary**  
`chmod +x lineforge lineforge_tests`

**Clear screen does nothing**  
The program uses `system("clear")` / `system("cls")`. On restricted environments the call may be ignored; functionality is otherwise unaffected.

**Unicode characters look wrong**  
Ensure the terminal is set to UTF-8. The editor stores and writes the bytes correctly; display depends on the terminal.

**Tests cannot create temporary files**  
Run the test binary from a writable directory.

**CMake cannot find a compiler**  
Install a C++ toolchain and make sure it is on PATH, or specify `-DCMAKE_CXX_COMPILER=...`.
