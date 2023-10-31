#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEXS 100
int visited[MAX_VERTEXS]; // 노드의 방문여부를 저장 할 배열

/* 초기 그래프의 인접리스트 표현
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

// 에러출력함수
void error(char* message) {
    fprintf(stderr, "\n%s\n", message);
}

// DFS에서 사용할 스택 정의
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

// BFS에서 사용할 큐 정의
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
        qptr->rear = (qptr->rear + 1) % (MAX_VERTEXS); // rear index를 한 칸 뒤로
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
    int n; // 정점의 개수
    int adj_mat[MAX_VERTEXS][MAX_VERTEXS]; // 인접행렬
} graphType_mat;

typedef struct GraphNode {
    int vertex; // 정점의 번호
    struct GraphNode* link; // 정점이 가리키는 다음 노드의 주소
} graphNode;

typedef struct GraphType_LIST {
    int n; // 정점의 개수
    graphNode* adj_list[MAX_VERTEXS]; // 인접리스트
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
            printf("%d ", node);  // 노드 방문 출력

            if (node == key) {  // 목표 노드 찾으면 종료
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

// BFS 함수
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
            printf("%d ", node);  // 노드 방문 출력

            if (node == key) {  // 목표 노드 찾으면 종료
                printf("\n탐색성공: %d\n", node);
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

    // 정점 추가
    for (int i = 0; i < 11; i++) {
        insert_vertex(g, i);
    }

    // 각 정점에 대한 인접 정점 추가
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
        printf("1. 깊이 우선 탐색\n");
        printf("2. 너비 우선 탐색\n");
        printf("3. 종료\n");
        printf("==========\n");
        printf("메뉴 입력: ");
        scanf("%d", &menu);

        int startNode, key;
        switch (menu) {
        case 1:
            printf("시작할 번호와 탐색할 값 입력: ");
            scanf("%d %d", &startNode, &key);
            dfs_list(g, startNode, key);
            break;
        case 2:
            printf("시작할 번호와 탐색할 값 입력: ");
            scanf("%d %d", &startNode, &key);
            bfs_list(g, startNode, key);
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("잘못된 입력입니다 다시 시도해주세요: ");
            break;
        }
    }

    return 0;
}
