# Project 3 — District & Town Manager
## AVL Tree + Double Hashing (C)

A two-phase town management system built in C. Phase 1 manages towns in an **AVL self-balancing tree** loaded from `district.txt`. Phase 2 transfers the data into a **double-hashing hash table** with automatic rehashing, loaded from `towns.txt`.

---

## Phase 1 — AVL Tree

Stores towns sorted alphabetically by town name. Each node holds:

| Field | Type | Description |
|---|---|---|
| District | string | District name |
| Town | string | Town name (sort key) |
| Population | int | Town population |
| Elevation | int | Elevation in metres |
| HasMunicipality | string | `yes` or `no` |

### AVL Tree Menu

| Option | Operation |
|---|---|
| 1 | Insert a new town |
| 2 | Find and update a town's population / elevation / municipality |
| 3 | List all towns alphabetically (in-order traversal) |
| 4 | List towns with population above a threshold |
| 5 | List towns by municipality status (yes/no) |
| 6 | Delete a town |
| 7 | Save tree to `towns.txt` |
| 8 | Exit to Hash Table menu |

### Input File (`district.txt`)
```
Nablus:Nablus:150000:580:yes
Nablus:Beit_Furik:4000:400:no
Ramallah:Bireh:60000:860:yes
```

---

## Phase 2 — Double Hashing Table

Loads towns from `towns.txt` into a hash table using **double hashing** (open addressing) to resolve collisions. Auto-rehashes when load factor exceeds 0.70, growing through 26 pre-selected prime-based sizes up to 842,879,579.

```
h(k, i) = (h1(k) + i × h2(k)) % tableSize

h1(k) = polynomial_hash(k) % SIZE_LIST[n]
h2(k) = PRIME_LIST[n] - (polynomial_hash(k) % PRIME_LIST[n])
```

### Hash Table Menu

| Option | Operation |
|---|---|
| 1 | Print full hash table (index + value) |
| 2 | Print statistics (size, items, load factor) |
| 3 | Insert a town |
| 4 | Search for a town (reports collisions) |
| 5 | Delete a town (marks slot `DELETED` for probe chain integrity) |
| 6 | Save to `towns.txt` and exit |

---

## Compile & Run

```bash
gcc main.c -o townmanager
./townmanager
```

Requires `district.txt` in the same directory. If absent, starts with an empty tree.

---

## Bugs Fixed

| # | Bug | Impact |
|---|---|---|
| 1 | `DeleteTown` (two-children): direct pointer copy `T->Town = minNode->Town`, then recursive delete frees `minNode` | **Use-after-free** — `T->Town/District/HasMunicipality` become dangling pointers |
| 2 | `DeleteTown` updated height but never rebalanced | Tree became unbalanced after deletions |
| 3 | `hashFunction` only used first 5 characters | Towns sharing a prefix always hashed to same slot — poor distribution |
| 4 | `deleteHashValue` never decremented `currentSize` | Load factor grew incorrectly — triggered unnecessary rehashes |
| 5 | `h2` recomputed inside the probe loop every iteration | Wasted computation — `h2` doesn't depend on `i` |
| 6 | No bounds check on `currentRehash` in `reHash` | Array overflow if data exceeded all 26 size levels |
| 7 | `scanf("%s")` into 5-byte `HasMunicipality` buffer on update | Buffer overflow on long input |
