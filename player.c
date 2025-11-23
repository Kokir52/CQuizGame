#include "player.h"

/******************************************************************
 * Main routine that contains the game loop. After the initial setup,
 * requests are made and published to the board.
 *
 * Stops if the 'exit' command is recognized or if EOF (CTRL+D) is
 * recognized, both should terminate the program properly.
 *
 * Initialize everything needed in here. Checks in this function must
 * not be relocated.
 *
 * Do NOT use any synchronization mechanisms outside of the TODO blocks.
 *
 * @param none
 *
 * @return predefined exit codesx
 */
int startplayer()
{
  initSharedMemoryObjectsPlayer();
  checkSHMPlayer();
  initMemoryMappingsPlayer();
  checkMMAPPlayer();
  initLocks();
  initProcess();
  checkBoard();

  // TODO Student START
  // init additional variables here if needed (optional)
  // wait for welcome message from board
  sem_wait(&mmaps.locks_->board_init_done);
  // TODO Student END

  checkAndPrintResponse();

  char buffer[REQUEST_MAX_LENGTH];
  do
  {
    printf("[PLAYER] ");

    // TODO Student START
    // use the provided function readCommand()
    // exit the loop if no command (EOF) is received or the command is
    // 'exit' (exit still has to be handled by the board)
    sem_wait(&mmaps.locks_->board_ready_for_new_command);
    // input command
    if(readCommand(buffer, REQUEST_MAX_LENGTH) == false) {
      strcpy(mmaps.request_->message, CMD_EXIT);
      strcpy(buffer, CMD_EXIT);
    } else
    {
      memset(mmaps.request_->message, 0, REQUEST_MAX_LENGTH * sizeof(char)); // this thing was worth 10 pts???
      strcpy(mmaps.request_->message, buffer);  
    }
    
    // some kind of sem_post
    sem_post(&mmaps.locks_->player_input);

      

    // wait for board to check the command 
    sem_wait(&mmaps.locks_->board_check_command);

    // - use the provided function readCommand()
    // - make sure the loop properly stops if 'exit' command is entered
    //   or EOF is received
    // - make sure that for both exit scenarios types the board properly
    //   handles the terminates as well

    // TODO Student END

    checkAndPrintResponse();
    checkResults();

  } while (strcmp(buffer, CMD_EXIT) != 0);

  closeMemoryMappingsPlayer();
  checkCleanup();

  return 0;
}

/******************************************************************
 *  This function starts and initializes the Board process.
 *  Use the predefined variable process_id for the pid of the newly created
 *  process. You can ignore process_id in the Board process.
 *
 *  @param none
 *
 *  @return none
 */
void initProcess()
{
  if (checkSetup())
  {
    return;
  }

  // TODO Student START
  // start the Board process and load the right executable (hint: look inside build directory)
  process_id = fork();
  if( process_id != 0 ) {
  }  else   {
    char* argv[] = {"./build/board", "argument",0};
    execv("./build/board",argv);
  }
  

  // TODO Student END
}

/******************************************************************
 * Initializes your shared objects of the Player and resizes them.
 * Make sure you only give the permissions the Player needs. But please
 * use MODERW, for compatibility with the testsystem.
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoryObjectsPlayer()
{
    // TODO Student START
    //make sure the flags are ok    Name              Flags             Mode
    fds.fd_shm_locks_ = shm_open(SHM_NAME_LOCKS, FLAGS_SHM_READWRITE, MODERW);
    fds.fd_shm_quizstate_ = shm_open(SHM_NAME_QUIZSTATE, FLAGS_SHM_READONLY, MODERW);
    fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READWRITE, MODERW);
    fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READONLY, MODERW);

    if (fds.fd_shm_quizstate_ == -1 || fds.fd_shm_locks_ == -1 || 
    fds.fd_shm_request_ == -1 || fds.fd_shm_response_ == -1)
    {
      printf("failed shm_open\n");
    }
    
    if (ftruncate(fds.fd_shm_locks_, 4096) == -1)
    {
      printf("failed ftruncate1 p\n");
    }
    /*if (ftruncate(fds.fd_shm_quizstate_, 4096) == -1)
    {
      printf("failed ftruncate2 p\n");
    }*/
    if (ftruncate(fds.fd_shm_request_, 4096) == -1)
    {
      printf("failed ftruncate3 p\n");
    }
    /*if (ftruncate(fds.fd_shm_response_, 4096) == -1)
    {
      printf("failed ftruncate4 p\n");
    }*/


    // TODO Student END
}

/******************************************************************
 * Maps the shared objects to the virtual memory space of the Player
 * Don't do anything else in this function.
 *
 * @param none
 *
 * @return none
 */
void initMemoryMappingsPlayer()
{
  if (checkProgressPlayer())
  {
    return;
  }

  // TODO Student START
  mmaps.locks_ = (shmlocks*) mmap(0, 4096, PROT_WRITE, MAP_SHARED, fds.fd_shm_locks_, 0);
  mmaps.quizstate_ = (shmquizsstate*) mmap(0, 4096, PROT_READ, MAP_SHARED, fds.fd_shm_quizstate_, 0);
  mmaps.request_ = (shmrequest*) mmap(0, 4096, PROT_WRITE, MAP_SHARED, fds.fd_shm_request_, 0);
  mmaps.response_ = (shmresponse*) mmap(0, 4096, PROT_READ, MAP_SHARED, fds.fd_shm_response_, 0);


  // TODO Student END
}

/******************************************************************
 * Initializes the locks of the shared object
 *
 * @param none
 *
 * @return none
 */
void initLocks()
{
  // TODO Student START
  sem_init(&mmaps.locks_->board_init_done, 1, 0);
  sem_init(&mmaps.locks_->player_input, 1, 0);
  sem_init(&mmaps.locks_->board_check_command, 1, 0);
  sem_init(&mmaps.locks_->board_ready_for_new_command, 1, 0);

  // TODO Student END
}

/******************************************************************
 * Removes all mmaps and shared objects as seen from the Player
 * This part is an essential function for closing this application
 * accordingly without leaving artifacts on your system!
 *
 * @param none
 *
 * @return none
 */
void closeMemoryMappingsPlayer()
{
  if(checkProgress())
  {
    return;
  }

  // TODO Student START

  // dont forget the locks
  sem_destroy(&mmaps.locks_->board_check_command);
  sem_destroy(&mmaps.locks_->board_init_done);
  sem_destroy(&mmaps.locks_->player_input);



  munmap(mmaps.locks_, 4096);
  munmap(mmaps.quizstate_, 4096);
  munmap(mmaps.request_, 4096);
  munmap(mmaps.response_, 4096);

  close(fds.fd_shm_locks_);
  close(fds.fd_shm_quizstate_);
  close(fds.fd_shm_request_);
  close(fds.fd_shm_response_);


  shm_unlink(SHM_NAME_LOCKS);
  shm_unlink(SHM_NAME_QUIZSTATE);
  shm_unlink(SHM_NAME_REQUEST);
  shm_unlink(SHM_NAME_RESPONSE);


  // TODO Student END
}

/******************************************************************
 * This function is used to check the response from the Board and print it.
 *
 * @param none
 *
 * @return none
 */
void checkAndPrintResponse(void) {
  printf("[BOARD] %s\n", mmaps.response_->message);
}
