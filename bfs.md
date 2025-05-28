# Graphs and BFS
Graphs are data structures that represent relationships between _things_. Obviously, using the word "things" is way too general, so I'll give some examples. I can have a graph of social relationships, like a family tree. Each node of the graph would be a person, while each "edge" (also called a _link_ or _arc_) would describe how each person is related (i.e. "son of", "cousin of"). I could have a graph of locations, where nodes represent cities and edges represent roads that connect them. Graphs and the algorithms we use to search and analyze them are utilized everywhere, like on social media sites, GPS applications, and internet routing protocols. 

Today, I'd like to explore how to build a simple graph data structure in C, as well as a well-known algorithm for determining the shortest path between nodes, called "Breadth First Search" or BFS.

### Directed graphs in C
There are several ways to represent graphs. On a basic level we need to store a list of nodes and a list of edges. For now, we will define a graph of six nodes, where the id of each node is just its index. For example, node `1` will have index of `1` and so on. An efficient way we can represent both nodes and their connections is an array of linked lists. Each index will hold all the connections for that node.

```c
#define NUM_NODES 6

typedef struct Node {
	int dest;
	struct Node *next;
} Node;

typedef struct Graph {
	Node *heads[NUM_NODES];
} Graph;
```

A hardcoded graph length is not ideal, but adds less complexity.  We can iterate on this later on. Each node struct holds the id or index of the node it is connected to. The `Graph` struct just has an array of pointers to each node adjacency list. Simple enough! Note that this graph is **directed** meaning edges or connections can only go one way. Node 1 can point to node 2, but node 2 cannot simultaneously point to node 1.  

Let's construct our graph data structure. Given an array of _edges_ we can initiate the graph:
```c
typedef struct Edge {
	int src, dest;
} Edge;

...

Edge edges[] = {{0, 1}, {1, 2}, {2, 3}, {3, 5}, {4, 3}, {4, 5}, {0, 5}};

...

Graph *graph = (Graph *)malloc(sizeof(Graph));

...

for (int i = 0; i < numEdges; i++){
	Node *node = (Node*)malloc(sizeof(Node));
	
...

	Edge edge = edges[i];
	int src = edge.src;
	int dest = edge.dest;
	node->dest = dest;
	node->next = graph->heads[src];
	graph->heads[src] = node;
}
```

_Please note I've omitted some lines for brevity._
To make the graph structure we iterate over each `Edge` and use the `src` to determine the id of the origin node. After creating the node, we add it as the head of the linked list for that index. Easy enough! Our graph data structure is now stored in memory. Let's actually do something with it.

### Breadth First Search
One of the most fundamental graph-based algorithms is BFS, which is used to determine the _shortest path_ between two nodes. Let's say I am a musician playing a show next week and am in need of a guitar amplifier. I'd like to borrow an amp from a friend. If none of my friends have an amp, I'd like each of them to ask each of their friends. Ideally, I'd like the fewest connections between friends. After all, who wants to bother a friend of a friend of a friend for music equipment. Ok, I text each of my musician friends if they have an amp. We can also think of this in terms of a _queue_, or a list of friends I can add to, but always remove the least recently added item:
```
["Bob", "Alice", "Jake"]
```
When I text Bob, the queue looks like this:
```
"Bob, do you have an amp I can borrow"
["Alice", "Jake"]
```
I don't want to bother Bob with multiple texts if none of my other friends have amps, so I'll ask him to ask his friends Greg and Kate:
```
"Bob, do you have an amp? If not, do any of your friends have one?"
["Alice", "Jake", "Greg", "Kate"]
```
If Bob _has_ an amp, then I end the search early. But he doesn't, so I keep the search going. Alice has a couple musician friends Paula and Oman. So I add them as well. 
```
["Alice", "Jake", "Greg", "Kate", "Paula", "Oman"]
```
What if I haven't had any luck with any of my friend's friends? We'll ask their friends. Oman has a couple music friends Jordan and Bob. 
```
["Oman", "Bob", "Jordan"]
```
But wait! I already asked Bob and he doesn't have an amp. To prevent this redundancy I'll keep track of the friends I've already asked and won't add them to my list. 

Ok, turns out Jordan has an amp! I stop my search here. The shortest path between my and a sweet vintage Fender Twin Reverb is `Alice -> Oman -> Jordan`. This is kind of a pain – I've got to get Jordan's contact info from Alice, she has to get it from Oman. But turns out Jordan is pretty chill and lets me borrow the amp. 

#### Implementation in C
We can express BFS more formally:
1. Given a directed graph determine the shortest path between Node A and Node B.
2. Find all neighbors of A and add to queue. 
3. For each item in the queue, check if item is the target node.
4. If yes, stop the search and return path. 
5. If not, add item's neighbors to queue.
6. Add item to visited list.
7. Continue until B is found or queue is empty.

##### Aside: queues
Before we implement this fully, we need to build our queue, as well as methods to `enqueue` and `dequeue` items:
```c
typedef struct QueueNode {
	int data;
	struct QueueNode* next;
} QueueNode;

typedef struct SearchQueue {
	QueueNode* head;
	QueueNode* tail;
	int size;
} SearchQueue;
```
Our queue will just be a linked list of node IDs (represented above as `int data`). To add items to the queue, we simply find the last item in the queue and add the new node behind it and set the new node to the `tail` or last item in the list. 
```c
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
```
If the last item does not exist, it means the list is empty so we set the new item to both the first and last item in the list. We'll also always want to increment the size of the queue. To dequeue, we'll simply pop off the `head` node and set the following item as the new `head`, as well as decrement the size.
```c
QueueNode* dequeue(SearchQueue* q){
	QueueNode* head = q->head;
	if(head == NULL) return NULL;
	q->head = head->next;
	if(q->head == NULL) q->tail = NULL;
	q->size -= 1;
	return head;
}
```
If the new `head` is `NULL` it means we are at the end of the queue, so we also will reset the `tail` pointer. Note that this type of queue is referred to as "First in first out" (FIFO), which BFS requires. This is in contrast to "Last in first out" (LIFO), which is used in program stacks and other graph search algorithms like DFS.

##### BFS
To begin, we'll represent the nodes we've visited as a map of booleans, with each key as the node id:
```c
bool visited[NUM_NODES] = {false};
```
To keep track of the full path, we'll store the parent of each node we visited as an array of integers. If the value is `-1` it means we have not visited the node yet:
```c
int parents[NUM_NODES];
for(int i = 0; i < NUM_NODES; i++){
	parents[i] = -1;
}
```
Ok, now we can start searching. We can initialize a queue and add the origin node as the first item:
```c
int start = 0;
int end = 3;

...

SearchQueue* q = createSearchQueue();
visited[start] = true;
enqueue(q, start);
bool found = false;
```

We can now begin our loop, which will continue until either a node is found or until the queue is empty:
```c
while (q->size > 0 && !found) {
...
}
```
Inside the loop we dequeue an item and iterate over its neighbors' connections. For each neighbor, if it has not yet been checked, we add it to the visited map, add the id of the node it points to. Then, if the neighbor is the final destination, we stop the search and return the result:
```c
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
```
Because we have tracked the path of each node, we can iterate through the `parents` array until the origin node is found. Visually this looks like this:
| Node | Parent | 
|----------|----------| 
| 1 | 0 |
| 2 | 1 | 
| 3 | 2| 

Given the destination node `3`, we find it's parent, `2`, which gives us its parent `1`, finally reaching back to `0`. An analogy to understand this approach of path reconstruction is breadcrumbs. For each node we visited, we left a trail back to the start.

Full implementation [here](https://github.com/jaqarrick/learning-algos/blob/main/examples/bfs_graph.c).

### Conclusion
BFS works best on _unweighted_ graphs, which we described above. In unweighted graphs, each edge is treated equally. Weighted graphs add extra qualifiers to edges. For example, a weighted graph describing transportation between cities might introduce factors like current traffic on routes, or total time it may take given transportation methods. In my music example above, I might factor in how far a friend lives from me, or even how well I know them. Algorithms that analyze directed and weighted graphs will need to be explored in another post. 

