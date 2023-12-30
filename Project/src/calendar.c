#include "calendar.h"


time_t getTimestamp() {
    return Calendar.now;
}

time_t getEndOfDay() {
    struct tm tm = *localtime(&Calendar.now);
    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 59;
    return mktime(&tm);
}

Date getDate() {
    struct tm tm = *localtime(&Calendar.now);
    return (Date) {
        .day = tm.tm_mday,
        .month = tm.tm_mon + 1,
        .year = tm.tm_year + 1900,
        .time = (Time) {
            .hour = tm.tm_hour,
            .minute = tm.tm_min,
            .second = tm.tm_sec
        }
    };
}

Date getDateFromTimestamp(time_t timestamp) {
    struct tm tm = *localtime(&timestamp);
    return (Date) {
        .day = tm.tm_mday,
        .month = tm.tm_mon + 1,
        .year = tm.tm_year + 1900,
        .time = (Time) {
            .hour = tm.tm_hour,
            .minute = tm.tm_min,
            .second = tm.tm_sec
        }
    };
}

void setDateTime(Date date) {
    struct tm tm = {
        .tm_mday = date.day,
        .tm_mon = date.month - 1,
        .tm_year = date.year - 1900,
        .tm_hour = date.time.hour,
        .tm_min = date.time.minute,
        .tm_sec = date.time.second
    };
    Calendar.now = mktime(&tm);
}

char* dateToString(Date date) {
    static char buffer[20];
    sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d", date.day, date.month, date.year, date.time.hour, date.time.minute, date.time.second);
    return buffer;
}

void clearReadings(Node *startNode) {
    while (startNode != NULL) {
        Node *temp = startNode;
        startNode = startNode->next;
        free(temp); // Free the memory associated with the node
    }
}

void manageReadings() {
    Node *node = Calendar.temperatures;
    Node *prev = NULL;
    time_t limit = Calendar.now - READING_EXPIRATION;

    // Traverse the list
    for (; node != NULL; node = node->next) {
        if (node->timestamp < limit) {
            // Clear rest of the list
            clearReadings(node);
            if (prev != NULL) {
                prev->next = NULL; // Update the next pointer of the previous node
            } else {
                Calendar.temperatures = NULL; // Update the head of the list
            }
            break;
        }
        prev = node;
    }
}

void addTemperature(float temperature) {
    manageReadings();
    Node* node;
    for (node = malloc(sizeof(Node)); node == NULL; node = malloc(sizeof(Node))) {
        //delete last node
        Node *temp = Calendar.temperatures;
        while (temp->next->next != NULL) temp = temp->next;
        free(temp->next);
        temp->next = NULL;
    }
    node->timestamp = getTimestamp();
    node->temperature = temperature;
    node->next = Calendar.temperatures;
    Calendar.temperatures = node;
    // printf("Added temperature: %f\n", temperature);
}

void printList(Node *startNode) {
    while (startNode != NULL) {
        printf("%f %d\n", startNode->temperature, startNode->timestamp);
        startNode = startNode->next;
    }
    printf("\n");
}

// create copy of list from startNode to endNode
Node *getRecordings(time_t from, time_t to) {
    Node *node = Calendar.temperatures;
    Node *startNode = NULL;
    Node *prev = NULL;
    // Traverse the list
    for (; node != NULL; node = node->next) {
        if (node->timestamp > from)
            continue;
        if (node->timestamp < to)
            break;
        Node *newNode = (Node*) malloc(sizeof(Node));
        newNode->timestamp = node->timestamp;
        newNode->temperature = node->temperature;
        newNode->next = NULL;
        if (startNode == NULL) {
            startNode = newNode;
        } else {
            prev->next = newNode;
        }
        prev = newNode;
    }
    return startNode;
}

void freeList(Node *startNode) {
    while (startNode != NULL) {
        Node *temp = startNode;
        startNode = startNode->next;
        free(temp); // Free the memory associated with the node
    }
}

calendar Calendar = {
    .now = 0,
    .temperatures = NULL,
    .getNow = getDate,
    .getEndOfDay = getEndOfDay,
    .getDateFromTimestamp = getDateFromTimestamp,
    .setDateTime = setDateTime,
    .toString = dateToString,
    .addTemperature = addTemperature,
    .getRecordings = getRecordings,
    .freeList = freeList
};