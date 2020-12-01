#include "redblack_tree.h"

#ifndef QUEUE
#define QUEUE

struct QueuePage {
    rbtree_node node;
    struct QueuePage* next;
    struct QueuePage* previous;
    void* memory_ref;
};

struct Queue {
    struct QueuePage *front;
    struct QueuePage *back;
};

#endif //PROJ4_CS537_QUEUE_H
