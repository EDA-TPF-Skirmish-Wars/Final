#pragma once
#include "Game.h"
void * callbackClient(const char* mapName, unsigned int mapNameSize, char checksum, void * screen);

bool callback(move_s move, int data1, int data2, int data3, int data4, int data5, void * game);

int callbackResponseAttack(void);