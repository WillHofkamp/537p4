#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b);
void exit_with_message(char *message);
struct node *make_node(int key, int pid);
void add_node(void **root, struct node *node);
struct node *find_node(void **root, int key);
void delete_node(void **root, int key);
void free_node(void *ptr);