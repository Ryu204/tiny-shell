# How to build

Prerequisites:
* `CMake` version `>=3.21` and `C11` compiler
* (optional) `clang-format`
* (optional) `clang-tidy`

```sh
# In project's root directory
cmake -S . -B build 
cmake --build build
```

`CMake` options:
```cmake
option(ENABLE_FORMAT "Use clang-format as formatter" off)
option(ENABLE_LINTER "Use clang-tidy as linter" off)
option(ENABLE_DOCS "Generate documentation with doxygen" off)
option(ENABLE_TEST "Build test programs" off)
```

# Description

This project features a simple interactive shell capable of running executable and scripts, as well as executing limited local commands.

## Supported platform: 

* Windows (tested on Windows 11)
* Unix-like (tested on WSL and Arch Linux) 

## Semantics:

* Multiples whitespaces is treated as a single space (`' '`)
* To retain whitespaces or to use multiples words as a parameter, use quotation mark `"` (e.g `" a   word with space"`)
* `'!'` symbol is not supported

## Implementation:

1. Scan input by line from standard input
1. Parse input string into `argc` and `argv`
1. Parse `argc`, `argv` to decide corresponding command type
1. Run the command and print result to standard output, or exit on `Exit` command
1. Go to step 1

OS-specific operations are separatedly implemented via `#ifdef` directive.

# Usage

To be documented.