#include "include.h"
#include "databus.h"

#define CD (1 << 14)
#define CE (1 << 15)
#define RD (1 << 16)
#define WR (1 << 17)

#define DATABUS_MASK 8 << 2
#define DISPLAYCONTROL_MASK CD | CE | RD | WR

#define DISPLAY_WIDTH 40
#define DISPLAY_HEIGHT 16

typedef struct {
    void(*printfAt)(int pos[2], char* string, ...);
    void(*clear)(void);
    void(*init)(void);
    void(*enable)(void);
    databus db;
} display;

extern display Display;