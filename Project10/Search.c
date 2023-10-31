#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEXS 100
int visited[MAX_VERTEXS]; // ����� �湮���θ� ���� �� �迭

/* �ʱ� �׷����� ��������Ʈ ǥ��
0 -> 2 -> 5 -> 6 -> 9 -> NULL
1 -> 4 -> 5 -> 7 -> 10 -> NULL
2 -> 0 -> 3 -> 4 -> NULL
3 -> 2 -> 4 -> 5 -> NULL
4 -> 1 -> 2 -> 3 -> 5 -> 6 -> 7 -> NULL
5 -> 0 -> 1 -> 3 -> 4 -> NULL
6 -> 0 -> 4 -> 7 -> 8 -> NULL
7 -> 1 -> 4 -> 10 -> NULL
8 -> 6 -> 9 -> 10 -> NULL
9 -> 0 -> 8 -> NULL
10 -> 1 -> 7 -> 8 -> NULL
*/

// ��������Լ�
void error(char* message) {
    fprintf(stderr, "\n%s\n", message);
}

// DFS���� ����� ���� ����
typedef struct {
    int data[MAX_VERTEXS];
    int top;
}StackType;

void init_stack(StackType* s) {
    s->top = -1;
}

int is_emptys(StackType* s) {
    return s->top == -1;
}

int is_fulls(StackType* s) {
    return s->top == MAX_VERTEXS - 1;
}

void push(StackType* s, int item) {
    if (is_fulls(s)) {
        error("Stack Full");
        return;
    }
    else {
        s->data[++(s->top)] = item;
    }
}

int pop(StackType* s) {
    if (is_emptys(s)) {
        error("Stack Empty");
        return -1;
    }
    else return(s->data[(s->top)--]);
}

// BFS���� ����� ť ����
typedef struct {
    int data[MAX_VERTEXS];
    int front, rear;
} QueueType;

void init_queue(QueueType* qptr) {
    qptr->front = qptr->rear = 0;
}

int is_emptyq(QueueType* qptr) {
    return (qptr->front == qptr->rear);
}

int is_fullq(QueueType* qptr) {
    return (qptr->front == qptr->rear + 1 % MAX_VERTEXS);
}

void enqueue(QueueType* qptr, int value) {
    if (is_fullq(qptr)) {
        error("Queue is Full");
        return;
    }
    else {
        qptr->rear = (qptr->rear + 1) % (MAX_VERTEXS); // rear index�� �� ĭ �ڷ�
        qptr->data[qptr->rear] = value;
    }
}

int dequeue(QueueType* qptr) {
    if (is_emptyq(qptr)) {
        error("Queue is Empty");
        return -1;
    }
    else {
        qptr->front++;
        return(qptr->data[qptr->front]);
    }
}

typedef struct GraphType_MAT {
    int n; // ������ ����
    int adj_mat[MAX_VERTEXS][MAX_VERTEXS]; // �������
} graphType_mat;

typedef struct GraphNode {
    int vertex; // ������ ��ȣ
    struct GraphNode* link; // ������ ����Ű�� ���� ����� �ּ�
} graphNode;

typedef struct GraphType_LIST {
    int n; // ������ ����
    graphNode* adj_list[MAX_VERTEXS]; // ��������Ʈ
} graphType_list;

void initGList(graphType_list* g) {
    int v;
    g->n = 0;
    for (v = 0; v < MAX_VERTEXS; v++) {
        g->adj_list[v] = NULL;
    }
}

void insert_vertex(graphType_list* g, int v) {
    if (((g->n) + 1) > MAX_VERTEXS) {
        error("Over vertexs's number");
        return;
    }
    g->n++;
}

void insert_edge(graphType_list* g, int u, int v) {
    graphNode* node;
    if (u >= g->n || v >= g->n) {
        error("Defalut vertex's number");
        return;
    }
    node = (graphNode*)malloc(sizeof(graphNode));
    node->vertex = v;
    node->link = g->adj_list[u];
    g->adj_list[u] = node;
}

void dfs_list(graphType_list* g, int v, int key) {
    graphNode* nptr;
    int node;
    StackType s;
    init_stack(&s);
    push(&s, v);
    for (int i = 0; i < g->n; i++) visited[i] = 0;

    while (!is_emptys(&s)) {
        node = pop(&s);
        if (!visited[node]) {
            visited[node] = 1;
            printf("%d ", node);  // ��� �湮 ���

            if (node == key) {  // ��ǥ ��� ã���� ����
                printf("\nFound key node! Visited nodes: %d\n", node);
                break;
            }

            for (nptr = g->adj_list[node]; nptr != NULL; nptr = nptr->link) {
                if (!visited[nptr->vertex]) {
                    push(&s, nptr->vertex);
                }
            }
        }
    }
}

// BFS �Լ�
void bfs_list(graphType_list* g, int v, int key) {
    graphNode* nptr;
    int node;
    QueueType q;
    init_queue(&q);
    enqueue(&q, v);
    for (int i = 0; i < g->n; i++) visited[i] = 0;

    while (!is_emptyq(&q)) {
        node = dequeue(&q);
        if (!visited[node]) {
            visited[node] = 1;
            printf("%d ", node);  // ��� �湮 ���

            if (node == key) {  // ��ǥ ��� ã���� ����
                printf("\nŽ������: %d\n", node);
                break;
            }

            for (nptr = g->adj_list[node]; nptr != NULL; nptr = nptr->link) {
                if (!visited[nptr->vertex]) {
                    enqueue(&q, nptr->vertex);
                }
            }
        }
    }
}

graphType_list* makeGraph() {
    graphType_list* g = (graphType_list*)malloc(sizeof(graphType_list));
    initGList(g);

    // ���� �߰�
    for (int i = 0; i < 11; i++) {
        insert_vertex(g, i);
    }

    // �� ������ ���� ���� ���� �߰�
    int edges[11][6] = {
        {2, 5, 6, 9, -1},  // 0
        {4, 5, 7, 10, -1}, // 1
        {0, 3, 4, -1},     // 2
        {2, 4, 5, -1},     // 3
        {1, 2, 3, 5, 6, 7, }, // 4
        {0, 1, 3, 4, -1}, // 5
        {0, 4, 7, 8, -1}, // 6
        {1, 4, 10, -1},   // 7
        {6, 9, 10, -1},    // 8
        {0, 8, -1},        // 9
        {1, 7, 8, -1}      // 10
    };

    for (int i = 0; i < 11; i++) {
        for (int j = 0; edges[i][j] != -1; j++) {
            insert_edge(g, i, edges[i][j]);
        }
    }

    return g;
}

int main() {
    graphType_list* g = makeGraph();
    int menu;
    while (1) {
        printf("==========\n");
        printf("1. ���� �켱 Ž��\n");
        printf("2. �ʺ� �켱 Ž��\n");
        printf("3. ����\n");
        printf("==========\n");
        printf("�޴� �Է�: ");
        scanf("%d", &menu);

        int startNode, key;
        switch (menu) {
        case 1:
            printf("������ ��ȣ�� Ž���� �� �Է�: ");
            scanf("%d %d", &startNode, &key);
            dfs_list(g, startNode, key);
            break;
        case 2:
            printf("������ ��ȣ�� Ž���� �� �Է�: ");
            scanf("%d %d", &startNode, &key);
            bfs_list(g, startNode, key);
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("�߸��� �Է��Դϴ� �ٽ� �õ����ּ���: ");
            break;
        }
    }

    return 0;
}
