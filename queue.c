

#include "queue.h"
#include <stddef.h>
#include <stdlib.h>


// function creates a new queue in memory that holds the back and front
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->back = NULL;
    return queue;
}

// add page to back of queue
void enqueue(struct Queue* queue, struct rbtree_node* node) {
    // Create new page
    struct QueuePage* page = (struct QueuePage*)malloc(sizeof(struct QueuePage));
    page->node = node;
    page->next = NULL;
    page->previous = NULL;

    // If queue is empty
    if (queue->back == NULL) {
        queue->front = page;
        queue->back = page;
        return;
    }
    else {
        // add page to back of queue
        page->previous = queue->back;
        queue->back->next = page;
        queue->back = page;
    }
}

// pop page off queue
struct QueuePage* dequeue(struct Queue* queue) {
    // if queue is empty
    if (queue->front == NULL)
        return NULL;

    // store the dequeued page
    struct QueuePage* popped = queue->front;
    queue->front = queue->front->next;

    // If it was the last element
    if (queue->front == NULL) {
        queue->back = NULL;
    }
    else {
        queue->front->previous = NULL;
    }

    return popped;
}

void delete_page(struct QueuePage* page) {
    if(page->previous != NULL) {
        page->previous->next = page->next;
    }
    if(page->next != NULL) {
        page->next->previous = page->previous;
    }
}


struct QueuePage* peek(struct Queue* queue) {
    return queue->front;
}