# jArser

`jArser` is a small C++17 project exploring how a JSON parser is built from
first principles. It currently reads the bundled JSON example and writes its
contents to standard output; the parsing API is the foundation for the next
stage of the project.

> Status: experimental. This is a learning project, not yet a production JSON
> library.

## Quick start

### Requirements

- A C++17-capable compiler (the default is `g++`)
- GNU Make or a compatible `make` for the automated install and cleaning.

### Build and run

```sh
make
./jsonParser
```

The executable reads [`jsonExample.json`](jsonExample.json) from the current
directory and prints its contents. Run it from the repository root so that the
example file can be found.

### Check and clean

```sh
make check
make clean
```

`make check` builds the program if needed, then runs it against the bundled
example. `make clean` removes the generated `jsonParser` executable.

To choose a different compiler or add flags, pass them to `make`:

```sh
make CXX=clang++ CXXFLAGS='-std=c++17 -O0 -g -Wall -Wextra'
```

## Project layout

| Path | Purpose |
| --- | --- |
| [`jsonParser.cpp`](jsonParser.cpp) | Program entry point and parser implementation in progress |
| [`jsonParser.hpp`](jsonParser.hpp) | Parser declarations and the `jsonValue` representation |
| [`jsonExample.json`](jsonExample.json) | Input used by the example program and `make check` |
| [`Makefile`](Makefile) | Build, check, and clean commands |
| [`.github/workflows/c-cpp.yml`](.github/workflows/c-cpp.yml) | GitHub Actions build and check workflow |

## Current scope and limitations

The repository already contains an initial parser interface:

- `fileReader` loads a file into a string.
- `jsonParser::parsePrimitive` begins distinguishing integer and floating-point
  values.
- `jsonParser::parseJson` and its helpers sketch object parsing.

The parser is not complete yet. In particular, it does not currently provide
full JSON tokenization, validation, arrays, strings with escape sequences,
booleans, `null`, robust error reporting, or a public result that the example
program displays. Avoid using it with untrusted or arbitrary JSON until those
areas are implemented and tested.

## Continuous integration

GitHub Actions runs on pushes and pull requests targeting `main`. The workflow
uses the same commands documented above:

```sh
make
make check
```

Keeping the local and CI commands identical makes failures straightforward to
reproduce before opening a pull request.

## Next steps

Useful directions for the project include:

1. Add a tokenizer with source locations and useful parse errors.
2. Represent all JSON value types safely (for example with `std::variant`).
3. Parse nested objects and arrays recursively.
4. Add focused unit tests for valid input, malformed input, and edge cases.
5. Let the executable accept a JSON file path as a command-line argument.
