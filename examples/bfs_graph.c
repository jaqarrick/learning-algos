#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_NODES 6

typedef struct Node {
  int dest;
  struct Node *next;
} Node;

typedef struct Graph {
  Node *heads[NUM_NODES];
} Graph;

typedef struct Edge {
  int src, dest;
} Edge;

typedef struct Path {
  Edge* edge;
  struct Path* next;
} Path;

typedef struct QueueNode {
  int data;
  struct QueueNode* next;
} QueueNode;

typedef struct SearchQueue {
  QueueNode* head;
  QueueNode* tail;
  int size;
} SearchQueue;

void exitWithAllocFailed(){
  printf("Memory allocation failed\n");
  exit(1);
}

SearchQueue* createSearchQueue(){
  SearchQueue* q = (SearchQueue*)malloc(sizeof(SearchQueue));
  if(q == NULL){
    exitWithAllocFailed();
  }
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
  return q;
}

void freeSearchQueue(SearchQueue* q){
  if(q->size != 0){
    // free search nodes
    QueueNode* currentNode = q->head;
    while(currentNode != NULL){
      QueueNode* next = currentNode->next;
      free(currentNode);
      currentNode = next;
    }
  }
  free(q);
}

void enqueue(SearchQueue* q, int data){
  QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
  if(newNode == NULL){
   exitWithAllocFailed(); 
  }
  newNode->data = data; 
  newNode->next = NULL;
  
  if(q->tail == NULL){
    q->head = newNode;
    q->tail = newNode;
  } else {
    q->tail->next = newNode;
    q->tail = newNode;
  }
  q->size += 1;
}

QueueNode* dequeue(SearchQueue* q){
  QueueNode* head = q->head;
  if(head == NULL) return NULL;
  q->head = head->next;
  if(q->head == NULL) q->tail = NULL; // Reset tail when queue becomes empty
  q->size -= 1;
  return head;
}

Graph *createGraph(Edge edges[], int numEdges) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if(graph == NULL){
    exitWithAllocFailed();
  }

  for (int i = 0; i < NUM_NODES ; i++) {
    graph->heads[i] = NULL;
  }

  for (int i = 0; i < numEdges; i++){
    Node *node = (Node*)malloc(sizeof(Node));
    if(node == NULL){
      exitWithAllocFailed();
    }
    Edge edge = edges[i];
    int src = edge.src;
    int dest = edge.dest;
    node->dest = dest;
    node->next = graph->heads[src];
    graph->heads[src] = node;
  }
  return graph;
}

void freeGraph(Graph* graph){
  for(int i = 0; i < NUM_NODES; i++){
    Node* current = graph->heads[i];
    while(current != NULL){
      Node* next = current->next;
      free(current);
      current = next;
    }
  }
  free(graph);
}

void printGraph(Graph* graph){
  for(int i = 0; i < NUM_NODES; i++){
    Node* currentNode = graph->heads[i];
    while (currentNode != NULL){
      printf("(%d -> %d)\t", i, currentNode->dest);
      currentNode = currentNode->next;
    }
    printf("\n");
  }
}

Path* getPath(int *parents, int start, int end){
  int currentNode = end;
  Path* head = NULL;
  while(currentNode != start){
    Path* p = (Path*)malloc(sizeof(Path));
    if(p == NULL){
      exitWithAllocFailed();
    }
    p->next = head;
    head = p;
    int parent = parents[currentNode];
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    if(edge == NULL){
      exitWithAllocFailed();
    }
    edge->src = parent;
    edge->dest = currentNode;
    currentNode = parent;
    p->edge = edge;
  }  
  return head;
}

void freePath(Path* p){
  while(p != NULL){
    free(p->edge);
    Path* next = p->next;
    free(p);
    p = next;
  }
}

void printPath(Path* p){
  if(p == NULL){
    printf("No path to print\n");
    return;
  }
  while(p != NULL){
    int src = p->edge->src;
    int dest = p->edge->dest;
    printf("%d ---> %d", src, dest);
    p = p->next;
    if(p != NULL) printf(", ");
  }
  printf("\n");
}

Path* bfs(Graph* graph, int start, int end){
  if(start < 0 || start >= NUM_NODES || end < 0 || end >= NUM_NODES){
    printf("Invalid start or end node\n");
    return NULL;
  }
  
  if(start == end){
    printf("Start and end are the same node\n");
    return NULL;
  }

  bool visited[NUM_NODES] = {false};  
  int parents[NUM_NODES];
  for(int i = 0; i < NUM_NODES; i++){
    parents[i] = -1;
  }
  
  SearchQueue* q = createSearchQueue();
  visited[start] = true; 
  enqueue(q, start);
  bool found = false;
  
  while (q->size > 0 && !found) {
    QueueNode* n = dequeue(q);
    int currentNode = n->data;
    free(n);    
    Node* neighbor = graph->heads[currentNode];  
    while (neighbor != NULL) {
      if (!visited[neighbor->dest]) {
        visited[neighbor->dest] = true;
        parents[neighbor->dest] = currentNode;
        enqueue(q, neighbor->dest);
        if (neighbor->dest == end) {
          found = true;
          break;
        }
      }
      neighbor = neighbor->next;
    }
  }
  
  freeSearchQueue(q);
  
  if(found){
    printf("Path found from %d to %d!\n", start, end);
    Path* path = getPath(parents, start, end);
    return path;
  } else {
    printf("No path exists from %d to %d.\n", start, end);
    return NULL;
  }
}

int main(int argc, char* argv[]) {
  if(argc < 3){
    printf("Provide start and end arguments\n");
    exit(1);
  }
  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  Edge edges[] = {{0, 1}, {1, 2}, {2, 3}, {3, 5}, {4, 3}, {4, 5}, {0, 5}};
  int numEdges = sizeof(edges) / sizeof(edges[0]);
  
  Graph *graph = createGraph(edges, numEdges);
  
  printf("Graph structure:\n");
  printGraph(graph);
  printf("\nRunning BFS from node 0 to node 2:\n");
  
  Path* path = bfs(graph, start, end);
  if(path != NULL) {
    printf("Shortest path: ");
    printPath(path);
    freePath(path);
  }
  
  freeGraph(graph);
  return 0;
}