#pragma once
#include "../peripherals/display.h"
#include "../peripherals/keypad.h"
#include "../calendar.h"
#include "../include.h"

int homeScene();

int recordingsScene();

int gameScene();

int settingsScene();

static int (*screens[])(void) = {homeScene, recordingsScene, settingsScene, gameScene}; // add new scenes here

static int currentScreen = 0;