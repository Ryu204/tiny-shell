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

To enable auto format and linter, do not use `Visual Studio` generator:

```sh
# In project's root directory
cmake -S . -B build -GNinja -DENABLE_LINTER=ON -DENABLE_FORMAT=ON
cmake --build build
```

# Build by git bash terminal

```sh
# Run this command in git bash terminal of the project's root directory
./compile.sh
```

# Usage

To be documented.