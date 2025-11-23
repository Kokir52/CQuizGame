#pragma once
#ifndef ASS4_PLAYER_H
#define ASS4_PLAYER_H

#include "util.h"

/*
 * Initialization of global variables
 */

filedescriptors fds = {-1, -1, -1, -1};
mappings mmaps = {NULL, NULL, NULL, NULL};
pid_t process_id = -1;

/*
 * Default functions for player.c
 */

int startplayer();
void initSharedMemoryObjectsPlayer();
void initMemoryMappingsPlayer();
void initLocks();
void initProcess();
void closeMemoryMappingsPlayer();

void checkAndPrintResponse();

#endif // ASS4_PLAYER_H
