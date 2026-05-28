# PDLex

PDLex is a small C++ prototype for a simple `.pd` language. It started as a lexer and grew into a tiny parser/interpreter that can read a file, build a lightweight AST, and execute a few basic statements.

## What It Can Do

- Tokenize identifiers, numbers, strings, booleans, operators, and punctuation
- Ignore `//` single-line comments and `/* ... */` block comments
- Parse variable declarations and reassignments
- Parse `print(...)` statements with multiple arguments
- Evaluate basic arithmetic expressions with `+`, `-`, `*`, `/`, and `%`

## Language Snapshot

The current language supports a small subset of statements:

- `int`, `double`, `string`, and `bool` variable declarations
- Reassignment with `name = expression;`
- `print(...)` for values and variables
- String literals with escape support for `\n`, `\t`, `\"`, and `\\`

Example:

```pd
int x = 10;
double y = 40.2;
string name = "Pranish";
bool ok = true;

int sum = x + 10;

print("Hello," name);
print("Sum:" sum);
print(ok);
```

## Build

Use any C++17-compatible compiler. With `g++`, build it like this:

```bash
g++ -std=c++17 main.cpp lexer.cpp AST.cpp interpreter.cpp -o ppi
```

If you prefer a different output name, change `-o ppi` to whatever you want.

## Run

Pass a `.pd` file to the executable:

```bash
./ppi test.pd
```

On Windows, the command will look more like:

```powershell
.\ppi.exe test.pd
```

## Example File

`test.pd` contains a sample program that exercises declarations, arithmetic, comments, and printing. It is a good starting point if you want to see the current syntax in action.

## Notes

- The interpreter is intentionally simple and only covers a subset of expressions and statements.
- Numeric expressions are evaluated as integers internally, so some decimal values may be truncated during arithmetic.
- `print(...)` currently outputs arguments separated by spaces.

## Project Files

- `main.cpp` - program entry point and statement dispatch
- `lexer.cpp` / `lexer.h` - tokenization
- `AST.cpp` / `AST.h` - parsing into AST nodes
- `interpreter.cpp` / `interpreter.h` - execution of parsed nodes
- `test.pd` - sample input file

## Status

This is still a work-in-progress toy language, but it is a solid base for experimenting with lexing, parsing, and interpretation in C++.
