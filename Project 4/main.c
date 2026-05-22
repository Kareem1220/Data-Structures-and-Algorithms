#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_VERTICES 100
#define HEAPSIZE     100

/* ─── Min-Heap ───────────────────────────────────────────────────────────── */

typedef struct {
    int vertex;
    int cost;
} MinHeapNode;

typedef struct {
    MinHeapNode array[HEAPSIZE];
    int size;
} MinHeap;

void initializeMinHeap(MinHeap *heap)
{
    heap->size = 0;
}

void insertMinHeap(MinHeap *heap, int vertex, int cost)
{
    if (heap->size >= HEAPSIZE)
    {
        printf("Heap Is Full.\n");
        return;
    }

    int i = heap->size++;
    while (i > 0 && heap->array[(i - 1) / 2].cost > cost)
    {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i].vertex = vertex;
    heap->array[i].cost   = cost;
}

MinHeapNode deleteMinHeap(MinHeap *heap)
{
    /* FIX: added empty-heap guard — original accessed array[0] on empty heap */
    if (heap->size == 0)
    {
        MinHeapNode empty = {-1, INT_MAX};
        return empty;
    }

    MinHeapNode root     = heap->array[0];
    MinHeapNode lastNode = heap->array[--heap->size];

    int i = 0;
    while ((i * 2) + 1 < heap->size)
    {
        int child = (i * 2) + 1;
        if (child + 1 < heap->size &&
            heap->array[child + 1].cost < heap->array[child].cost)
            child++;

        if (lastNode.cost <= heap->array[child].cost)
            break;

        heap->array[i] = heap->array[child];
        i = child;
    }
    heap->array[i] = lastNode;
    return root;
}

int isMinHeapEmpty(MinHeap *heap)
{
    return heap->size == 0;
}

/* ─── Graph ──────────────────────────────────────────────────────────────── */

int  adjacencyMatrix[MAX_VERTICES][MAX_VERTICES];
int  vertexCount = 0;
char vertices[MAX_VERTICES];

typedef struct {
    int visited;
    int cost;
    int prev;
} VertexInfo;

/* Global results (set by dijkstra / bfs, read by WriteToFile) */
int dijkstraPath[MAX_VERTICES];
int dijkstraPathLength = 0;
int dijkstraCost       = 0;

int bfsPath[MAX_VERTICES];
int bfsPathLength = 0;
int bfsCost       = 0;

/* ─── Queue (for BFS) ────────────────────────────────────────────────────── */

typedef struct {
    int Vertis[MAX_VERTICES];
    int front;
    int rear;
} Queue;

Queue *createQueue(void)
{
    Queue *queue  = (Queue *)malloc(sizeof(Queue));
    queue->front  = -1;
    queue->rear   = -1;
    return queue;
}

int isEmpty(Queue *queue)
{
    return queue->rear == -1;
}

void enqueue(Queue *queue, int value)
{
    if (queue->rear == MAX_VERTICES - 1)
    {
        printf("Queue Is Full.\n");
        return;
    }
    if (queue->front == -1) queue->front = 0;
    queue->Vertis[++queue->rear] = value;
}

int dequeue(Queue *queue)
{
    if (isEmpty(queue)) { printf("Queue Is Empty.\n"); return -1; }
    int vert = queue->Vertis[queue->front];
    if (queue->front >= queue->rear) { queue->front = -1; queue->rear = -1; }
    else queue->front++;
    return vert;
}

void deleteQueue(Queue *queue) { free(queue); }

/* ─── Prototypes ─────────────────────────────────────────────────────────── */

int  findVertexIndex(char v);
int  addVertex(char v);
void initializeGraph(void);
void loadGraphFromFile(void);
void dijkstra(int source, int destination);
void bfs(int source, int destination);
void WriteToFile(int source, int destination);
void printMenu(void);

/* ─── Graph helpers ──────────────────────────────────────────────────────── */

void initializeGraph(void)
{
    for (int i = 0; i < MAX_VERTICES; i++)
        for (int j = 0; j < MAX_VERTICES; j++)
            adjacencyMatrix[i][j] = (i == j) ? 0 : INT_MAX;
    vertexCount = 0;
}

int findVertexIndex(char vertex)
{
    for (int i = 0; i < vertexCount; i++)
        if (vertices[i] == vertex) return i;
    return -1;
}

int addVertex(char vertex)
{
    int idx = findVertexIndex(vertex);
    if (idx == -1)
    {
        vertices[vertexCount++] = vertex;
        return vertexCount - 1;
    }
    return idx;
}

void loadGraphFromFile(void)
{
    FILE *file = fopen("routers.txt", "r");
    if (!file)
    {
        printf("Failed To Open routers.txt.\n");
        return;
    }

    initializeGraph();

    char src, dest;
    int  weight;

    /* FIX: added leading space before %c so scanf skips whitespace/newlines
       between tokens — original read stray newline chars as vertex names     */
    while (fscanf(file, " %c-%c-%d", &src, &dest, &weight) == 3)
    {
        int si = addVertex(src);
        int di = addVertex(dest);
        adjacencyMatrix[si][di] = weight;
        adjacencyMatrix[di][si] = weight;
    }
    fclose(file);
    printf("Graph Loaded Successfully. %d Vertices Found.\n", vertexCount);
}

/* ─── Dijkstra ───────────────────────────────────────────────────────────── */

void dijkstra(int source, int destination)
{
    VertexInfo vertexInfo[MAX_VERTICES];
    MinHeap    heap;
    initializeMinHeap(&heap);

    for (int i = 0; i < vertexCount; i++)
    {
        vertexInfo[i].visited = 0;
        vertexInfo[i].cost    = INT_MAX;
        vertexInfo[i].prev    = -1;
    }

    vertexInfo[source].cost = 0;
    insertMinHeap(&heap, source, 0);

    while (!isMinHeapEmpty(&heap))
    {
        MinHeapNode node = deleteMinHeap(&heap);
        int u = node.vertex;
        if (vertexInfo[u].visited) continue;
        vertexInfo[u].visited = 1;

        for (int v = 0; v < vertexCount; v++)
        {
            if (!vertexInfo[v].visited && adjacencyMatrix[u][v] != INT_MAX)
            {
                int alt = vertexInfo[u].cost + adjacencyMatrix[u][v];
                if (alt < vertexInfo[v].cost)
                {
                    vertexInfo[v].cost = alt;
                    vertexInfo[v].prev = u;
                    insertMinHeap(&heap, v, alt);
                }
            }
        }
    }

    dijkstraPathLength = 0;
    dijkstraCost       = 0;

    if (vertexInfo[destination].cost == INT_MAX)
    {
        printf("Dijkstra: No Path From %c To %c.\n",
               vertices[source], vertices[destination]);
        return;
    }

    dijkstraCost = vertexInfo[destination].cost;
    for (int v = destination; v != -1; v = vertexInfo[v].prev)
        dijkstraPath[dijkstraPathLength++] = v;

    /* FIX: print path (src→dest) first, then cost — matches BFS output order */
    printf("Dijkstra's Shortest Path:\n");
    printf("The Path : ");
    for (int i = dijkstraPathLength - 1; i >= 0; i--)
        printf("%c%s", vertices[dijkstraPath[i]], (i > 0) ? " -> " : "\n");
    printf("Total Cost: %d\n", dijkstraCost);
}

/* ─── BFS ────────────────────────────────────────────────────────────────── */

void bfs(int source, int destination)
{
    VertexInfo vertexInfo[MAX_VERTICES];
    for (int i = 0; i < vertexCount; i++)
    {
        vertexInfo[i].visited = 0;
        vertexInfo[i].cost    = INT_MAX;
        vertexInfo[i].prev    = -1;
    }

    Queue *queue = createQueue();
    enqueue(queue, source);
    vertexInfo[source].visited = 1;
    vertexInfo[source].cost    = 0;

    int found = 0;
    while (!isEmpty(queue) && !found)
    {
        int current = dequeue(queue);
        for (int i = 0; i < vertexCount; i++)
        {
            if (adjacencyMatrix[current][i] != INT_MAX && !vertexInfo[i].visited)
            {
                vertexInfo[i].visited = 1;
                vertexInfo[i].cost    = vertexInfo[current].cost + adjacencyMatrix[current][i];
                vertexInfo[i].prev    = current;
                enqueue(queue, i);
                if (i == destination) { found = 1; break; }
            }
        }
    }
    deleteQueue(queue);

    bfsPathLength = 0;
    bfsCost       = 0;

    if (!vertexInfo[destination].visited)
    {
        printf("BFS: No Path From %c To %c.\n",
               vertices[source], vertices[destination]);
        return;
    }

    for (int at = destination; at != -1; at = vertexInfo[at].prev)
        bfsPath[bfsPathLength++] = at;

    bfsCost = vertexInfo[destination].cost;

    printf("BFS Path (Weighted Cost):\n");
    printf("The Path : ");
    for (int i = bfsPathLength - 1; i >= 0; i--)
        printf("%c%s", vertices[bfsPath[i]], (i > 0) ? " -> " : "\n");
    printf("Total Cost: %d\n", bfsCost);
}

/* ─── Write to file ──────────────────────────────────────────────────────── */

void WriteToFile(int source, int destination)
{
    FILE *file = fopen("shortest_distance.txt", "w");
    if (!file) { printf("Failed To Open Output File.\n"); return; }

    fprintf(file, "Shortest Path Results\n");
    fprintf(file, "Source      : %c\n", vertices[source]);
    fprintf(file, "Destination : %c\n", vertices[destination]);

    /* FIX: both blocks now print src→dest with "->" arrows,
       matching screen output. Original used "<-" (dest→src). */
    fprintf(file, "\nDijkstra's Algorithm:\n");
    if (dijkstraPathLength > 0)
    {
        fprintf(file, "The Path : ");
        for (int i = dijkstraPathLength - 1; i >= 0; i--)
            fprintf(file, "%c%s", vertices[dijkstraPath[i]], (i > 0) ? " -> " : "\n");
        fprintf(file, "Total Cost : %d\n", dijkstraCost);
    }
    else fprintf(file, "No Path Found.\n");

    fprintf(file, "\nBFS Algorithm:\n");
    if (bfsPathLength > 0)
    {
        fprintf(file, "The Path : ");
        for (int i = bfsPathLength - 1; i >= 0; i--)
            fprintf(file, "%c%s", vertices[bfsPath[i]], (i > 0) ? " -> " : "\n");
        fprintf(file, "Total Cost : %d\n", bfsCost);
    }
    else fprintf(file, "No Path Found.\n");

    fclose(file);
    printf("Results Written To shortest_distance.txt\n");
}

/* ─── Menu ───────────────────────────────────────────────────────────────── */

void printMenu(void)
{
    printf("\nMain Menu:\n");
    printf("1. Load Routers From File\n");
    printf("2. Set Source Vertex\n");
    printf("3. Set Destination & Run Algorithms\n");
    printf("4. Save Results To File & Exit\n");
    printf("Enter Your Decision: ");
}

/* ─── Main ───────────────────────────────────────────────────────────────── */

int main(void)
{
    /* FIX: initializeGraph() at start — global adjacencyMatrix is zero-initialized
       by default (not INT_MAX), so Dijkstra/BFS would find phantom zero-weight
       edges everywhere if user skips option 1.                                 */
    initializeGraph();

    int  Decision;
    char src, dest;
    int  sourceIndex      = -1;
    int  destinationIndex = -1;
    int  graphLoaded      = 0;   /* FIX: track whether graph was loaded */

    while (1)
    {
        printMenu();
        scanf("%d", &Decision);

        switch (Decision)
        {
        case 1:
            loadGraphFromFile();
            graphLoaded = 1;
            break;

        case 2:
            /* FIX: warn user if graph not loaded yet */
            if (!graphLoaded)
            {
                printf("Please Load The Graph First (Option 1).\n");
                break;
            }
            printf("Enter Source Vertex: ");
            scanf(" %c", &src);
            sourceIndex = findVertexIndex(src);
            if (sourceIndex == -1)
                printf("Vertex '%c' Not Found In The Graph.\n", src);
            else
                printf("Source Set To: %c\n", src);
            break;

        case 3:
            if (!graphLoaded)
            {
                printf("Please Load The Graph First (Option 1).\n");
                break;
            }
            if (sourceIndex == -1)
            {
                printf("Please Set The Source Vertex First (Option 2).\n");
                break;
            }
            printf("Enter Destination Vertex: ");
            scanf(" %c", &dest);
            destinationIndex = findVertexIndex(dest);
            if (destinationIndex == -1)
            {
                printf("Vertex '%c' Not Found In The Graph.\n", dest);
                break;
            }
            printf("Destination Set To: %c\n\n", dest);
            dijkstra(sourceIndex, destinationIndex);
            printf("\n");
            bfs(sourceIndex, destinationIndex);
            break;

        case 4:
            if (sourceIndex != -1 && destinationIndex != -1)
                WriteToFile(sourceIndex, destinationIndex);
            else
                printf("Source Or Destination Not Set — Nothing To Save.\n");
            printf("Exiting. Thank You For Using My Program!\n");
            return 0;   /* FIX: was return 1 — convention for success is 0 */

        default:
            printf("Invalid Option. Please Choose 1–4.\n");
            break;
        }
    }

    return 0;
}
