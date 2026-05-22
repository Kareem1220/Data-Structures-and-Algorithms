# Project 1 — Polynomial Operations
## Stack of Linked Lists — Add, Subtract, Multiply (C)

A polynomial calculator built in C using a **Stack of Linked Lists**. Each polynomial is stored as a sorted linked list of terms (coefficient + exponent), and the stack holds all loaded polynomials. Supports addition, subtraction, and multiplication of the top two polynomials, with file I/O.

---

## Data Structures

**Linked List** (`node2`) — one node per polynomial term, sorted by exponent (descending)

```
Header → [3, x^4] → [2, x^2] → [1, x^0] → NULL
```

**Stack** (`node1`) — each cell points to a polynomial linked list

```
Header → [→ poly3] → [→ poly2] → [→ poly1] → NULL
```

---

## Menu Operations

| Option | Operation |
|---|---|
| 1 | Load polynomials from `input.txt` |
| 2 | Print all loaded polynomials |
| 3 | Add top two polynomials |
| 4 | Subtract top two polynomials (second − first) |
| 5 | Multiply top two polynomials |
| 6 | Print result to screen |
| 7 | Save result to `output.txt` |
| 8 | Exit |

---

## Input Format (`input.txt`)

Each line is one polynomial. Terms are written as `coefficient x^ exponent`:

```
3x^4+2x^2+1x^0
5x^3-1x^1
```

---

## How Addition & Subtraction Work

The sorted structure lets us merge in one linear pass:

- **Equal exponents** → combine coefficients, advance both pointers
- **Left exponent larger** → copy left term, advance left only
- **Right exponent larger** → copy right term, advance right only

This ensures no terms are skipped regardless of how many terms each polynomial has.

---

## Compile & Run

```bash
gcc main.c -o polynomial
./polynomial
```

---

## Bugs Fixed

| Bug | Impact |
|---|---|
| `decision` uninitialized in `while(decision!=8)` | Undefined behaviour — program could skip the menu entirely |
| `readFromFile(t[MAXPOLY])` → `readFromFile(t)` | Out-of-bounds array access — memory corruption |
| `insertSorted()`: `=` instead of `==` | Assignment as condition — always evaluated as true, corrupted list order |
| `insertSorted()`: `t->next` dereferenced before NULL check | Crash on last element insertion |
| `case 5` multiply: `f2` uninitialized | Immediate crash / undefined behaviour |
| `char line[10]` in `readFromFile` | Buffer overflow on any real polynomial input |
| No `NULL` check after `fopen()` | Crash if `input.txt` is missing |
| Addition/Subtraction always advanced both pointers | Terms with unmatched exponents were silently dropped |
| `new` as variable name in `push()` | Reserved keyword in C++ |
| `printOnFile` printed from `l` regardless of stack state | Would print empty/wrong list if no operation was run |
