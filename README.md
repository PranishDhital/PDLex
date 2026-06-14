# PDLex

PDLex is a small C++20 prototype for a simple `.pd` language. It started as a lexer and grew into a tiny parser/interpreter that can read a file, build a lightweight AST, and execute the supported statements.

This repository is licensed under the Apache License 2.0. See [`LICENSE`](LICENSE) for the full text.

For the language reference, see [`examples/syntax.md`](examples/syntax.md).

## Overview

PDLex currently supports:

- Lexing identifiers, numbers, strings, booleans, operators, and punctuation
- Single-line `//` comments and block `/* ... */` comments
- Variable declarations and reassignment
- `print(...)` and `printnl(...)`
- `input(...)`
- `if`, `else if`, and `else`
- `for` loops
- `while` loops
- Basic arithmetic, comparison, and equality expressions
- Postfix increment and decrement with `++` and `--`

## Language Support

### Keywords

The language currently recognizes these keywords:

- `int`
- `double`
- `string`
- `bool`
- `print`
- `printnl`
- `input`
- `if`
- `else`
- `for`
- `while`

Boolean values use `true` and `false`.

### Variables

Declare variables with a type and name, with or without an initializer:

```pd
int count;
int total = 10;
double ratio = 2.5;
string name = "PDLex";
bool ready = true;
```

Supported types:

- `int`
- `double`
- `string`
- `bool`

Reassignment uses `=`:

```pd
count = count + 1;
name = "new value";
ready = false;
```

Postfix increment and decrement are also supported:

```pd
count++;
count--;
```

### Expressions

Supported operators:

- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `<`, `>`, `<=`, `>=`
- Equality: `==`, `!=`
- Unary plus/minus: `+expr`, `-expr`

Operator precedence is:

1. Unary `+` and `-`
2. `*`, `/`, `%`
3. `+`, `-`
4. `<`, `>`, `<=`, `>=`
5. `==`, `!=`

### Printing

Use `print(...)` to output values separated by spaces:

```pd
print("Hello", name, count);
```

Use `printnl(...)` for the same style of output without a trailing newline:

```pd
printnl("Loading");
printnl(".");
printnl(".");
printnl(".");
```

### Input

Use `input(...)` to read user input into a variable.

Supported forms:

```pd
input(name);
input("Enter your name: ", name);
input("Enter age: ", age);
```

You can also use `input(...)` inside an assignment, for example:

```pd
int age = input("Enter age: ");
```

### Conditionals

Use `if`, `else if`, and `else` for branching:

```pd
if (score >= 90)
{
    print("A");
}
else if (score >= 80)
{
    print("B");
}
else
{
    print("C or lower");
}
```

General form:

```pd
if (condition)
{
    statements
}
else if (condition)
{
    statements
}
else
{
    statements
}
```

### Loops

`for` loops use three comma-separated parts inside the parentheses:

```pd
for (int i = 0, i < 5, i++)
{
    print(i);
}
```

General form:

```pd
for (declaration, condition, update)
{
    statements
}
```

The update slot currently supports postfix `++` and `--`.

`while` loops are also supported:

```pd
while (running)
{
    print("looping");
}
```

General form:

```pd
while (condition)
{
    statements
}
```

### Comments

Single-line comments:

```pd
// comment
```

Block comments:

```pd
/*
    comment
*/
```

## Example

```pd
int i = 0;
bool running = true;

while (running)
{
    printnl(i);
    i++;

    if (i >= 5)
    {
        running = false;
    }
}
```

## Build

Use any C++20-compatible compiler. With CMake, build the project like this:

```bash
cmake -S . -B build
cmake --build build
```

The executable target is named `ppi`.

## Run

Pass a `.pd` file to the executable:

```powershell
build\bin\Debug\ppi.exe examples\test.pd
```

```powershell
build\bin\Release\ppi.exe examples\test.pd
```

On Linux or macOS, the paths will look similar under `build/bin/Debug` or `build/bin/Release`.

## Example Files

- `test.pd` - general language coverage and mixed syntax
- `if.pd` - conditional branching examples
- `for.pd` - `for` loop examples, including nested loops
- `while.pd` - `while` loop examples
- `input.pd` - input handling examples
- `b_math.pd` - arithmetic and precedence checks
- `c_comparison.pd` - comparison operator checks
- `syntax.md` - language syntax reference

These files are the best starting point if you want to see the current syntax in action.

## Project Files

- `main.cpp` - program entry point and statement dispatch
- `lexer/lexer.cpp` and `lexer/lexer.h` - tokenization
- `ast/AST.cpp` and `ast/AST.h` - parsing into AST nodes
- `interpreter/interpreter.cpp` and `interpreter/interpreter.h` - execution of parsed nodes
- `error/error.cpp` and `error/error.h` - syntax and runtime error formatting

## Notes

- The interpreter is intentionally simple and only covers a subset of expressions and statements.
- `print(...)` and `printnl(...)` separate arguments with spaces.
- Comparison operators evaluate to `1` for true and `0` for false when used in expressions.
- `for` loop updates support only simple postfix increments and decrements.
- The language reference in [`examples/syntax.md`](examples/syntax.md) should be treated as the source of truth for supported forms.

## vscode Extension
The zip folder in the release contains a .vsix file which contains all the things to support syntax highlighting in vscode for the .pd files.

### Installation steps for .vsix :

The file name is `pdlex-0.0.1.vsix` 
- Step 1: `ctrl + shift + x` , to open vscode extension market
- Step 2: At the top right of the extension market there is a `...` press it 
- Step 3: Select Install from VSIX

- step 4: find and click the `pdlex-0.0.1.vsix`
- step 5: To find the extension in your extension market search `pdlex` , after installation

## Status

This is no longer getting updates from 1.4 onwards
