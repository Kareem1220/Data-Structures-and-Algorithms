# Project 3 — Word Frequency Counter
## AVL Tree + Hash Table (C)

A word frequency counter built in C using two parallel data structures — an **AVL self-balancing tree** and a **chained Hash Table** — both loaded from `input.txt`. Supports insert, delete, search, and statistics across both structures.

---

## Data Structures

### AVL Tree
A self-balancing Binary Search Tree storing words alphabetically. Each node tracks the word's **frequency** (how many times it appears). Rebalances on every insert/delete using single and double rotations.

```
Balance factor = Height(Left) - Height(Right)
+2 → Left-heavy  → SingleRotateLeft or DoubleRotateLeft
-2 → Right-heavy → SingleRotateRight or DoubleRotateRight
```

### Hash Table
An array of **11 linked lists** (chaining). Each word maps to a slot via a polynomial rolling hash. Stores word + frequency per node.

```
hashIndex(word) = (Σ hashValue << 5 + char) % 11
```

---

## Menu Operations

| Option | Operation |
|---|---|
| 1 | Load words from `input.txt` |
| 2 | Confirm AVL Tree is ready |
| 3 | Insert all file words into AVL Tree |
| 4 | Delete a word from AVL Tree |
| 5 | Print all words sorted (in-order traversal) |
| 6 | Confirm Hash Table is ready |
| 7 | Insert all file words into Hash Table |
| 8 | Delete a word from Hash Table |
| 9 | Search for a word and print its frequency |
| 10 | Print Hash Table statistics (unique words, most repeated) |
| 11 | Exit |

---

## Input Format (`input.txt`)
One word per token, any case (automatically converted to lowercase):
```
Hello world hello AVL tree TREE
```

---

## Compile & Run

```bash
gcc main.c -o wordfreq
./wordfreq
```

---

## Bugs Fixed

| # | Bug | Impact |
|---|---|---|
| 1 | `Hash H` never `malloc`'d | **Immediate crash** — dangling pointer dereferenced in `initHash` |
| 2 | `fscanf(..., word[Max_Length])` | **Out-of-bounds** — reading into a single char off the array end |
| 3 | `InsertAVL(tree1, &word[Max_Length])` | **Out-of-bounds address** passed as string |
| 4 | `InsertAVL` return value discarded | `tree1` root never updated — tree appeared permanently empty |
| 5 | `DeleteAVL` return value discarded | Same — deletions had no effect |
| 6 | `FindAVL` dereferenced `T->key` before NULL check | **Crash** on empty tree |
| 7 | `DeleteAVL` called twice in case 4 | **Double-deletion** — second call crashes or corrupts tree |
| 8 | `DeleteHash` called twice in case 8 | **Double-deletion** — same issue |
| 9 | Case 9 never prompted for `word` | Searched random garbage left in `word[]` |
| 10 | `InsertHash` used uninitialized `Temp->Frequency2++` | **Undefined behaviour** — `SearchHash` result discarded |
| 11 | `DeleteHash` logic inverted (`!=` instead of `==`) | Deleted the **wrong node** every time |
| 12 | `DeleteAVL` broken `if-else` chain | Balance rechecks ran unconditionally — wrong rotations |
| 13 | `T->Right->Right` accessed without NULL check | **Crash** during rebalancing on certain tree shapes |
| 14 | `hash()` used signed `int` → negative indices | **Negative array index** for high-ASCII characters |
| 15 | `while(1)` never re-prompted for `Decision` | **Same case ran forever** — program stuck after any choice |
| 16 | No NULL check on `file` before `fscanf` in cases 3 & 7 | Crash if file never opened |
| 17 | `MakeEmpty(tree1)` result not assigned | `tree1` pointed to freed memory after `MakeEmpty` |
