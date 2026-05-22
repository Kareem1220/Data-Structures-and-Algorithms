# Project 2 — Tournament Simulator
## Stack & Queue — Champion & Runner-Up Finder (C)

A tournament simulator using a **linked-list Stack** and a **linked-list Queue** implemented from scratch in C. Given a list of team IDs, the program runs alternating odd/even rounds to determine the **Champion** and **Runner-Up**.

---

## Tournament Rules

| Round Type | Winner |
|---|---|
| Odd round | Team with the **smaller** ID |
| Even round | Team with the **larger** ID |

Rounds alternate automatically. In each match, two teams at the front of the queue compete — the winner advances to the next round.

---

## How It Works

```
Teams entered → Queue
                  │
         champion(q)     → runs alternating rounds → returns Champion
         runnerUp(q)     → replays tournament, tracks all teams
                            that lost directly to Champion,
                            then runs champion() on those losers
                            → returns Runner-Up
```

---

## Data Structures

Both are **header-node linked lists**:

**Stack** (`node1`) — LIFO, used for duplicate detection and runner-up tracking
- `push()` — insert at top
- `pop()` — remove from top
- `topS()` — peek top

**Queue** (`node2`) — FIFO, used to simulate tournament rounds
- `enqueue()` — insert at rear
- `dequeue()` — remove from front
- `topQ()` — peek front

---

## Input Format

```
Enter number of teams: 4
Enter 4 IDs: 1001 2002 3003 4004
```

- IDs must be 4-digit integers (−9999 to 9999, excluding 0)
- IDs must be unique
- Minimum 2 teams required
- Enter `−9999` as team count to exit

---

## Compile & Run

```bash
gcc main.c -o tournament
./tournament
```

---

## Bugs Fixed

| Bug | Description |
|---|---|
| Off-by-one in `duplicatedInput()` | Stray `i++` after while loop caused comparison with uninitialized memory → false duplicate rejections |
| `new` variable name | Renamed to `newNode` (reserved keyword in C++) |
| Unreachable `free()` in `main()` | Moved cleanup inside loop before every return path |
| Memory leaks in `champion()` | Now properly drains and frees `q1` before returning |
| Memory leaks in `runnerUp()` | `findChamp` and `q1` are now freed after use |
| `dequeue()` null dereference | NULL check moved before dereferencing `q` |
| `enqueue()` null check order | NULL check on `newNode` moved before dereferencing it |
| Missing `\n` in messages | Fixed throughout |
| `count` not reset per iteration | Reset at top of `while(1)` loop |
