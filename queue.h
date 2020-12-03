
#ifndef PROJ4_CS537_QUEUE_H
#define PROJ4_CS537_QUEUE_H

struct QueuePage {
    unsigned long pid;
    unsigned long vpn;
    struct QueuePage* next;
    struct QueuePage* previous;
    void* memory_ref;
};

struct Queue {
    struct QueuePage *front;
    struct QueuePage *back;
};

struct Queue* createQueue();

void enqueue(struct Queue* queue, unsigned long pid, unsigned long vpn);

struct QueuePage* dequeue(struct Queue* queue);

void delete_page(struct QueuePage* page);

struct QueuePage* peek(struct Queue* queue);

#endif //PROJ4_CS537_QUEUE_H
