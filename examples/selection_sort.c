#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// To illuminate the benefits of a linked list for easier transactions we'll implement one here
typedef struct Node {
    int data; // to store an integer
    struct Node* next;
} Node;

typedef struct MaxNode {
    int max;
    struct Node* node;
} MaxNode;

Node* create_node(int data){
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append(Node** head_ref, int data){
    Node* new_node = create_node(data);

    // If the list is empty, make the new node the head node
    if(*head_ref == NULL){
        *head_ref = new_node;
        return;
    }

    // Find last node
    Node* last = *head_ref;
    while (last->next != NULL){
        last = last->next;
    }

    last->next = new_node;
    return;
}

void prepend(Node** head_ref, int data){
    Node* new_node = create_node(data);
    new_node->next = *head_ref;
    *head_ref = new_node;
}


void print_list(Node* head){
    Node* current = head;
    while(current != NULL){
        printf("%d\n", current->data);
        current = current->next;
    }
}

void free_list(Node* head){
    Node* current = head;
    Node* next;
    while(current != NULL){
        // grab the next item
        next = current->next;
        free(current);
        current = next;
    }
}

void remove_node(Node** head_ref, Node* node_to_remove){
    // if the node is the head, remove and point to next node
    if(*head_ref == node_to_remove){
        *head_ref = node_to_remove->next;
        free(node_to_remove);
        return;
    }

    Node* current = *head_ref;
    while(current != NULL && current->next != node_to_remove){
        current = current->next;
    }

    if(current != NULL){
        current->next = node_to_remove->next;
        free(node_to_remove);
    }
}

void selection_sort(Node** head_ref){
    Node* sorted_list_head = NULL;
    // go through linked list and find largest number
    while(*head_ref != NULL){
        MaxNode max_node;
        max_node.max = INT_MIN;  // Initialize to minimum possible value
        max_node.node = NULL;
        
        Node* current = *head_ref;
        while(current != NULL){
            int value = current->data;
            if(value > max_node.max){
                max_node.max = value;
                max_node.node = current;
            }
            current = current->next;
        }

        // prepend(&sorted_list_head, max_node.max);
        append(&sorted_list_head, max_node.max);
        // remove max node from list
        remove_node(head_ref, max_node.node);
    }

    *head_ref = sorted_list_head;
    printf("Sorted List:\n");
    print_list(*head_ref);
}

int main(){
    Node* head = NULL;
    int arr[] = {44, 111, 3, 5, 1, 3}; // yes, this is an array, but we can convert it to a linked list for our purposes
    int size = sizeof(arr) / sizeof(arr[0]);
    for(int i = 0; i < size; i++){
        // append(&head, arr[i]);
        prepend(&head, arr[i]); // we can use prepend instead with O(1) time complexity
    }
    print_list(head);
    selection_sort(&head);
    free_list(head);
    return 0;
}

