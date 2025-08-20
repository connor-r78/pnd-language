#include "gc.h"
#include <stdio.h>

typedef struct Node {
    struct Node *left, *right;
    int value;
} Node;

int main(void) {
    // Step 1: Initialize the GC with a local stack pointer
    int stack_bottom;
    gc_init(&stack_bottom);

    // Step 2: Allocate some nodes
    Node *root = GC_NEW(Node);
    root->value = 1;

    Node *child1 = GC_NEW(Node);
    child1->value = 2;
    root->left = child1;

    Node *child2 = GC_NEW(Node);
    child2->value = 3;
    root->right = child2;
    
    // Step 3: Register root pointer if you want explicit root tracking
    gc_register_root((void **)&root);

    // Step 4: Allocate more nodes without keeping references
    for (int i = 0; i < 10; ++i) {
        Node *temp = GC_NEW(Node);
        temp->value = i;
        // Not stored anywhere → eligible for collection
    }

    // Step 5: Trigger garbage collection manually
    gc_collect();

    // Step 6: Check that your root is still alive
    printf("Root value: %d\n", root->value);
    printf("Left child value: %d\n", root->left->value);
    printf("Right child value: %d\n", root->right->value);

    return 0;
}