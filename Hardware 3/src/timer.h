#include "include.h"


typedef struct {
    int temp;
    int light;
} flags;

typedef struct {
    int ms;
    void (*init)(int interval, void (*callback)(void));
    flags Flags;
} timer;

extern timer Timer;