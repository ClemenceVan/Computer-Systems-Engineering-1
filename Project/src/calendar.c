#include "calendar.h"


time_t getTimestamp() {
    return Calendar.now;
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

    // Traverse the list
    for (; node != NULL; node = node->next) {
        if (node->timestamp > 10) {
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
    // manageReadings();
    Node* node = (Node*) malloc(sizeof(Node));
    node->timestamp = getTimestamp();
    node->temperature = temperature;
    node->next = Calendar.temperatures;
    Calendar.temperatures = node;
    printf("Added temperature: %f\n", temperature);
}

calendar Calendar = {
    .now = 0,
    .temperatures = NULL,
    .getNow = getDate,
    .setDateTime = setDateTime,
    .toString = dateToString,
    .addTemperature = addTemperature
};