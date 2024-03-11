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
```

# Usage

To be documented.