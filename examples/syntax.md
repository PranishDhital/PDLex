# PDLex Syntax Reference

This file documents the keywords and statement forms currently supported by the language.

## Keywords

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

Boolean values are written as `true` and `false`.

## Variable Declarations

Declare a variable with a type, a name, and an optional initializer:

```pd
int count;
int total = 10;
double ratio = 2.5;
string name = "PDLex";
bool ready = true;
```

General form:

```pd
type identifier;
type identifier = expression;
```

Supported types:

- `int`
- `double`
- `string`
- `bool`

## Assignment

Reassign an existing variable with `=`:

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

## Expressions

Supported expression operators:

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

Examples:

```pd
int a = 3 + 2 * 5;
bool b = 10 >= 5;
bool c = 7 != 9;
```

## Printing

Use `print(...)` to output values separated by spaces:

```pd
print("Hello", name, count);
```

Use `printnl(...)` when you want the same style of output without a trailing newline:

```pd
printnl("Loading");
printnl(".");
printnl(".");
printnl(".");
```

## Input

Use `input(...)` to read user input into a variable.

Supported forms:

```pd
input(name);
input("Enter your name: ", name);
input("Enter age: ", age);
```

## If Statements

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

## For Loops

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

## While Loops

Use `while` for repeated execution while a condition stays true:

```pd
while (running == true)
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

## Comments

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
