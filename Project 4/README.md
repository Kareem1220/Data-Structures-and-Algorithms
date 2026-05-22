# Project 4 — Network Shortest Path
## Dijkstra + BFS on a Weighted Graph (C)

A shortest-path finder for a network of routers, built in C using an **adjacency matrix graph**, a **Min-Heap** for Dijkstra's greedy algorithm, and a **Queue** for BFS traversal. Loads the graph from a file and outputs results to both screen and file.

---

## Data Structures

**Adjacency Matrix** — `int[100][100]`, initialized to `INT_MAX` (no edge) or `0` (self). Edge weights are filled from file.

**Min-Heap** — array-based binary heap used by Dijkstra to always extract the lowest-cost unvisited vertex next (greedy property).

**Queue** — array-based FIFO used by BFS to explore vertices level by level.

---

## Algorithms

### Dijkstra's Algorithm
Greedy shortest-path — always processes the vertex with the lowest known cost first via the Min-Heap. Guarantees the optimal path in graphs with non-negative weights.

```
1. Set all costs to ∞, source cost = 0
2. Insert source into Min-Heap
3. While heap is not empty:
   a. Extract min-cost vertex u
   b. For each neighbor v of u:
      if cost[u] + weight(u,v) < cost[v]:
         update cost[v], prev[v] = u
         insert v into heap
4. Trace back path via prev[]
```

### BFS (Weighted)
Explores vertices by hop count (level by level). Tracks actual edge weights for cost calculation, but path order is BFS-determined — not necessarily cost-optimal like Dijkstra.

---

## Input Format (`routers.txt`)

Each line: `SourceVertex-DestinationVertex-Weight`

```
A-B-4
A-C-2
B-D-5
C-D-1
```

Vertices are single characters. Edges are undirected (both directions set automatically).

---

## Menu

| Option | Description |
|---|---|
| 1 | Load graph from `routers.txt` |
| 2 | Set source vertex |
| 3 | Set destination & run both algorithms |
| 4 | Save results to `shortest_distance.txt` & exit |

---

## Compile & Run

```bash
gcc main.c -o router
./router
```

---

## Output File (`shortest_distance.txt`)

```
Shortest Path Results
Source      : A
Destination : D

Dijkstra's Algorithm:
The Path : A -> C -> D
Total Cost : 3

BFS Algorithm:
The Path : A -> B -> D
Total Cost : 9
```

---

## Bugs Fixed

| # | Bug | Impact |
|---|---|---|
| 1 | `adjacencyMatrix` global zero-init (not `INT_MAX`) at start | **Phantom zero-weight edges everywhere** if graph not loaded — completely wrong Dijkstra/BFS results |
| 2 | `fscanf("%c-%c-%d")` — `%c` reads leftover newlines | **Wrong vertex characters** read from file |
| 3 | No `graphLoaded` guard in cases 2 & 3 | Confusing "vertex not found" error when file was never loaded |
| 4 | `deleteMinHeap` had no empty-heap guard | **Undefined behaviour** — accessed `array[0]` on empty heap |
| 5 | `WriteToFile` iterated path forward with `<-` arrows | Printed path **destination→source** in file, opposite of screen output |
| 6 | `return 1` on successful exit | Wrong exit code — convention is `return 0` |
