#include "shmo/queue.h"

int main(void) {
    Queue *q = queue_create(stdalloc);

    if (!queue_enqueue(q, bytes_of_str("first"))) {
        // handle error
    }
    queue_enqueue(q, bytes_of_str("second"));
    queue_enqueue(q, bytes_of_str("third"));
    queue_enqueue(q, bytes_of_str("fourth"));

    while (!queue_empty(q)) {
        const char *elm = bytes_to_str(queue_front(q));
        printf("%s\n", elm);
        queue_dequeue(q);
    }

    queue_destroy(q);
    return 0;
}
