# C-IO-Implementation

### Project Description
This is a from-scratch implementation of standard C input/output functions (stdio.h). The project includes custom versions of `fopen`, `fclose`, `fseek`, `fflush`, and buffered read/write operations.

### Key Features
1. **Buffering**: Support for both buffered and unbuffered I/O operations
2. **Standard Streams**: `stdin`, `stdout`, `stderr` with appropriate buffering modes
3. **File Operations**: File opening, closing, and positioning
4. **Error Handling**: `feof` and `ferror` flags for stream state monitoring

### Project Structure
- `io.h` - Header file with structure and function declarations
- `io.c` - Core I/O functions implementation
- `tester.c` - Comprehensive test suite for the library
- `Makefile` - Build and test system

### Build and Usage
1. **Build tests**: `make`
2. **Run tests**: `make run` or `./tester`
3. **Memory leak check**: `make memcheck`
4. **Cleanup**: `make clean`

### Architectural Decisions
1. **FILE Structure**: Contains buffer, pointers, file descriptor, and bit flags
2. **Global File Table**: `_iob[OPEN_MAX]` array for managing open streams
3. **Macro-based Implementation**: `getc` and `putc` implemented as macros for efficiency

### Limitations
1. Only "r", "w", "a" modes are supported
2. Maximum open files: 20
3. Buffer size: 1024 bytes
4. No formatted I/O support (printf/scanf)

### Testing
The test suite validates:
- Basic file read/write operations
- Append mode functionality
- File positioning with fseek
- Buffer flushing mechanisms
- Multiple file handling
- Unbuffered stderr output
- EOF and error flag management

### License
This educational implementation is free to use for learning purposes.
