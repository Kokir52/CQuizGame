#ifndef ASS4_UTIL_H
#define ASS4_UTIL_H

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

/*
 * Use those defines for the name required
 * to open shared objects
 */

#define SHM_NAME_REQUEST "/request"
#define SHM_NAME_RESPONSE "/response"
#define SHM_NAME_LOCKS "/locks"
#define SHM_NAME_QUIZSTATE "/quizsstate"

/*
 * Use those defines for checking the commands
 */

#define CMD_START "start"
#define CMD_ANSWER "answer"
#define CMD_SKIP "skip"
#define CMD_EXIT "exit"

/*
 * Other game defines. Use whenever possible.
 */

#define REQUEST_MAX_LENGTH 32
#define RESPONSE_MAX_LENGTH 512
#define NUM_QUESTIONS_PER_GAME 4
#define NUM_ANSWERS_PER_QUESTION 4

/*
 * Can be used for testing
 */

#define TEST_STR "DEAD"
#define LENGTH_TEST_STR 0x4

/*
 * Important and useful predefined values
 * for shared objects
 */

#define FLAGS_SHM_READONLY O_RDONLY | O_CREAT
#define FLAGS_SHM_READWRITE O_RDWR | O_CREAT
#define MODERW S_IRUSR | S_IWUSR

/*
 *  Error codes you can use for testing.
 */

#define ERROR_SHM_PLAYER -1
#define ERROR_MMAP_PLAYER -3
#define ERROR_RES_PLAYER -5
#define ERROR_UNMAP_CLOSE_PLAYER -7

#define ERROR_SHM_BOARD -2
#define ERROR_MMAP_BOARD -4
#define ERROR_REQ_BOARD -6
#define ERROR_UNMAP_CLOSE_BOARD -8


/*
 * file descriptors of your shared objects
 */
typedef struct
{
  int fd_shm_request_;
  int fd_shm_response_;
  int fd_shm_locks_;
  int fd_shm_quizstate_;
} filedescriptors;

/*
 * shared objects for your locks / semaphores / cond variables (you don't necessarily need them all)
 */
typedef struct
{
  // TODO Student START
  sem_t board_init_done;
  sem_t player_input;
  sem_t board_check_command;
  sem_t board_ready_for_new_command;
  // TODO Student END
} shmlocks;

/*
 * the response struct
 */
typedef struct
{
  char message[RESPONSE_MAX_LENGTH];
} shmresponse;

/*
 * the adventure state struct
 */
typedef struct
{
  int question_ids_[NUM_QUESTIONS_PER_GAME];
  int current_question_index_;
  char answers_[NUM_QUESTIONS_PER_GAME];
  bool game_active_;
} shmquizsstate;

/*
 * the request struct
 */
typedef struct
{
  char message[REQUEST_MAX_LENGTH];
} shmrequest;

/*
 *the according address pointing to the mapped region
 */
typedef struct
{
  shmrequest* request_;
  shmresponse* response_;
  shmlocks* locks_;
  shmquizsstate* quizstate_;
} mappings;

/*
 * global variables. You MUST USE them!!!!
 */
extern filedescriptors fds;
extern pid_t process_id;
extern mappings mmaps;

/*
 * check functions you should not move in the code (you can implement your own functionality in util.c)
 */
void checkCleanup();
void removeAllSHM();
void checkResults();
int checkSetup();
void checkSHMBoard();
void checkMMAPBoard();
void checkSHMPlayer();
void checkMMAPPlayer();
void checkBoard();
void checkCleanup();

int checkProgress();
int checkProgressPlayer();
int checkProgressBoard();


extern int startplayer();
extern int startboard();

/*
 * Useful functions to use
 */

/**
 * @brief Read command from the user.
 *
 * @param buffer The buffer to store the command.
 * @param size The size of the buffer.
 * @return True if the command was read successfully, false otherwise.
 */
bool readCommand(char* buffer, size_t size);

#endif // ASS4_UTIL_H
