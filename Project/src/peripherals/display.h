#pragma once
#include "../include.h"
#include "./databus.h"

#define CD (1 << 14)
#define CE (1 << 15)
#define RD (1 << 16)
#define WR (1 << 17)

#define DATABUS_MASK 8 << 2
#define DISPLAYCONTROL_MASK CD | CE | RD | WR

#define DISPLAY_WIDTH 40
#define DISPLAY_HEIGHT 15

typedef struct {
    /* 
    * displayPrintfAt
    * 
    * Prints a string to the display at the specified position.
    * 
    * @param pos: The position to print the string at.
    * @param string: The string to print.
    * @param ...: The arguments to the string.
    */
    void(*printfAt)(int pos[2], char* string, ...);
    /*
    * clear
    *
    * Clears the display.
    */
    void(*clear)(void);
    /*
    * init
    *
    * Initializes the display.
    */
    void(*init)(void);
    /*
    * enable
    *
    * Enables the display.
    */
    void(*enable)(void);
    databus db;
} display;

extern display Display;