# jArser

`jArser` is a lightweight, modern C++23 JSON parser and AST builder developed
from first principles. It provides memory-safe, recursive JSON parsing into a
type-safe AST using standard vocabulary types (`std::variant`, `std::optional`,
`std::string_view`), without external dependencies.

> Status: Complete. Fully functional first-principles C++23 JSON parser and AST builder.

## Features

- **Type-Safe Recursive AST**: Backed by `std::variant` (`jsonData`), supporting `null` (`std::nullptr_t`), booleans, `int64_t`, `double`, `std::string`, `jsonArray`, and `jsonObject`.
- **Zero Memory Leaks**: Uses standard containers (`std::map`, `std::vector`) and value semantics instead of raw pointers or manual heap allocations.
- **Fast Primitive Parsing**: Employs `std::from_chars` for high-performance, allocation-free integer and floating-point parsing.
- **Idiomatic Error Handling**: Functions return `std::optional` to convey parse absence or syntax errors cleanly without throwing exceptions.
- **AST Pretty-Printing**: Includes `jsonParser::printJson()` powered by `std::visit` to format and display the parsed AST.

## Quick start

### Requirements

- A C++23-capable compiler (e.g. GCC 13+ or Clang 16+)
- GNU Make or compatible `make` utility

### Build and run

```sh
make
./jsonParser jsonExample.json
```

The executable accepts a file path as an argument. If no argument is passed, it displays usage instructions.

### Check and clean

```sh
make check
make clean
```

`make check` compiles the program with strict warnings (`-Wall -Wextra -Werror -Wconversion`) and executes it against the bundled [`jsonExample.json`](jsonExample.json).

To test with a different compiler (e.g. Clang):

```sh
make CXX=clang++
```

## Project layout

| Path | Purpose |
| --- | --- |
| [`jsonParser.cpp`](jsonParser.cpp) | CLI entry point, recursive parser, and AST printer implementation |
| [`jsonParser.hpp`](jsonParser.hpp) | Type-safe AST structures (`jsonValue`, `jsonData`) and parser API |
| [`jsonExample.json`](jsonExample.json) | Example JSON payload containing nested structures used in tests |
| [`Makefile`](Makefile) | Strict C++23 compilation, test (`check`), and clean recipes |
| [`.github/workflows/c-cpp.yml`](.github/workflows/c-cpp.yml) | GitHub Actions CI workflow triggered on pushes and PRs to `main` |

## Supported JSON Types

| JSON Type | C++ Representation in `jsonData` |
| --- | --- |
| `null` | `std::nullptr_t` |
| `true` / `false` | `bool` |
| Integer | `int64_t` |
| Floating point | `double` |
| String (`"..."`) | `std::string` |
| Array (`[...]`) | `std::vector<jsonValue>` (`jsonArray`) |
| Object (`{...}`) | `std::map<std::string, jsonValue>` (`jsonObject`) |

## Continuous integration

GitHub Actions runs on every push and pull request targeting `main`. The pipeline builds with `-std=c++23` and executes `make check` on Ubuntu runners.
