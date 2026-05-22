#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── Hash Table ─────────────────────────────────────────────────────────── */

#define MAX_REHASH 26
#define EMPTY   NULL
#define DELETED ((char *)-1)

int SIZE_LIST[MAX_REHASH]  = {11,47,97,197,397,797,1597,3203,6421,12853,25717,51437,102877,205759,411527,823117,1646237,3292489,6584983,13169977,26339969,52679969,105359939,210719881,421439783,842879579};
int PRIME_LIST[MAX_REHASH] = {7, 43,89,193,389,787,1583,3191,6397,12841,25703,51431,102871,205753,411503,823103,1646221,3292463,6584953,13169959,26339929,52679921,105359933,210719863,421439719,842879569};

struct hashNode;
typedef struct hashNode *hash;

struct hashNode {
    char **array;
    int   currentSize;
    int   currentRehash;
};

/* ─── Hash helpers ───────────────────────────────────────────────────────── */

unsigned long hashFunction(char *str, int size)
{
    /* FIX: original only hashed first 5 chars — towns sharing a prefix
       always collided. Now hashes the entire string.                    */
    unsigned long h = 0;
    while (*str != '\0')
        h = (h * 31 + (unsigned char)*str++) % size;
    return h;
}

void initializeHash(hash h)
{
    for (int i = 0; i < SIZE_LIST[h->currentRehash]; i++)
        h->array[i] = EMPTY;
}

hash MakeEmptyHash(void)
{
    hash h          = (hash)malloc(sizeof(struct hashNode));
    h->currentSize  = 0;
    h->currentRehash = 0;
    h->array        = (char **)malloc(sizeof(char *) * SIZE_LIST[0]);
    initializeHash(h);
    return h;
}

void insertToHash(char *x, hash h);   /* forward declaration for reHash */

void reHash(hash h)
{
    /* FIX: added bounds check — original would overflow SIZE_LIST/PRIME_LIST */
    if (h->currentRehash + 1 >= MAX_REHASH)
    {
        printf("Hash table at maximum capacity — cannot rehash further.\n");
        return;
    }

    int oldRehash = h->currentRehash;
    int oldSize   = SIZE_LIST[oldRehash];

    /* Save live entries */
    char **oldArray = h->array;

    h->currentRehash++;
    h->currentSize = 0;
    h->array = (char **)malloc(sizeof(char *) * SIZE_LIST[h->currentRehash]);
    initializeHash(h);

    for (int i = 0; i < oldSize; i++)
    {
        if (oldArray[i] != EMPTY && oldArray[i] != DELETED)
        {
            insertToHash(oldArray[i], h);
            free(oldArray[i]);
        }
    }
    free(oldArray);
}

void insertToHash(char *x, hash h)
{
    if ((double)h->currentSize / SIZE_LIST[h->currentRehash] > 0.70)
        reHash(h);

    /* FIX: h2 computed once before loop — original recomputed it every iteration */
    unsigned long h2 = PRIME_LIST[h->currentRehash]
                     - (hashFunction(x, PRIME_LIST[h->currentRehash])
                        % PRIME_LIST[h->currentRehash]);

    int tableSize = SIZE_LIST[h->currentRehash];
    for (int i = 0; i < tableSize; i++)
    {
        int loc = (int)((hashFunction(x, tableSize) + (unsigned long)i * h2) % tableSize);
        if (h->array[loc] == EMPTY || h->array[loc] == DELETED)
        {
            h->array[loc] = strdup(x);
            h->currentSize++;
            return;
        }
    }
    printf("Hash table full — could not insert \"%s\".\n", x);
}

void deleteHashValue(char *x, hash h)
{
    unsigned long h2 = PRIME_LIST[h->currentRehash]
                     - (hashFunction(x, PRIME_LIST[h->currentRehash])
                        % PRIME_LIST[h->currentRehash]);

    int tableSize = SIZE_LIST[h->currentRehash];
    for (int i = 0; i < tableSize; i++)
    {
        int loc = (int)((hashFunction(x, tableSize) + (unsigned long)i * h2) % tableSize);
        if (h->array[loc] == EMPTY)
        {
            printf("Town \"%s\" not found in hash table.\n\n", x);
            return;
        }
        if (h->array[loc] != DELETED && strcmp(h->array[loc], x) == 0)
        {
            free(h->array[loc]);
            h->array[loc] = DELETED;
            h->currentSize--;   /* FIX: original never decremented — load factor was wrong */
            printf("Town \"%s\" deleted.\n\n", x);
            return;
        }
    }
    printf("Town \"%s\" not found in hash table.\n\n", x);
}

void searchHashValue(char *x, hash h)
{
    unsigned long h2 = PRIME_LIST[h->currentRehash]
                     - (hashFunction(x, PRIME_LIST[h->currentRehash])
                        % PRIME_LIST[h->currentRehash]);

    int tableSize  = SIZE_LIST[h->currentRehash];
    int collisions = 0;
    for (int i = 0; i < tableSize; i++)
    {
        int loc = (int)((hashFunction(x, tableSize) + (unsigned long)i * h2) % tableSize);
        if (h->array[loc] == EMPTY)
        {
            printf("\"%s\" not found.\n\n", x);
            return;
        }
        if (h->array[loc] != DELETED && strcmp(h->array[loc], x) == 0)
        {
            printf("\"%s\" found at index %d with %d collision(s).\n\n",
                   x, loc, collisions);
            return;
        }
        collisions++;
    }
    printf("\"%s\" not found.\n\n", x);
}

void printHash(hash h)
{
    printf("%-6s  Value\n", "Index");
    for (int i = 0; i < SIZE_LIST[h->currentRehash]; i++)
    {
        if      (h->array[i] == DELETED) printf("%-5d:  DELETED\n", i);
        else if (h->array[i] == EMPTY)   printf("%-5d:  EMPTY\n",   i);
        else                              printf("%-5d:  %s\n", i, h->array[i]);
    }
}

void printHashStats(hash h)
{
    printf("Table size  : %d\n", SIZE_LIST[h->currentRehash]);
    printf("Items       : %d\n", h->currentSize);
    printf("Load factor : %.2f\n\n",
           (double)h->currentSize / SIZE_LIST[h->currentRehash]);
}

void saveHashToFile(hash h)
{
    FILE *file = fopen("towns.txt", "w");
    if (!file) { printf("Could not open towns.txt for writing.\n\n"); return; }
    for (int i = 0; i < SIZE_LIST[h->currentRehash]; i++)
        if (h->array[i] != EMPTY && h->array[i] != DELETED)
            fprintf(file, "%s\n", h->array[i]);
    fclose(file);
}

void freeHash(hash h)
{
    for (int i = 0; i < SIZE_LIST[h->currentRehash]; i++)
        if (h->array[i] != EMPTY && h->array[i] != DELETED)
            free(h->array[i]);
    free(h->array);
    free(h);
}

/* ─── AVL Tree ───────────────────────────────────────────────────────────── */

struct AVLnode;
typedef struct AVLnode *AVLNode;

struct AVLnode {
    char   *District;
    char   *Town;
    int     Population;
    int     Elevation;
    char   *HasMunicipality;
    AVLNode Left, Right;
    int     Height;
};

int Height(AVLNode P)
{
    return (P == NULL) ? -1 : P->Height;
}

int Max(int a, int b) { return a > b ? a : b; }

AVLNode SingleRotateWithLeft(AVLNode K2)
{
    AVLNode K1  = K2->Left;
    K2->Left    = K1->Right;
    K1->Right   = K2;
    K2->Height  = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height  = Max(Height(K1->Left), K2->Height)        + 1;
    return K1;
}

AVLNode SingleRotateWithRight(AVLNode K1)
{
    AVLNode K2  = K1->Right;
    K1->Right   = K2->Left;
    K2->Left    = K1;
    K1->Height  = Max(Height(K1->Left),  Height(K1->Right)) + 1;
    K2->Height  = Max(Height(K2->Right), K1->Height)        + 1;
    return K2;
}

AVLNode DoubleRotateWithLeft(AVLNode K3)
{
    K3->Left = SingleRotateWithRight(K3->Left);
    return SingleRotateWithLeft(K3);
}

AVLNode DoubleRotateWithRight(AVLNode K1)
{
    K1->Right = SingleRotateWithLeft(K1->Right);
    return SingleRotateWithRight(K1);
}

AVLNode MakeEmpty(AVLNode T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T->District);
        free(T->Town);
        free(T->HasMunicipality);
        free(T);
    }
    return NULL;
}

AVLNode Find(char *Town, AVLNode T)
{
    if (T == NULL) return NULL;
    int cmp = strcmp(Town, T->Town);
    if      (cmp < 0) return Find(Town, T->Left);
    else if (cmp > 0) return Find(Town, T->Right);
    else              return T;
}

AVLNode FindMin(AVLNode T)
{
    if (T == NULL)       return NULL;
    if (T->Left == NULL) return T;
    return FindMin(T->Left);
}

AVLNode FindMax(AVLNode T)
{
    if (T != NULL)
        while (T->Right != NULL) T = T->Right;
    return T;
}

AVLNode Insert(char *District, char *Town, int Population,
               int Elevation, char *HasMunicipality, AVLNode T)
{
    if (T == NULL)
    {
        T = (AVLNode)malloc(sizeof(struct AVLnode));
        if (T == NULL) { printf("Out of memory.\n"); return NULL; }
        T->District        = strdup(District);
        T->Town            = strdup(Town);
        T->Population      = Population;
        T->Elevation       = Elevation;
        T->HasMunicipality = strdup(HasMunicipality);
        T->Height          = 0;
        T->Left = T->Right = NULL;
        return T;
    }

    int cmp = strcmp(Town, T->Town);
    if (cmp < 0)
    {
        T->Left = Insert(District, Town, Population, Elevation, HasMunicipality, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
            T = (strcmp(Town, T->Left->Town) < 0)
                ? SingleRotateWithLeft(T)
                : DoubleRotateWithLeft(T);
    }
    else if (cmp > 0)
    {
        T->Right = Insert(District, Town, Population, Elevation, HasMunicipality, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
            T = (strcmp(Town, T->Right->Town) > 0)
                ? SingleRotateWithRight(T)
                : DoubleRotateWithRight(T);
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

AVLNode DeleteTown(char *Town, AVLNode T)
{
    if (T == NULL) return NULL;

    int cmp = strcmp(Town, T->Town);
    if (cmp < 0)
        T->Left = DeleteTown(Town, T->Left);
    else if (cmp > 0)
        T->Right = DeleteTown(Town, T->Right);
    else
    {
        if (T->Left == NULL)
        {
            AVLNode temp = T->Right;
            free(T->District); free(T->Town); free(T->HasMunicipality); free(T);
            return temp;
        }
        else if (T->Right == NULL)
        {
            AVLNode temp = T->Left;
            free(T->District); free(T->Town); free(T->HasMunicipality); free(T);
            return temp;
        }
        else
        {
            /* FIX: original did T->Town = minNode->Town (direct pointer copy),
               then DeleteTown freed minNode — leaving T->Town as a dangling
               pointer (use-after-free). Must strdup before the recursive delete,
               and free the old strings first.                                    */
            AVLNode minNode = FindMin(T->Right);

            char *newDistrict = strdup(minNode->District);
            char *newTown     = strdup(minNode->Town);
            char *newMuni     = strdup(minNode->HasMunicipality);
            int   newPop      = minNode->Population;
            int   newElev     = minNode->Elevation;

            T->Right = DeleteTown(minNode->Town, T->Right);   /* frees minNode */

            free(T->District);        T->District        = newDistrict;
            free(T->Town);            T->Town            = newTown;
            free(T->HasMunicipality); T->HasMunicipality = newMuni;
            T->Population  = newPop;
            T->Elevation   = newElev;
        }
    }

    /* FIX: rebalance after deletion — original updated height but never rotated */
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;

    int balance = Height(T->Left) - Height(T->Right);
    if (balance == 2)
        T = (Height(T->Left->Left) >= Height(T->Left->Right))
            ? SingleRotateWithLeft(T)
            : DoubleRotateWithLeft(T);
    else if (balance == -2)
        T = (Height(T->Right->Right) >= Height(T->Right->Left))
            ? SingleRotateWithRight(T)
            : DoubleRotateWithRight(T);

    return T;
}

void PrintInOrder(AVLNode t)
{
    if (t == NULL) return;
    PrintInOrder(t->Left);
    printf("District: %-15s Town: %-20s Pop: %-8d Elev: %-6d Municipality: %s\n",
           t->District, t->Town, t->Population, t->Elevation, t->HasMunicipality);
    PrintInOrder(t->Right);
}

void SaveToFile(AVLNode t, FILE *file)
{
    if (t == NULL) return;
    SaveToFile(t->Left, file);
    fprintf(file, "%s:%s:%d:%d:%s\n",
            t->District, t->Town, t->Population, t->Elevation, t->HasMunicipality);
    SaveToFile(t->Right, file);
}

void printHigherPopulation(AVLNode tree, int minPop)
{
    if (tree == NULL) return;
    printHigherPopulation(tree->Left, minPop);
    if (tree->Population > minPop)
        printf("District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n\n",
               tree->District, tree->Town, tree->Population,
               tree->Elevation, tree->HasMunicipality);
    printHigherPopulation(tree->Right, minPop);
}

void printByMunicipality(AVLNode tree, char *municipality)
{
    if (tree == NULL) return;
    printByMunicipality(tree->Left, municipality);
    if (strcmp(tree->HasMunicipality, municipality) == 0)
        printf("District: %s, Town: %s, Population: %d, Elevation: %d, Municipality: %s\n\n",
               tree->District, tree->Town, tree->Population,
               tree->Elevation, tree->HasMunicipality);
    printByMunicipality(tree->Right, municipality);
}

/* ─── Main ───────────────────────────────────────────────────────────────── */

int main(void)
{
    AVLNode tree = NULL;

    /* Load AVL tree from district.txt */
    FILE *in = fopen("district.txt", "r");
    if (in != NULL)
    {
        char district[50], town[50], hasMunicipality[10];
        int  population, elevation;
        while (fscanf(in, "%49[^:]:%49[^:]:%d:%d:%9[^\n]\n",
                      district, town, &population, &elevation, hasMunicipality) == 5)
            tree = Insert(district, town, population, elevation, hasMunicipality, tree);
        fclose(in);
    }
    else
        printf("district.txt not found — starting with empty tree.\n\n");

    int  choice;
    char buf[100];

    /* ── AVL Tree Menu ─────────────────────────────────────────────────── */
    while (1)
    {
        printf("\n── AVL Tree Menu ──────────────────────────────\n");
        printf("1. Insert a new town\n");
        printf("2. Find and update a town\n");
        printf("3. List all towns (alphabetical)\n");
        printf("4. List towns with population above threshold\n");
        printf("5. List towns by municipality status\n");
        printf("6. Delete a town\n");
        printf("7. Save tree to file\n");
        printf("8. Exit to Hash Table menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            char district[50], town[50], hasMunicipality[10];
            int  population, elevation;
            printf("Enter: district town population elevation municipality(yes/no): ");
            scanf("%49s %49s %d %d %9s", district, town, &population, &elevation, hasMunicipality);
            tree = Insert(district, town, population, elevation, hasMunicipality, tree);
        }
        else if (choice == 2)
        {
            char town[50];
            printf("Enter town name: ");
            scanf("%49s", town);
            AVLNode t = Find(town, tree);
            if (t != NULL)
            {
                char newMuni[10];
                printf("Current — Pop: %d  Elev: %d  Municipality: %s\n",
                       t->Population, t->Elevation, t->HasMunicipality);
                printf("Enter new population, elevation, municipality: ");
                /* FIX: original wrote into t->HasMunicipality directly (5-byte strdup buffer)
                   — overflow if user typed a long string. Read into separate buffer first. */
                scanf("%d %d %9s", &t->Population, &t->Elevation, newMuni);
                free(t->HasMunicipality);
                t->HasMunicipality = strdup(newMuni);
            }
            else
                printf("Town \"%s\" not found.\n\n", town);
        }
        else if (choice == 3)
            PrintInOrder(tree);
        else if (choice == 4)
        {
            int minPop;
            printf("Enter population threshold: ");
            scanf("%d", &minPop);
            printHigherPopulation(tree, minPop);
        }
        else if (choice == 5)
        {
            char municipality[10];
            printf("Enter municipality status (yes/no): ");
            scanf("%9s", municipality);
            printByMunicipality(tree, municipality);
        }
        else if (choice == 6)
        {
            char town[50];
            printf("Enter town to delete: ");
            scanf("%49s", town);
            tree = DeleteTown(town, tree);
        }
        else if (choice == 7)
        {
            FILE *out = fopen("towns.txt", "w");
            if (out) { SaveToFile(tree, out); fclose(out); printf("Saved to towns.txt.\n"); }
            else       printf("Could not open towns.txt for writing.\n");
        }
        else if (choice == 8)
        {
            printf("Switching to Hash Table menu...\n\n");
            break;
        }
        else
            printf("Invalid choice.\n\n");
    }

    /* ── Hash Table Menu ───────────────────────────────────────────────── */
    hash h = MakeEmptyHash();

    /* Load towns.txt into hash table */
    FILE *tf = fopen("towns.txt", "r");
    if (tf)
    {
        char town[100];
        while (fgets(town, sizeof(town), tf))
        {
            town[strcspn(town, "\n")] = '\0';
            if (town[0] != '\0') insertToHash(town, h);
        }
        fclose(tf);
    }

    do {
        printf("\n── Hash Table Menu ────────────────────────────\n");
        printf("1. Print hash table\n");
        printf("2. Print hash statistics\n");
        printf("3. Insert a town\n");
        printf("4. Search for a town\n");
        printf("5. Delete a town\n");
        printf("6. Save and exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1: printHash(h);       break;
        case 2: printHashStats(h);  break;
        case 3:
            printf("Town name: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = '\0';
            insertToHash(buf, h);
            break;
        case 4:
            printf("Search for: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = '\0';
            searchHashValue(buf, h);
            break;
        case 5:
            printf("Delete town: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = '\0';
            deleteHashValue(buf, h);
            break;
        case 6:
            saveHashToFile(h);
            printf("Saved to towns.txt. Exiting...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 6);

    freeHash(h);
    tree = MakeEmpty(tree);
    return 0;
}
