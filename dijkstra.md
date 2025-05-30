# Dijkstra's Algorithm
In our previous exploration, we discussed Breadth First Search - an efficient algorithm to determine the shortest path between vertices on _unweighted_ graphs. _Weighted_ graphs have edges that differ in _cost_. For example, driving from point A to point B may seem faster because there is only one connection between nodes, but suppose the bike route from point A to point C to point B is actually the quickest route:
```
(A) =====> (B)
    10mins 

(A) =====> (C) ======> (A)
    2mins      5mins
```

Dijkstra's algorithm provides the shortest path between two points on a weighted and directed graph:
```c
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
```
Full implementation [here](./examples/dijkstra.c)

🚧 Todo: better explanation of the algorithm, step by step 🚧