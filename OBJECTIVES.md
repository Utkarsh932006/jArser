# jArser Modernization Roadmap & Learning Objectives

This document outlines key engineering and language objectives to modernize `jArser`, make it memory-safe, and upgrade it to modern C++ idioms with official reference documentation.

---

## 1. Type-Safe AST & Memory Safety

### Objective
Replace the raw `union jsonValue` and manual heap allocations (`new std::map`) with a type-safe `std::variant`-based recursive AST to eliminate memory leaks and provide compile-time type safety.

### Current Code vs. Modern Alternative
- **Current**: Raw union in [`jsonParser.hpp`](jsonParser.hpp) with a dangling `new std::map<std::string, jsonValue>` pointer.
- **Modern C++**:
  ```cpp
  #include <variant>
  #include <vector>
  #include <map>
  #include <string>

  struct JsonValue;
  using JsonObject = std::map<std::string, JsonValue>;
  using JsonArray  = std::vector<JsonValue>;

  using JsonData = std::variant<
      std::nullptr_t,
      bool,
      int64_t,
      double,
      std::string,
      JsonArray,
      JsonObject
  >;

  struct JsonValue {
      JsonData data;
  };
  ```

### Key References & Resources
- [`std::variant` on cppreference](https://en.cppreference.com/w/cpp/utility/variant) — Type-safe union container.
- [`std::visit` on cppreference](https://en.cppreference.com/w/cpp/utility/variant/visit) — Pattern matching and visiting variant alternatives.
- [`std::holds_alternative` on cppreference](https://en.cppreference.com/w/cpp/utility/variant/holds_alternative) — Checking the active type.
- [`std::get_if` on cppreference](https://en.cppreference.com/w/cpp/utility/variant/get_if) — Non-throwing type access via pointer.

---

## 2. Zero-Copy String Parsing (`std::string_view`)

### Objective
Replace iterator index arithmetic and repeated `std::string::substr()` heap allocations with zero-copy `std::string_view` slices across tokenizing and parsing functions.

### Current Code vs. Modern Alternative
- **Current**: `output.substr(start - output.begin(), end - start)` in [`jsonParser.cpp:L34`](jsonParser.cpp#L34).
- **Modern C++**:
  ```cpp
  #include <string_view>

  // Slices without memory allocation:
  std::string_view slice = source.substr(start_pos, count);
  ```

### Key References & Resources
- [`std::string_view` on cppreference](https://en.cppreference.com/w/cpp/string/basic_string_view) — Non-owning view of a character sequence.
- [`std::string_view::substr` on cppreference](https://en.cppreference.com/w/cpp/string/basic_string_view/substr) — $O(1)$ zero-allocation sub-views.
- [`std::string_view::remove_prefix` on cppreference](https://en.cppreference.com/w/cpp/string/basic_string_view/remove_prefix) — Trimming consumed tokens in place.

---

## 3. Fast Number Parsing (`std::from_chars`)

### Objective
Replace `std::stoi` and `std::stod` (which require null-terminated strings and allocate temporaries) with low-overhead, zero-allocation `std::from_chars`.

### Current Code vs. Modern Alternative
- **Current**: `std::stoi(substr)` and `std::stod(substr)` in [`jsonParser.cpp:L38-40`](jsonParser.cpp#L38-L40).
- **Modern C++**:
  ```cpp
  #include <charconv>
  #include <system_error>

  double value{};
  auto [ptr, ec] = std::from_chars(slice.data(), slice.data() + slice.size(), value);
  if (ec == std::errc()) {
      // Successfully parsed
  }
  ```

### Key References & Resources
- [`std::from_chars` on cppreference](https://en.cppreference.com/w/cpp/utility/from_chars) — High-performance, non-allocating string-to-number conversion.
- [`std::from_chars_result` on cppreference](https://en.cppreference.com/w/cpp/utility/from_chars_result) — Structured return type with error code.

---

## 4. Modern Loops, Algorithms & Ranges

### Objective
Use range-based for loops, structured bindings, and standard algorithms instead of C-style manual indexing or raw iterator increments.

### Modern C++ Patterns
```cpp
// Structured bindings for map iteration:
for (const auto& [key, val] : jsonMap) {
    // Process key-value pairs cleanly
}

// C++20 views and algorithms:
#include <ranges>
#include <algorithm>

auto is_whitespace = [](char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; };
```

### Key References & Resources
- [Structured Bindings (`auto [a, b]`) on cppreference](https://en.cppreference.com/w/cpp/language/structured_binding) — Unpacking tuples, pairs, and structs.
- [Range-based `for` loop on cppreference](https://en.cppreference.com/w/cpp/language/range-for) — Iterating collections without manual iterators.
- [Ranges Library (`std::ranges`) on cppreference](https://en.cppreference.com/w/cpp/ranges) — Composable range transformations and algorithms.

---

## 5. Efficient File Ingestion (`std::filesystem` & Streams)

### Objective
Replace line-by-line `std::getline` (which strips `\n` and triggers repeated reallocations) with a single-pass file read into memory.

### Modern C++ Patterns
```cpp
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
```

### Key References & Resources
- [`std::basic_ifstream::rdbuf` on cppreference](https://en.cppreference.com/w/cpp/io/basic_ios/rdbuf) — Direct buffer stream extraction.
- [`std::filesystem::file_size` on cppreference](https://en.cppreference.com/w/cpp/filesystem/file_size) — Querying exact file size to pre-allocate buffer with `reserve()`.

---

## 6. Safe Error Diagnostics & Vocabulary Types

### Objective
Replace abrupt `assert()` statements with structured diagnostic types indicating error descriptions, line numbers, and column offsets.

### Modern C++ Patterns
```cpp
#include <optional>
#include <string>

struct ParseError {
    std::string message;
    size_t line;
    size_t column;
};

// Or std::optional for simple absence:
std::optional<JsonValue> parse(std::string_view input);
```

### Key References & Resources
- [`std::optional` on cppreference](https://en.cppreference.com/w/cpp/utility/optional) — Managing optional return values safely.
- [`std::expected` on cppreference (C++23)](https://en.cppreference.com/w/cpp/utility/expected) — Returning value or error object without exceptions.

---

## 7. CLI UX & POSIX Compliance

### Objective
Support command-line arguments, pipe inputs from `stdin`, separate diagnostic output from data output, and return standard exit codes.

### Key References & Resources
- [Standard streams (`std::cin`, `std::cout`, `std::cerr`) on cppreference](https://en.cppreference.com/w/cpp/io/cin) — Stream routing.
- [`EXIT_SUCCESS` and `EXIT_FAILURE` on cppreference](https://en.cppreference.com/w/cpp/utility/program/EXIT_status) — Standard exit code semantics.
