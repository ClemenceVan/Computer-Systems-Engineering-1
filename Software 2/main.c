#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct LinkedList {
    int id;
    double sensorData;
    struct LinkedList *next;
} element;

void insertFirst(struct LinkedList **first, struct LinkedList *el) {
    el->next = first ? *first : NULL;
    *first = el;
};

int isMember (struct LinkedList **first, struct LinkedList *el) {
    for (struct LinkedList *temp = *first; temp != NULL; temp = temp->next)
        if (temp == el)
            return 1;
    return 0;
}

void printList(struct LinkedList *first) {
    int count = 0;
    for (struct LinkedList *temp = first; temp != NULL; temp = temp->next, count++)
        printf("[%d] ID: %d, Sensor Data: %f\n", count, temp->id, temp->sensorData);
};

void removeElement(struct LinkedList **first, struct LinkedList *el) {
    struct LinkedList *temp = *first;
    struct LinkedList *prev = NULL;

    for (;temp != NULL && temp != el; prev = temp, temp = temp->next);// Loop until element is found or end of list is reached
    if (temp == NULL) return; // Element not found
    if (temp == *first) // Element is head
        *first = temp->next;
    else
        prev->next = temp->next;

    temp->next = NULL; // Remove reference to next element
}

struct LinkedList * readSensor(int id) {
    element *el = malloc(sizeof(element));
    if (!el) {
        printf("Error allocating memory\n");
        exit(1);
    }

    el->id = id;
    el->sensorData = (double)rand() / RAND_MAX;
    el->next = NULL;
    return el;
}

void sortLinkedList(element **first) {
    element *sorted = NULL;
    element *biggest = NULL;

    while (*first != NULL) {
        for (element *elem = *first; elem != NULL; elem = elem->next)
            if (biggest == NULL || elem->sensorData > biggest->sensorData)
                biggest = elem;

        removeElement(first, biggest);
        insertFirst(&sorted, biggest);
        biggest = NULL;
    }

    *first = sorted;
}

void freeLinkedList(struct LinkedList **first) {
    for (struct LinkedList *temp = *first, *toDelete = NULL; temp != NULL;) {
        toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }
    *first = NULL;
}

int main() {
    srand(time(NULL));
    // element el1 = {1, 1.1, NULL};
    struct LinkedList *first = NULL;
    insertFirst(&first, readSensor(1));
    printf("%d\n", first->id);
    insertFirst(&first, readSensor(2));
    insertFirst(&first, readSensor(3));
    insertFirst(&first, readSensor(4));
    insertFirst(&first, readSensor(5));
    // printf("%d, %d\n", isMember(&first, &el4), isMember(&first, &el3));
    printList(first);
    sortLinkedList(&first);
    printf("\n\n");
    printList(first);
    freeLinkedList(&first);
    printf("\n\n");
    printList(first);
    return 0;
}