# PDLex

PDLex is a small C++ prototype for a simple `.pd` language. It started as a lexer and grew into a tiny parser/interpreter that can read a file, build a lightweight AST, and execute a few basic statements.

This repository is licensed under the Apache License 2.0. See [`LICENSE`](LICENSE) for the full text.

## What It Can Do

- Tokenize identifiers, numbers, strings, booleans, operators, and punctuation
- Ignore `//` single-line comments and `/* ... */` block comments
- Parse variable declarations and reassignments
- Parse `print(...)` statements with multiple arguments
- Parse `input (...)` statements.
- Evaluate basic arithmetic expressions with `+`, `-`, `*`, `/`, and `%`

## Language Snapshot

The current language supports a small subset of statements:

- `int`, `double`, `string`, and `bool` variable declarations
- Reassignment with `name = expression;`
- `print(...)` for values and variables
- `input(...)` to accept user input for values.
- String literals with escape support for `\n`, `\t`, `\"`, and `\\`

Example:

```pd
int x = 10;
double y = 40.2;
string first_name = "Pranish";
string last_name;
bool ok = true;

int sum = x + 10;

input ("Lastname : " , last_name);
print("Hello," first_name);
print("Sum:" sum);
print(ok);
```

## Build

Use any C++17-compatible compiler. With `cmake`, build it like this:

```bash
cmake --build .
```

If you prefer a different output name, change `-o ppi` to whatever you want in the CmakeLists.txt.

## Run

Pass a `.pd` file to the executable (if compiled in release):

```bash
./build/Release/ppi examples/test.pd
```

On Windows, the command will look more like (Release):

```powershell
build\Release\ppi.exe examples\test.pd
```

If compiled in Debug : 

```bash
./build/Debug/ppi examples/test.pd
```

On Windows, the command will look like:
```powershell
 build\Debug\ppi.exe examples\test.pd
```

## Example File

`test.pd` contains a sample program that exercises declarations, arithmetic, comments, printing and input. It is a good starting point if you want to see the current syntax in action.

## Notes

- The interpreter is intentionally simple and only covers a subset of expressions and statements.
- `print(...)` currently outputs arguments separated by spaces.

## Project Files

- `main.cpp` - program entry point and statement dispatch
- `lexer.cpp` / `lexer.h` - tokenization
- `AST.cpp` / `AST.h` - parsing into AST nodes
- `interpreter.cpp` / `interpreter.h` - execution of parsed nodes
- `test.pd` - sample input file

## Status

This is still a work-in-progress toy language, but it is a solid base for experimenting with lexing, parsing, and interpretation in C++.
