#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define NUM_NODES 6

typedef struct Node {
  int dest;
  int weight;
  struct Node *next;
} Node;

typedef struct Graph {
  Node *heads[NUM_NODES];
} Graph;

typedef struct Edge {
  int src, dest, weight;
} Edge;

typedef struct Path {
  Edge* edge;
  struct Path* next;
} Path;

void exitWithAllocFailed(){
  printf("Memory allocation failed\n");
  exit(1);
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
    int weight = edge.weight;
    node->dest = dest;
    node->weight = weight;
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
      printf("(%d -> %d) w: %d\t", i, currentNode->dest, currentNode->weight);
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

int getCheapestNode(int distances[], bool visited[]){
    int minDistance = INT_MAX;
    int minNode = -1;
    for(int i = 0; i < NUM_NODES; i++){
        if(!visited[i]){
            if(distances[i] < minDistance){
                minDistance = distances[i];
                minNode = i;
                if(minDistance == 0) break;
            }
        }
    }
    return minNode;
}

Path* dijkstraSearch(Graph* graph, int start, int end){
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
    int distances[NUM_NODES];
    // init non-zeroed arrays
    for(int i = 0; i < NUM_NODES; i++){
        parents[i] = -1;
        distances[i] = INT_MAX;
    }

    // now for the algorithm
    distances[start] = 0; // this prioritizes the start 

    for (int count = 0; count < NUM_NODES; count++){
        int nodeIndex = getCheapestNode(distances, visited);
        Node* neighbor = graph->heads[nodeIndex];
        while(neighbor != NULL){
            int neighborIndex = neighbor->dest;
            int weight = neighbor->weight;
            Node* next = neighbor->next;

            if(weight < distances[neighborIndex]){
                distances[neighborIndex] = weight;
                parents[neighborIndex] = nodeIndex;
            }
            neighbor = next;
        }
        visited[nodeIndex] = true;
    }

    if(visited[end] == true){
        printf("Path found\n");
        return getPath(parents, start, end);
    } else {
        printf("Invalid start and end. No Path found\n");
        exit(0);
    }
}

int main(int argc, char* argv[]) {
  if(argc < 3){
    printf("Provide start and end arguments\n");
    exit(1);
  }
  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  Edge edges[] = {{0, 1, 5}, {0, 2, 10}, {1, 4, 11}, {2, 4, 2}, {1, 2, 1}, {5, 1, 2}, {3, 5, 2}};
  int numEdges = sizeof(edges) / sizeof(edges[0]); 
  
  Graph *graph = createGraph(edges, numEdges);
  
  printf("Graph structure:\n");
  printGraph(graph);
  Path* path = dijkstraSearch(graph, start, end);
  printPath(path);
  freePath(path);
  freeGraph(graph);
  return 0;
}