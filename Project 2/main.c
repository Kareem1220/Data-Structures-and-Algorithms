#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── Stack ──────────────────────────────────────────────────────────────── */

struct node1;
typedef struct node1 *ptr1;

struct node1 {
    int id;
    ptr1 next;
};

typedef ptr1 stack;

/* ─── Queue ──────────────────────────────────────────────────────────────── */

struct node2;
typedef struct node2 *ptr2;

struct node2 {
    int id;
    ptr2 next;
};

typedef ptr2 queue;

/* ─── Stack Operations ───────────────────────────────────────────────────── */

void  push(stack s, int k);
void  pop(stack s);
stack topS(stack s);
int   isEmptyS(stack s);

/* ─── Queue Operations ───────────────────────────────────────────────────── */

int   isEmptyQ(queue q);
void  enqueue(queue q, int k);
void  dequeue(queue q);
queue topQ(queue q);

/* ─── Tournament Logic ───────────────────────────────────────────────────── */

queue champion(queue q);
queue runnerUp(queue tempQueue);
int   duplicatedInput(stack s, int count);
void  freeQueue(queue q);
void  freeStack(stack s);

/* ─────────────────────────────────────────────────────────────────────────── */

int main()
{
    int n, temp, count;

    while (1)
    {
        count = 0;

        /* Allocate fresh Stack and Queues each iteration */
        stack s = (stack)malloc(sizeof(struct node1));
        s->next = NULL;

        queue q = (queue)malloc(sizeof(struct node2));
        q->next = NULL;

        queue tempQueue = (queue)malloc(sizeof(struct node2));
        tempQueue->next = NULL;

        printf("\nPlease Enter The Number Of Teams Followed By Each Team's ID.\n\n");
        scanf("%d", &n);

        if (n == -9999)
        {
            printf("\nThank You For Using My Program\n\n");
            printf("Exiting...\n\n");
            freeQueue(q);
            freeQueue(tempQueue);
            freeStack(s);
            return 0;
        }

        if (n < 0)
        {
            printf("You Cannot Have A Negative Number Of Teams.\n\n");
            freeQueue(q);
            freeQueue(tempQueue);
            freeStack(s);
            return 1;
        }

        if (n == 0 || n == 1)
        {
            printf("You Need To Have At Least Two Teams Or More!\n\n");
            freeQueue(q);
            freeQueue(tempQueue);
            freeStack(s);
            return 1;
        }

        printf("Please Enter %d ID(s).\n\n", n);

        for (int i = 0; i < n; i++)
        {
            scanf("%d", &temp);

            if (temp >= 10000 || temp <= -10000)
            {
                printf("Invalid ID Value.\n\n");
                freeQueue(q);
                freeQueue(tempQueue);
                freeStack(s);
                return 1;
            }

            push(s, temp);
            enqueue(q, temp);
            enqueue(tempQueue, temp);
            count++;
        }

        if (duplicatedInput(s, count))
        {
            printf("\nID's Must Be Unique — Do Not Duplicate Them.\n\n");
            freeQueue(q);
            freeQueue(tempQueue);
            freeStack(s);
            return 1;
        }

        if (count != n)
        {
            printf("The Number Of Teams Does Not Match The Number Of ID's.\n\n");
            freeQueue(q);
            freeQueue(tempQueue);
            freeStack(s);
            return 1;
        }

        ptr2 c = champion(q);
        printf("\nThe Champion Is %d!!!\n", c->id);

        ptr2 r = runnerUp(tempQueue);
        printf("\nThe Runner-Up Is %d!!!\n", r->id);

        freeQueue(q);
        freeQueue(tempQueue);
        freeStack(s);
    }

    return 0;
}

/* ─────────────────────────────────────────────────────────────────────────── */

void push(stack s, int k)
{
    if (s == NULL)
    {
        printf("\nThe Stack Does Not Exist. Please Initialize One First.\n\n");
        return;
    }

    stack newNode = (stack)malloc(sizeof(struct node1));
    if (newNode == NULL)
    {
        printf("Out of memory.\n\n");
        return;
    }

    newNode->id   = k;
    newNode->next = s->next;
    s->next       = newNode;
}

void pop(stack s)
{
    if (!isEmptyS(s))
    {
        ptr1 d = s->next;
        s->next = d->next;
        free(d);
    }
    else
        printf("Stack Is Empty, Nothing To Pop.\n");
}

stack topS(stack s)
{
    return isEmptyS(s) ? NULL : s->next;
}

int isEmptyS(stack s)
{
    return (s == NULL) || (s->next == NULL);
}

/* ─────────────────────────────────────────────────────────────────────────── */

int isEmptyQ(queue q)
{
    return (q == NULL) || (q->next == NULL);
}

void enqueue(queue q, int k)
{
    if (q == NULL)
    {
        printf("Queue Does Not Exist.\n\n");
        return;
    }

    ptr2 newNode = (ptr2)malloc(sizeof(struct node2));
    if (newNode == NULL)
    {
        printf("Out of memory.\n\n");
        return;
    }

    newNode->id   = k;
    newNode->next = NULL;

    if (isEmptyQ(q))
    {
        q->next = newNode;
        return;
    }

    ptr2 temp = q->next;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
}

void dequeue(queue q)
{
    if (q == NULL)
    {
        printf("Queue Does Not Exist.\n\n");
        return;
    }

    if (isEmptyQ(q))
    {
        printf("The Queue Is Empty, Nothing To Dequeue.\n\n");
        return;
    }

    ptr2 t = q->next;
    q->next = t->next;
    free(t);
}

queue topQ(queue q)
{
    return isEmptyQ(q) ? NULL : q->next;
}

/* ─────────────────────────────────────────────────────────────────────────── */

queue champion(queue q)
{
    queue q1 = (queue)malloc(sizeof(struct node2));
    q1->next = NULL;

    int round = 1;
    ptr2 t1, t2;

    while (!isEmptyQ(q) && q->next->next != NULL)
    {
        t1 = topQ(q);
        t2 = t1->next;

        if (topQ(q1) != NULL &&
            (topQ(q1)->id == t1->id || topQ(q1)->id == t2->id))
        {
            round++;
            while (!isEmptyQ(q1))
                dequeue(q1);
        }

        if (round % 2 == 0)
        {
            /* Even round — larger ID wins */
            enqueue(q,  t1->id > t2->id ? t1->id : t2->id);
            enqueue(q1, t1->id > t2->id ? t1->id : t2->id);
        }
        else
        {
            /* Odd round — smaller ID wins */
            enqueue(q,  t1->id < t2->id ? t1->id : t2->id);
            enqueue(q1, t1->id < t2->id ? t1->id : t2->id);
        }

        dequeue(q);
        dequeue(q);
    }

    /* Free q1 nodes before freeing header */
    while (!isEmptyQ(q1))
        dequeue(q1);
    free(q1);

    return topQ(q);
}

/* ─────────────────────────────────────────────────────────────────────────── */

queue runnerUp(queue tempQueue)
{
    /* Re-run champion on a copy to find the champion's ID */
    queue findChamp = (queue)malloc(sizeof(struct node2));
    findChamp->next = NULL;

    ptr2 tmp = tempQueue->next;
    while (tmp != NULL)
    {
        enqueue(findChamp, tmp->id);
        tmp = tmp->next;
    }

    int champID = champion(findChamp)->id;
    while (!isEmptyQ(findChamp)) dequeue(findChamp);
    free(findChamp);

    /* Replay the tournament, recording everyone who lost to the champion */
    queue q1 = (queue)malloc(sizeof(struct node2));
    q1->next = NULL;

    stack s1 = (stack)malloc(sizeof(struct node1));
    s1->next = NULL;

    int round = 1;
    ptr2 t1, t2;

    while (!isEmptyQ(tempQueue) && tempQueue->next->next != NULL)
    {
        t1 = topQ(tempQueue);
        t2 = t1->next;

        /* Record whoever faced (and lost to) the champion */
        if (t1->id == champID) push(s1, t2->id);
        if (t2->id == champID) push(s1, t1->id);

        if (topQ(q1) != NULL &&
            (topQ(q1)->id == t1->id || topQ(q1)->id == t2->id))
        {
            round++;
            while (!isEmptyQ(q1))
                dequeue(q1);
        }

        if (round % 2 == 0)
        {
            enqueue(tempQueue, t1->id > t2->id ? t1->id : t2->id);
            enqueue(q1,        t1->id > t2->id ? t1->id : t2->id);
        }
        else
        {
            enqueue(tempQueue, t1->id < t2->id ? t1->id : t2->id);
            enqueue(q1,        t1->id < t2->id ? t1->id : t2->id);
        }

        dequeue(tempQueue);
        dequeue(tempQueue);
    }

    while (!isEmptyQ(q1)) dequeue(q1);
    free(q1);

    /* Build a queue from the stack (reversing order as required) */
    queue q2 = (queue)malloc(sizeof(struct node2));
    q2->next = NULL;

    while (!isEmptyS(s1))
    {
        enqueue(q2, topS(s1)->id);
        pop(s1);
    }
    free(s1);

    /* The runner-up is the champion among all champion's losers */
    return champion(q2);
}

/* ─────────────────────────────────────────────────────────────────────────── */

int duplicatedInput(stack s, int count)
{
    int temp[count];
    int i = 0;

    while (count != 0)
    {
        temp[i] = topS(s)->id;
        i++;
        count--;
        pop(s);
    }
    /* BUG FIX: removed stray i++ that caused off-by-one,
       comparing uninitialized memory and producing false duplicates */

    for (int j = 0; j < i; j++)
        for (int k = j + 1; k < i; k++)
            if (temp[j] == temp[k]) return 1;

    return 0;
}

/* ─── Cleanup Helpers ────────────────────────────────────────────────────── */

void freeQueue(queue q)
{
    if (q == NULL) return;
    while (!isEmptyQ(q))
        dequeue(q);
    free(q);
}

void freeStack(stack s)
{
    if (s == NULL) return;
    while (!isEmptyS(s))
        pop(s);
    free(s);
}
