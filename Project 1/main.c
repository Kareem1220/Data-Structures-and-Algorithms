#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPOLY  10   /* max polynomials loaded from file        */
#define MAXLINE  256  /* max characters per line in input file   */

/* ─── Linked-List node (one polynomial term) ─────────────────────────────── */

struct node2;
typedef struct node2 *ptr2;

struct node2 {
    int  coefficient;
    int  exponent;
    ptr2 next;
};

typedef ptr2 list;
typedef ptr2 pos;

/* ─── Stack node (each cell points to a polynomial linked-list) ─────────── */

struct node1;
typedef struct node1 *ptr1;

struct node1 {
    ptr1 next;
    ptr2 below;
};

typedef ptr1 stack;

/* ─── Prototypes ─────────────────────────────────────────────────────────── */

void  push(stack s, ptr2 p);
void  pop(stack s);
int   isEmpty(stack s);
ptr1  top(stack s);
void  insertSorted(list l, ptr2 temp);
void  displayList(list l);
void  printMenu(void);
stack readFromFile(list t[]);
void  printOnFile(list l);
list  newList(void);

/* ─────────────────────────────────────────────────────────────────────────── */

int main(void)
{
    int decision = 0;   /* FIX: was uninitialized — undefined behaviour on first check */

    stack s1 = NULL, s2 = NULL;
    list  p1, p2, p3, p4, l;
    list  t[MAXPOLY];

    while (decision != 8)
    {
        printMenu();
        scanf("%d", &decision);

        switch (decision)
        {
        /* ── 1. Load from file ─────────────────────────────────────────── */
        case 1:
            s1 = readFromFile(t);   /* FIX: was readFromFile(t[MAXPOLY]) — out-of-bounds */
            printf("\nThe Data From The File Has Been Loaded.\n\n");
            break;

        /* ── 2. Print loaded polynomials ───────────────────────────────── */
        case 2:
            printf("\nPrinting The File Values to the Screen...\n\n");
            s1 = readFromFile(t);
            while (!isEmpty(s1))
            {
                s2 = top(s1);
                displayList(s2->below);
                printf("\n-------------------------------------------\n");
                pop(s1);
            }
            break;

        /* ── 3. Add top two polynomials ────────────────────────────────── */
        case 3:
            s1 = readFromFile(t);
            l  = newList();
            printf("\nAdding The Polynomials...\n\n");

            s2 = top(s1);  p1 = s2->below;  pop(s1);
            s2 = top(s1);  p2 = s2->below;  pop(s1);

            p3 = p1->next;
            p4 = p2->next;

            /* FIX: advance only the pointer with the larger exponent
               when exponents differ, so no terms are skipped            */
            while (p3 != NULL && p4 != NULL)
            {
                if (p3->exponent == p4->exponent)
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = p3->coefficient + p4->coefficient;
                    term->exponent    = p3->exponent;
                    term->next        = NULL;
                    insertSorted(l, term);
                    p3 = p3->next;
                    p4 = p4->next;
                }
                else if (p3->exponent > p4->exponent)
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = p3->coefficient;
                    term->exponent    = p3->exponent;
                    term->next        = NULL;
                    insertSorted(l, term);
                    p3 = p3->next;
                }
                else
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = p4->coefficient;
                    term->exponent    = p4->exponent;
                    term->next        = NULL;
                    insertSorted(l, term);
                    p4 = p4->next;
                }
            }
            while (p3 != NULL)
            {
                ptr2 term = (ptr2)malloc(sizeof(struct node2));
                term->coefficient = p3->coefficient;
                term->exponent    = p3->exponent;
                term->next        = NULL;
                insertSorted(l, term);
                p3 = p3->next;
            }
            while (p4 != NULL)
            {
                ptr2 term = (ptr2)malloc(sizeof(struct node2));
                term->coefficient = p4->coefficient;
                term->exponent    = p4->exponent;
                term->next        = NULL;
                insertSorted(l, term);
                p4 = p4->next;
            }

            push(s1, l);
            printf("\nThe Polynomials Have Been Added.\n\n");
            break;

        /* ── 4. Subtract top from second-top ───────────────────────────── */
        case 4:
            s1 = readFromFile(t);
            l  = newList();
            printf("\nSubtracting The Polynomials...\n\n");

            s2 = top(s1);  p1 = s2->below;  pop(s1);
            s2 = top(s1);  p2 = s2->below;  pop(s1);

            p3 = p1->next;
            p4 = p2->next;

            /* p2 - p1  (second - first, matching the original intent) */
            while (p3 != NULL && p4 != NULL)
            {
                if (p3->exponent == p4->exponent)
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = p4->coefficient - p3->coefficient;
                    term->exponent    = p4->exponent;
                    term->next        = NULL;
                    insertSorted(l, term);
                    p3 = p3->next;
                    p4 = p4->next;
                }
                else if (p3->exponent > p4->exponent)
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = -(p3->coefficient);
                    term->exponent    = p3->exponent;
                    term->next        = NULL;
                    insertSorted(l, term);
                    p3 = p3->next;
                }
                else
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = p4->coefficient;
                    term->exponent    = p4->exponent;
                    term->next        = NULL;
                    insertSorted(l, term);
                    p4 = p4->next;
                }
            }
            while (p3 != NULL)
            {
                ptr2 term = (ptr2)malloc(sizeof(struct node2));
                term->coefficient = -(p3->coefficient);
                term->exponent    = p3->exponent;
                term->next        = NULL;
                insertSorted(l, term);
                p3 = p3->next;
            }
            while (p4 != NULL)
            {
                ptr2 term = (ptr2)malloc(sizeof(struct node2));
                term->coefficient = p4->coefficient;
                term->exponent    = p4->exponent;
                term->next        = NULL;
                insertSorted(l, term);
                p4 = p4->next;
            }

            push(s1, l);
            printf("\nThe Polynomials Have Been Subtracted.\n\n");
            break;

        /* ── 5. Multiply top two polynomials ───────────────────────────── */
        case 5:
            s1 = readFromFile(t);
            l  = newList();
            printf("\nMultiplying The Polynomials...\n\n");

            s2 = top(s1);  p1 = s2->below;  pop(s1);
            s2 = top(s1);  p2 = s2->below;  pop(s1);

            /* FIX: correct polynomial multiplication — every term of p1
               by every term of p2; f2 was previously uninitialized       */
            for (p3 = p1->next; p3 != NULL; p3 = p3->next)
            {
                for (p4 = p2->next; p4 != NULL; p4 = p4->next)
                {
                    ptr2 term = (ptr2)malloc(sizeof(struct node2));
                    term->coefficient = p3->coefficient * p4->coefficient;
                    term->exponent    = p3->exponent    + p4->exponent;
                    term->next        = NULL;

                    /* Merge with existing term of same exponent if present */
                    pos cur = l->next;
                    int merged = 0;
                    while (cur != NULL)
                    {
                        if (cur->exponent == term->exponent)
                        {
                            cur->coefficient += term->coefficient;
                            free(term);
                            merged = 1;
                            break;
                        }
                        cur = cur->next;
                    }
                    if (!merged)
                        insertSorted(l, term);
                }
            }

            push(s1, l);
            printf("\nThe Polynomials Have Been Multiplied.\n\n");
            break;

        /* ── 6. Print result to screen ─────────────────────────────────── */
        case 6:
            printf("\nPrinting The Results To The Screen...\n\n");
            if (!isEmpty(s1))
            {
                s2 = top(s1);
                displayList(s2->below);
                printf("\n");
                pop(s1);
            }
            else
                printf("\nThe Stack Is Empty, Nothing To Print.\n\n");
            break;

        /* ── 7. Print result to file ────────────────────────────────────── */
        case 7:
            printf("\nPrinting The Results On The File...\n\n");
            if (!isEmpty(s1))
            {
                s2 = top(s1);
                printOnFile(s2->below);
                printf("\nOutput Written To output.txt Successfully.\n\n");
            }
            else
                printf("\nNothing To Print — Run An Operation First.\n\n");
            break;

        /* ── 8. Exit ────────────────────────────────────────────────────── */
        case 8:
            printf("\nExiting The Program.\n\n");
            break;

        default:
            printf("\nInvalid Option. Please Choose Between 1 and 8.\n\n");
        }
    }

    return 0;
}

/* ─────────────────────────────────────────────────────────────────────────── */

list newList(void)
{
    list l = (list)malloc(sizeof(struct node2));
    l->next = NULL;
    return l;
}

void push(stack s, ptr2 p)
{
    if (s == NULL) return;
    ptr1 newNode = (ptr1)malloc(sizeof(struct node1));   /* FIX: renamed from `new` */
    newNode->below = p;
    newNode->next  = s->next;
    s->next        = newNode;
}

void pop(stack s)
{
    if (!isEmpty(s))
    {
        ptr1 d  = s->next;
        s->next = d->next;
        free(d);
    }
}

int isEmpty(stack s)
{
    return (s == NULL) || (s->next == NULL);
}

ptr1 top(stack s)
{
    return isEmpty(s) ? NULL : s->next;
}

/* ─────────────────────────────────────────────────────────────────────────── */

void insertSorted(list l, ptr2 temp)
{
    if (l == NULL || temp == NULL)
    {
        printf("Error: list or node is NULL.\n");
        return;
    }

    /* FIX: original had = instead of ==, dead inner NULL check,
       and a NULL dereference in the while condition.
       Correct version: walk until t->next is NULL or has smaller exponent */
    ptr2 t = l;
    while (t->next != NULL && t->next->exponent > temp->exponent)
        t = t->next;

    temp->next = t->next;
    t->next    = temp;
}

/* ─────────────────────────────────────────────────────────────────────────── */

void displayList(list l)
{
    if (l == NULL) return;
    pos p = l->next;
    int first = 1;
    while (p != NULL)
    {
        if (!first && p->coefficient >= 0) printf("+");
        printf("%dX^%d", p->coefficient, p->exponent);
        p     = p->next;
        first = 0;
    }
    if (first) printf("0");   /* empty polynomial */
}

/* ─────────────────────────────────────────────────────────────────────────── */

void printMenu(void)
{
    printf("\n\t\t Welcome To My Program!\t\t\n\n");
    printf(" Please Select Your Choice:\n\n");
    printf("1. Load the polynomials file\n");
    printf("2. Print the polynomials\n");
    printf("3. Add the polynomials\n");
    printf("4. Subtract the polynomials\n");
    printf("5. Multiply the polynomials\n");
    printf("6. Print the results to the screen\n");
    printf("7. Print the results to file\n");
    printf("8. Exit\n");
}

/* ─────────────────────────────────────────────────────────────────────────── */

stack readFromFile(list t[])   /* FIX: parameter matches array, not single element */
{
    FILE *file = fopen("input.txt", "r");
    if (file == NULL)          /* FIX: no NULL check in original — crash on missing file */
    {
        printf("Error: Could not open input.txt\n");
        stack s = (stack)malloc(sizeof(struct node1));
        s->next  = NULL;
        s->below = NULL;
        return s;
    }

    stack s = (stack)malloc(sizeof(struct node1));
    s->next  = NULL;
    s->below = NULL;

    int  i = 0;
    char line[MAXLINE];         /* FIX: was char line[10] — buffer overflow on any real input */

    while (i < MAXPOLY && fgets(line, sizeof(line), file))
    {
        t[i] = newList();
        char *token = strtok(line, "xX^+- \t\n");

        while (token != NULL)
        {
            int coeff = atoi(token);
            token = strtok(NULL, "xX^+- \t\n");
            if (token == NULL) break;
            int expo = atoi(token);

            ptr2 temp          = (ptr2)malloc(sizeof(struct node2));
            temp->next        = NULL;
            temp->coefficient = coeff;
            temp->exponent    = expo;

            insertSorted(t[i], temp);
            token = strtok(NULL, "xX^+- \t\n");
        }

        push(s, t[i]);
        i++;
    }

    fclose(file);
    return s;
}

/* ─────────────────────────────────────────────────────────────────────────── */

void printOnFile(list l)
{
    FILE *file = fopen("output.txt", "w");
    if (file == NULL)
    {
        printf("Error: Could not open output.txt for writing.\n");
        return;
    }

    list temp  = l->next;
    int  first = 1;
    while (temp != NULL)
    {
        if (!first && temp->coefficient >= 0) fprintf(file, "+");
        fprintf(file, "%dX^%d", temp->coefficient, temp->exponent);
        temp  = temp->next;
        first = 0;
    }
    fprintf(file, "\n");
    fclose(file);
}
