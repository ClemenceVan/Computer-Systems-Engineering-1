// #include <time.h>
// #include <stdio.h>

// typedef struct {
//     unsigned short hour;
//     unsigned short minute;
//     unsigned short second;
// } Time;

// typedef struct {
//     unsigned short day;
//     unsigned short month;
//     unsigned short year;
//     Time time;
// } Date;

// time_t now = 0;

// Date getDate() {
//     struct tm tm = *localtime(&now);
//     return (Date) {
//         .day = tm.tm_mday,
//         .month = tm.tm_mon + 1,
//         .year = tm.tm_year + 1900,
//         .time = (Time) {
//             .hour = tm.tm_hour,
//             .minute = tm.tm_min,
//             .second = tm.tm_sec
//         }
//     };
// }

// void setDateTime(Date date) {
//     struct tm tm = {
//         .tm_mday = date.day,
//         .tm_mon = date.month - 1,
//         .tm_year = date.year - 1900,
//         .tm_hour = date.time.hour,
//         .tm_min = date.time.minute,
//         .tm_sec = date.time.second
//     };
//     now = mktime(&tm);
// }

// char* dateToString(Date date) {
//     static char buffer[20];
//     sprintf(buffer, "%d/%d/%d %d:%d:%d", date.day, date.month, date.year, date.time.hour, date.time.minute, date.time.second);
//     return buffer;
// }

// void codeToExecute() {
//     static int temp = 0;
//     temp += 10;
//     if (temp >= 1000) {
//         temp = 0;
//         now += 1;
//     }
// }

// void main(void) {

//     struct timespec delay;
//     delay.tv_sec = 0;
//     delay.tv_nsec = 10000000; // 10 ms in nanoseconds
//     setDateTime((Date) {
//         .day = 19,
//         .month = 12,
//         .year = 2023,
//         .time = (Time) {
//             .hour = 21,
//             .minute = 29,
//             .second = 0
//         }
//     });

//     while (1) {
//         Date date = getDate();
//         printf("%s\n", dateToString(date));
//         // Execute the code
//         codeToExecute();

//         // Wait for 10 ms
//         if (nanosleep(&delay, NULL) == -1) {
//             perror("nanosleep");
//             return;
//         }
//     }
// }

#include <stdio.h>
#include <stdlib.h>

struct Node;

typedef struct Node_t {
    float timestamp;
    float temperature;
    struct Node_t* next;
} Node;

typedef struct {
    Node *temperatures;
} calendar;

calendar Calendar = {
    NULL
};

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

void main(void) {
    for (int i = 20; i > 0; i--) {
        //insert at the beginning
        Node* node = (Node*) malloc(sizeof(Node));
        node->timestamp = i;
        node->temperature = i;
        node->next = Calendar.temperatures;
        Calendar.temperatures = node;
    }
    for (Node* node = Calendar.temperatures; node != NULL; node = node->next) {
        printf("%f ", node->timestamp);
    }
    printf("\n");
    manageReadings();
    for (Node* node = Calendar.temperatures; node != NULL; node = node->next) {
        printf("%f ", node->timestamp);
    }
    printf("\n");
    clearReadings(Calendar.temperatures);
}