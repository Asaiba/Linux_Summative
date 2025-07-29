#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 10

int queue[MAX_QUEUE_SIZE];
int count = 0;             // Current number of items in the queue
int front = 0, rear = 0;   // Circular queue indices

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;

/**
 * Thread-safe enqueue operation.
 * Blocks if queue is full.
 */
void* producer(void* arg) {
    int item_id = 1;
    while (1) {
        sleep(2);  // Simulate production time

        pthread_mutex_lock(&mutex);

        // Wait if queue is full
        while (count == MAX_QUEUE_SIZE) {
            printf("[Producer] Queue full. Waiting...\n");
            pthread_cond_wait(&cond_not_full, &mutex);
        }

        // Produce item
        queue[rear] = item_id;
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        count++;
        printf("[Producer] Produced item %d. Queue size: %d\n", item_id, count);
        item_id++;

        // Signal consumer that queue is not empty
        pthread_cond_signal(&cond_not_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/**
 * Thread-safe dequeue operation.
 * Blocks if queue is empty.
 */
void* consumer(void* arg) {
    while (1) {
        sleep(3);  // Simulate packaging time

        pthread_mutex_lock(&mutex);

        // Wait if queue is empty
        while (count == 0) {
            printf("[Consumer] Queue empty. Waiting...\n");
            pthread_cond_wait(&cond_not_empty, &mutex);
        }

        // Consume item
        int item = queue[front];
        front = (front + 1) % MAX_QUEUE_SIZE;
        count--;
        printf("[Consumer] Packed item %d. Queue size: %d\n", item, count);

        // Signal producer that queue is not full
        pthread_cond_signal(&cond_not_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/**
 * Main function to start producer and consumer threads.
 */
int main() {
    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Let the simulation run indefinitely
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
