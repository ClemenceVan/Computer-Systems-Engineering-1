#include "include.h"
#include "databus.h"

#define CD (1 << 14);
#define CE (1 << 15);
#define RD (1 << 16);
#define WR (1 << 17);


void displayPrintf(char *string, ...);
void clearDisplay(void);
void DisplayInit(void);
void Init_Display(void);