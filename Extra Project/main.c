#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max_Length 50
#define Hash_Size  11   /* prime — reduces collisions */

/* ─── AVL Tree node ──────────────────────────────────────────────────────── */

typedef struct node *AVL;
struct node {
    char key[Max_Length];
    AVL  Left, Right;
    int  Height;
    int  Frequency;
};

/* ─── Hash Table — array of linked-list heads ────────────────────────────── */

typedef struct node2 *List;
struct node2 {
    char key2[Max_Length];
    List Next;
    int  Frequency2;
};

typedef struct hash *Hash;
struct hash {
    List Table[Hash_Size];
};

/* ─── Prototypes ─────────────────────────────────────────────────────────── */

char *ToLower(char *X);
void  DisplayMainMenu(void);
void  PrintTree(AVL T);
AVL   MakeEmpty(AVL T);
AVL   FindAVL(AVL T, char *X);
AVL   FindMin(AVL T);
AVL   FindMax(AVL T);
int   Height(AVL P);
int   Max(int a, int b);
AVL   SingleRotateLeft(AVL K2);
AVL   SingleRotateRight(AVL K1);
AVL   DoubleRotateLeft(AVL K3);
AVL   DoubleRotateRight(AVL K1);
AVL   InsertAVL(AVL T, char *X);
AVL   DeleteAVL(AVL T, char *X);
int   hashIndex(char *key);
void  initHash(Hash H);
List  InitList(char *X);
void  InsertHash(Hash H, char *X);
List  SearchHash(Hash H, char *X);
int   DeleteHash(Hash H, char *X);
void  PrintHash(Hash H);

/* ─────────────────────────────────────────────────────────────────────────── */

int main(void)
{
    char word[Max_Length];
    int  Decision = 0;

    AVL  tree1 = MakeEmpty(NULL);

    /* FIX: Hash H was never malloc'd — was a dangling pointer that crashed on initHash */
    Hash H = (Hash)malloc(sizeof(struct hash));
    initHash(H);

    FILE *file = fopen("input.txt", "r");

    printf("------------------------------Welcome-----------------------------------------\n\n");

    while (1)
    {
        /* FIX: menu + scanf now at top of loop so Decision is refreshed every iteration */
        DisplayMainMenu();
        scanf("%d", &Decision);
        printf("\n---------------------------------------------------------------------------\n\n");

        while (Decision < 1 || Decision > 11)
        {
            printf("Nonexistent Operation, Choose A Different One.\n\n");
            DisplayMainMenu();
            scanf("%d", &Decision);
            printf("\n---------------------------------------------------------------------------\n\n");
        }

        switch (Decision)
        {
        /* ── 1. Check file ──────────────────────────────────────────────── */
        case 1:
            printf("Loading Data From The File...\n\n");
            if (file == NULL)
                printf("Error: input.txt Does Not Exist.\n\n");
            else
                printf("Data Has Been Loaded.\n\n");
            break;

        /* ── 2. Confirm AVL tree created ────────────────────────────────── */
        case 2:
            printf("AVL Tree Is Ready.\n\n");
            break;

        /* ── 3. Insert all words from file into AVL Tree ─────────────────── */
        case 3:
            printf("Inserting Words Into The AVL Tree...\n\n");
            if (file == NULL)           /* FIX: no NULL check in original */
            {
                printf("Error: File Not Loaded.\n\n");
                break;
            }
            rewind(file);
            /* FIX: was fscanf(file, "%s", word[Max_Length]) — out-of-bounds char.
               FIX: tree1 = InsertAVL(...) — original discarded the returned root */
            while (fscanf(file, "%s", word) != EOF)
            {
                ToLower(word);
                tree1 = InsertAVL(tree1, word);
            }
            printf("Words Inserted Into AVL Tree.\n\n");
            break;

        /* ── 4. Delete a word from AVL Tree ─────────────────────────────── */
        case 4:
            printf("Enter The Word You Wish To Delete: ");
            scanf("%s", word);
            ToLower(word);
            /* FIX: original called DeleteAVL twice — deletes the node twice */
            if (FindAVL(tree1, word) == NULL)
                printf("The Word \"%s\" Does Not Exist In The Tree.\n\n", word);
            else
            {
                tree1 = DeleteAVL(tree1, word);
                printf("The Word \"%s\" Has Been Deleted.\n\n", word);
            }
            break;

        /* ── 5. Print AVL Tree in order ──────────────────────────────────── */
        case 5:
            printf("Printing The AVL Tree (Sorted):\n\n");
            if (tree1 == NULL)
                printf("The Tree Is Empty.\n\n");
            else
            {
                PrintTree(tree1);
                printf("\n\n");
            }
            break;

        /* ── 6. Confirm hash table created ───────────────────────────────── */
        case 6:
            printf("Hash Table Is Ready.\n\n");
            break;

        /* ── 7. Insert all words from file into Hash Table ───────────────── */
        case 7:
            printf("Inserting Words Into The Hash Table...\n\n");
            if (file == NULL)           /* FIX: no NULL check in original */
            {
                printf("Error: File Not Loaded.\n\n");
                break;
            }
            rewind(file);
            /* FIX: was fscanf(file, "%s", word[Max_Length]) — out-of-bounds */
            while (fscanf(file, "%s", word) != EOF)
            {
                ToLower(word);
                InsertHash(H, word);
            }
            printf("Words Inserted Into Hash Table.\n\n");
            break;

        /* ── 8. Delete a word from Hash Table ────────────────────────────── */
        case 8:
            printf("Enter The Word You Wish To Delete: ");
            scanf("%s", word);
            ToLower(word);
            /* FIX: original called DeleteHash twice — double-deletion bug */
            if (DeleteHash(H, word) == -1)
                printf("The Word \"%s\" Does Not Exist In The Hash Table.\n\n", word);
            else
                printf("The Word \"%s\" Has Been Deleted.\n\n", word);
            break;

        /* ── 9. Search for a word in Hash Table ──────────────────────────── */
        case 9:
            /* FIX: original never prompted — used whatever was in word[] last */
            printf("Enter The Word To Search For: ");
            scanf("%s", word);
            ToLower(word);
            {
                List result = SearchHash(H, word);
                if (result == NULL)
                    printf("The Word \"%s\" Was Not Found.\n\n", word);
                else
                    printf("Found: \"%s\" — Repeated %d Time(s).\n\n",
                           result->key2, result->Frequency2);
            }
            break;

        /* ── 10. Print Hash Table statistics ────────────────────────────── */
        case 10:
            printf("Hash Table Statistics:\n\n");
            PrintHash(H);
            break;

        /* ── 11. Exit ────────────────────────────────────────────────────── */
        case 11:
            printf("Thank You For Using My Program!\n\n");
            if (file) fclose(file);
            free(H);
            return 0;

        default:
            printf("Invalid Input. Enter A Number Between 1 and 11.\n\n");
            break;
        }
    }
}

/* ─────────────────────────────────────────────────────────────────────────── */

char *ToLower(char *X)
{
    for (int i = 0; X[i] != '\0'; i++)
        if (X[i] >= 'A' && X[i] <= 'Z')
            X[i] += ('a' - 'A');
    return X;
}

void DisplayMainMenu(void)
{
    printf("Choose The Operation You Wish To Execute:\n\n");
    printf(" 1- Load The Data File.\n");
    printf(" 2- Create The AVL Tree.\n");
    printf(" 3- Insert All Words Into The AVL Tree.\n");
    printf(" 4- Delete A Word From The AVL Tree.\n");
    printf(" 5- Print Sorted Words In The AVL Tree.\n");
    printf(" 6- Create The Hash Table.\n");
    printf(" 7- Insert All Words Into The Hash Table.\n");
    printf(" 8- Delete A Word From The Hash Table.\n");
    printf(" 9- Search For A Word And Print Its Frequency.\n");
    printf("10- Print Hash Table Statistics.\n");
    printf("11- Exit.\n\n");
    printf("---------------------------------------------------------------------------\n\n");
}

/* ─── AVL Tree ───────────────────────────────────────────────────────────── */

AVL MakeEmpty(AVL T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

AVL FindAVL(AVL T, char *X)
{
    /* FIX: original dereferenced T->key before checking T == NULL — crash on empty tree */
    if (T == NULL) return NULL;
    int cmp = strcmp(X, T->key);
    if      (cmp < 0) return FindAVL(T->Left, X);
    else if (cmp > 0) return FindAVL(T->Right, X);
    else              return T;
}

void PrintTree(AVL T)
{
    if (T != NULL)
    {
        PrintTree(T->Left);
        printf("%s (x%d)  ", T->key, T->Frequency);
        PrintTree(T->Right);
    }
}

AVL FindMin(AVL T)
{
    if (T == NULL)        return NULL;
    if (T->Left == NULL)  return T;
    return FindMin(T->Left);
}

AVL FindMax(AVL T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;
    return T;
}

int Height(AVL P)
{
    return (P == NULL) ? -1 : P->Height;
}

int Max(int a, int b)
{
    return a > b ? a : b;
}

AVL SingleRotateLeft(AVL K2)
{
    AVL K1      = K2->Left;
    K2->Left    = K1->Right;
    K1->Right   = K2;
    K2->Height  = Max(Height(K2->Left),  Height(K2->Right)) + 1;
    K1->Height  = Max(Height(K1->Left),  K2->Height)        + 1;
    return K1;
}

AVL SingleRotateRight(AVL K1)
{
    AVL K2      = K1->Right;
    K1->Right   = K2->Left;
    K2->Left    = K1;
    K1->Height  = Max(Height(K1->Left),  Height(K1->Right)) + 1;
    K2->Height  = Max(Height(K2->Right), K1->Height)        + 1;
    return K2;
}

AVL DoubleRotateLeft(AVL K3)
{
    K3->Left = SingleRotateRight(K3->Left);
    return SingleRotateLeft(K3);
}

AVL DoubleRotateRight(AVL K1)
{
    K1->Right = SingleRotateLeft(K1->Right);
    return SingleRotateRight(K1);
}

AVL InsertAVL(AVL T, char *X)
{
    if (T == NULL)
    {
        T = (AVL)malloc(sizeof(struct node));
        if (T == NULL) { printf("Out of memory.\n"); return NULL; }
        strcpy(T->key, X);
        T->Height    = 0;
        T->Left      = T->Right = NULL;
        T->Frequency = 1;
    }
    else if (strcmp(X, T->key) < 0)
    {
        T->Left = InsertAVL(T->Left, X);
        if (Height(T->Left) - Height(T->Right) == 2)
            T = (strcmp(X, T->Left->key) < 0)
                ? SingleRotateLeft(T)
                : DoubleRotateLeft(T);
    }
    else if (strcmp(X, T->key) > 0)
    {
        T->Right = InsertAVL(T->Right, X);
        if (Height(T->Right) - Height(T->Left) == 2)
            T = (strcmp(X, T->Right->key) > 0)
                ? SingleRotateRight(T)
                : DoubleRotateRight(T);
    }
    else
        T->Frequency++;

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

AVL DeleteAVL(AVL T, char *X)
{
    /* FIX: original had broken if-else chain — balance checks ran unconditionally.
       FIX: T->Right->Right and T->Left->Left accessed without NULL checks. */
    if (T == NULL) return NULL;

    int cmp = strcmp(X, T->key);

    if (cmp < 0)
    {
        T->Left = DeleteAVL(T->Left, X);
        /* Rebalance if right subtree is now taller */
        if (Height(T->Right) - Height(T->Left) == 2)
        {
            if (T->Right->Right != NULL &&
                Height(T->Right->Right) >= Height(T->Right->Left))
                T = SingleRotateRight(T);
            else
                T = DoubleRotateRight(T);
        }
    }
    else if (cmp > 0)
    {
        T->Right = DeleteAVL(T->Right, X);
        /* Rebalance if left subtree is now taller */
        if (Height(T->Left) - Height(T->Right) == 2)
        {
            if (T->Left->Left != NULL &&
                Height(T->Left->Left) >= Height(T->Left->Right))
                T = SingleRotateLeft(T);
            else
                T = DoubleRotateLeft(T);
        }
    }
    else
    {
        /* Found the node to delete */
        if (T->Left != NULL && T->Right != NULL)
        {
            /* Two children: replace with in-order successor */
            AVL successor = FindMin(T->Right);
            strcpy(T->key, successor->key);
            T->Frequency = successor->Frequency;
            T->Right = DeleteAVL(T->Right, T->key);
        }
        else
        {
            /* Zero or one child */
            AVL old = T;
            T = (T->Left != NULL) ? T->Left : T->Right;
            free(old);
            return T;
        }
    }

    if (T != NULL)
        T->Height = Max(Height(T->Left), Height(T->Right)) + 1;

    return T;
}

/* ─── Hash Table ─────────────────────────────────────────────────────────── */

int hashIndex(char *key)
{
    unsigned int hashValue = 0;         /* FIX: signed int overflowed → negative indices */
    while (*key != '\0')
        hashValue = (hashValue << 5) + (unsigned char)(*key++);
    return (int)(hashValue % Hash_Size);
}

void initHash(Hash H)
{
    for (int i = 0; i < Hash_Size; i++)
        H->Table[i] = NULL;
}

List InitList(char *X)
{
    List Temp        = (List)malloc(sizeof(struct node2));
    strcpy(Temp->key2, X);
    Temp->Next       = NULL;
    Temp->Frequency2 = 1;
    return Temp;
}

void InsertHash(Hash H, char *X)
{
    /* FIX: original used uninitialized Temp->Frequency2++ — SearchHash result was discarded */
    List found = SearchHash(H, X);
    if (found != NULL)
    {
        found->Frequency2++;
        return;
    }
    int  Index      = hashIndex(X);
    List newNode    = InitList(X);
    newNode->Next   = H->Table[Index];
    H->Table[Index] = newNode;
}

List SearchHash(Hash H, char *X)
{
    List Temp = H->Table[hashIndex(X)];
    while (Temp != NULL)
    {
        if (strcmp(Temp->key2, X) == 0) return Temp;
        Temp = Temp->Next;
    }
    return NULL;
}

int DeleteHash(Hash H, char *X)
{
    /* FIX: original logic was INVERTED — while(strcmp != 0) deleted the wrong node.
       Correct: walk until we find the match, then unlink and free it. */
    int  Index = hashIndex(X);
    List Curr  = H->Table[Index];
    List Prev  = NULL;

    while (Curr != NULL)
    {
        if (strcmp(Curr->key2, X) == 0)
        {
            if (Prev != NULL) Prev->Next      = Curr->Next;
            else              H->Table[Index] = Curr->Next;
            free(Curr);
            return 1;
        }
        Prev = Curr;
        Curr = Curr->Next;
    }
    return -1;
}

void PrintHash(Hash H)
{
    int  j = 0;
    int  HighestFrequency = 0;
    char HighestWord[Max_Length] = "";

    for (int i = 0; i < Hash_Size; i++)
    {
        printf("Index [%2d]: ", i);
        List Temp = H->Table[i];
        if (Temp == NULL) printf("(empty)");
        while (Temp != NULL)
        {
            printf("\"%s\" x%d  ", Temp->key2, Temp->Frequency2);
            if (Temp->Frequency2 > HighestFrequency)
            {
                HighestFrequency = Temp->Frequency2;
                strcpy(HighestWord, Temp->key2);
            }
            if (Temp->Frequency2 == 1) j++;
            Temp = Temp->Next;
        }
        printf("\n---------------------------------------------------------\n");
    }
    printf("\nUnique Words (appear once): %d\n", j);
    if (HighestFrequency > 0)
        printf("Most Repeated: \"%s\" — %d Time(s).\n\n", HighestWord, HighestFrequency);
    else
        printf("Table Is Empty.\n\n");
}
