 #define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <search.h>

// This is what we are storing in our redblack tree

struct node {
    int key;
    int pid;
};

int procStart;
int procEnd;

// This is the compare function for the redblack tree
// Returns -1 if a <  b
// Returns 1  if a >  b
// Returns 0  if a == b
int compare(const void *a, const void *b)
{
    struct node *node_a = (struct node *)a;
    struct node *node_b = (struct node *)b;

    if (node_a->key < node_b->key)
        return -1;
    else if (node_a->key > node_b->key)
        return 1;
    else
        return 0;
}

// This function prints an error message and exits
void exit_with_message(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

// Make node for redblack tree
struct node *make_node(int key, int pid)
{
    struct node *node;

    if ((node = malloc(sizeof(struct node))) == NULL)
        exit_with_message("Failed to malloc");

    node->key = key;
    node->pid = pid;

    return node;
}

// Add the node to the redblack tree
void add_node(void **root, struct node *node)
{
    void *result;
    struct node *existing;
    
    if(procStart == NULL || node->key < procStart) {
        procStart = node->key;
    } else if(procEnd == NULL || node->key > procEnd) {
        procEnd = node->key;
    }

    if ((result = tsearch(node, root, compare)) == NULL) {
        // Failed to add the node
        exit_with_message("Insufficient memory");
    } else {
        // Check if an node with the same key already existed
        existing = *(struct node **)result;

        if (existing != node) {
            printf("Node with key already exists. ");
            printf("PID: %d, pid: %d\n", existing->key, existing->pid);
            free(node);
        } else {
            printf("Added node. key: %d, pid: %d\n", node->key, node->pid);
        }
    }
}

// Find a node in the redblack tree
// Returns NULL if no node is found
struct node *find_node(void **root, int key)
{
    void *result;
    struct node *node;
    struct node search_node;

    search_node.key = key;

    if ((result = tfind(&search_node, root, compare)) == NULL) {
        // No node found
        printf("No node found. key: %d\n", key);
        node = NULL;
    } else {
        // Node found
        node = *(struct node**)result;
        printf("Found node. key: %d, PID: %d\n", node->key, node->pid);
    }

    return node;
}

// Delete a node in the redblack tree
// If it is not found, do nothing
void delete_node(void **root, int key)
{
    struct node *node;

    if ((node = find_node(root, key)) == NULL) {
        // Nothing to delete
    } else {
        tdelete(node, root, compare);
        printf("Deleted node. key: %d, PID: %d\n", node->key, node->pid);
        // It's important to free the only after deleting it
        free(node);
    }
}

// Free node when destroying the tree
void free_node(void *ptr)
{
    struct node *node = ptr;

    printf("Freeing node during tree destroy. key: %d, PID: %d\n",
            node->key, node->pid);

    free(node);
}

int main()
{
    void *root;
    struct node *node;

    // Initialize the binary tree
    root = NULL;

    printf("ADDING NODES\n------------------------------\n");
    add_node(&root, make_node(1, 1435));
    add_node(&root, make_node(2, 1436));
    add_node(&root, make_node(1, 1437));
    add_node(&root, make_node(4, 1438));
    add_node(&root, make_node(5, 1439));

    printf("\nFINDING NODES\n------------------------------\n");
    node = find_node(&root, 1);
    node = find_node(&root, 3);

    printf("\nDELETING NODES\n------------------------------\n");
    delete_node(&root, 1);
    delete_node(&root, 2);

    printf("\nDESTROYING TREE\n------------------------------\n");
    tdestroy(root, free_node);
}
